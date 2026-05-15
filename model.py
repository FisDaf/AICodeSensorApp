"""
model.py - Анализаторы кода и функции для детектирования уязвимостей
"""

import html
import torch
import numpy as np

from model_loader import model, tokenizer, device


def predict_long_code(text, model, tokenizer, device, window_size=512, stride=256):
    """Предсказание по скользящему окну для длинных файлов (без сглаживания)"""
    model.eval()
    
    inputs = tokenizer(
        text, 
        return_tensors="pt", 
        truncation=False, 
        return_offsets_mapping=True
    )
    
    all_input_ids = inputs["input_ids"][0]
    all_offsets = inputs["offset_mapping"][0].tolist()
    seq_len = all_input_ids.size(0)
    
    token_probs_sum = np.zeros(seq_len)
    token_counts = np.zeros(seq_len)
    
    for start_idx in range(0, seq_len, stride):
        end_idx = min(start_idx + window_size, seq_len)
        
        window_ids = all_input_ids[start_idx:end_idx].unsqueeze(0).to(device)
        window_mask = torch.ones_like(window_ids).to(device)
        
        with torch.no_grad():
            logits = model(window_ids, window_mask)
            probs = torch.softmax(logits, dim=-1)[0, :, 1].cpu().numpy()
            
        token_probs_sum[start_idx:end_idx] += probs
        token_counts[start_idx:end_idx] += 1
        
        if end_idx == seq_len:
            break
            
    final_probs = token_probs_sum / token_counts
    tokens = tokenizer.convert_ids_to_tokens(all_input_ids)
    
    return tokens, final_probs, all_offsets


def remove_isolated_highlights(probs, offsets, code_text, threshold, min_group_size=7):
    """
    Улучшенный фильтр: удаляет группы, если они слишком короткие 
    ИЛИ если они состоят в основном из мусорных символов (скобки, знаки).
    """
    is_suspicious = probs > threshold
    new_probs = probs.copy()
    
    i = 0
    while i < len(is_suspicious):
        if is_suspicious[i]:
            start = i
            while i < len(is_suspicious) and is_suspicious[i]:
                i += 1
            end = i
            
            group_len = end - start
            
            # Извлекаем текст всей подозрительной группы
            group_text = ""
            for idx in range(start, end):
                s, e = offsets[idx]
                group_text += code_text[s:e]
            
            # Очищаем текст от скобок и знаков для проверки "содержательности"
            content_only = "".join([c for c in group_text if c.isalnum()])
            
            if group_len < min_group_size or len(content_only) < 3:
                new_probs[start:end] = 0
        else:
            i += 1
            
    return new_probs


def render_html_result(text, probs, offsets, threshold, status, status_color, suspicious_count, total_tokens, confidence):
    """Генерирует HTML на основе вероятностей с фильтрацией мусорных токенов"""
    html_out = f"<h3>Анализ файла с использованием AI:</h3>"
    html_out += f"<p style='color: {status_color}; font-weight: bold;'>Статус: {status}</p>"
    html_out += f"<p>Найдено {suspicious_count} подозрительных токенов из {total_tokens}. "
    html_out += f"Макс. уверенность: {confidence:.2%}, порог: {threshold:.2f}</p>"

    html_out += "<pre style='background-color: #1e1e1e; color: #ccc; padding: 10px; white-space: pre-wrap; word-wrap: break-word;'>"

    last_idx = 0
    for (start, end), prob in zip(offsets, probs):
        if start == end:
            continue

        html_out += html.escape(text[last_idx:start])
        
        chunk_raw = text[start:end]
        chunk_escaped = html.escape(chunk_raw)

        # ЭВРИСТИКА: Игнорируем пробелы, переносы строк и одиночные скобки/пунктуацию
        is_meaningful = len(chunk_raw.strip()) > 0 and chunk_raw.strip() not in ['{', '}', ';', '\n', '\r']

        # Красим только если вероятность выше порога И токен имеет смысл
        if prob > threshold and is_meaningful:
            # Сплошная заливка 0.4 для ровного цвета (выглядит аккуратнее, чем градиент по вероятности)
            color = "rgba(255, 85, 85, 0.4)" 
            html_out += f'<span style="background-color: {color}; border-radius: 2px;">{chunk_escaped}</span>'
        else:
            html_out += chunk_escaped
            
        last_idx = end

    html_out += html.escape(text[last_idx:]) + "</pre>"
    return html_out


visualize_long_ai_code = render_html_result 


class CodeSensorModel:
    def __init__(self):
        self.version = "1.0.0-beta"

    def analyze(self, code_text: str) -> tuple[int, str]:
                
        if model is None:
            return 0, "<h3 style='color: gray;'>Модель не загружена</h3>"
        
        tokens, probs, offsets = predict_long_code(code_text, model, tokenizer, device)
        
        lines_count = len(code_text.split('\n'))
        chars_count = len(code_text)
        
        if lines_count < 25 or chars_count < 200:
            threshold = 0.85 
        else:
            threshold = 0.7
            
        probs = remove_isolated_highlights(probs, offsets, code_text, threshold, min_group_size=7)
        
        meaningful_probs = []
        for (start, end), prob in zip(offsets, probs):
            if start == end:
                continue
                
            chunk_raw = code_text[start:end]
            if len(chunk_raw.strip()) > 0 and chunk_raw.strip() not in ['{', '}', ';', '\n', '\r']:
                meaningful_probs.append(prob)
        
        confidence = float(max(meaningful_probs)) if len(meaningful_probs) > 0 else 0.0
        total_tokens = len(meaningful_probs)
        suspicious_count = sum(1 for p in meaningful_probs if p > threshold)
        density = suspicious_count / total_tokens if total_tokens > 0 else 0
        
        if density > 0.2:
            verdict = 2
            status = "ПОДОЗРИТЕЛЬНЫЙ"
            status_color = "#ff5555"
        elif density > 0.05:
            verdict = 1
            status = "ВНИМАНИЕ"
            status_color = "#ffb86c"
        else:
            verdict = 0
            
        if verdict >= 1:
            html_result = render_html_result(
                code_text, probs, offsets, threshold, 
                status, status_color, suspicious_count, total_tokens, confidence
            )
        else:
            html_result = "<h2 style='color: #50fa7b;'>Файл безопасен</h2>"
            
        print(f'Вердикт: {verdict}, Подозрительных токенов: {suspicious_count}/{total_tokens}, Макс. уверенность: {confidence:.2%}')
                    
        return verdict, html_result


class CodeSensorModel_first:
    """Классификатор уязвимостей в коде"""
    def __init__(self):
        self.sensor = CodeSensorModel()

    def get_verdict(self, code_text: str) -> int:
        verdict, _ = self.sensor.analyze(code_text)
        return verdict


class CodeSensorModel_second:
    """Детальный анализ подозрительного кода"""
    def __init__(self):
        self.sensor = CodeSensorModel()

    def process_code(self, code_text: str) -> str:
        _, html_result = self.sensor.analyze(code_text)
        return html_result