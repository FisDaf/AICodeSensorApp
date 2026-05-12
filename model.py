import html
import torch
import numpy as np

from model_loader import model, tokenizer, device


def predict_long_code(text, model, tokenizer, device, window_size=512, stride=256):
    """Предсказание по скользящему окну для длинных файлов"""
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


def render_html_result(text, probs, offsets, threshold, status, status_color, suspicious_count, total_tokens, confidence):
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
        chunk = html.escape(text[start:end])

        if prob > threshold:
            color = f"rgba(255, 50, 50, {prob:.3f})"
            html_out += f'<span style="background-color: {color}; border-radius: 2px;">{chunk}</span>'
        else:
            html_out += chunk
            
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
        
        if lines_count < 25 and chars_count < 1000:
            threshold = 0.85
        else:
            threshold = 0.7
            
        confidence = float(max(probs)) if len(probs) > 0 else 0.0
        total_tokens = len([1 for start, end in offsets if start != end])
        suspicious_count = sum(1 for prob in probs if prob > threshold)
        density = suspicious_count / total_tokens if total_tokens > 0 else 0
        
        if density > 0.15:
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
            
        return verdict, html_result


class CodeSensorModel_first:
    def __init__(self):
        self.sensor = CodeSensorModel()

    def get_verdict(self, code_text: str) -> int:
        verdict, _ = self.sensor.analyze(code_text)
        return verdict


class CodeSensorModel_second:
    def __init__(self):
        self.sensor = CodeSensorModel()

    def process_code(self, code_text: str) -> str:
        _, html_result = self.sensor.analyze(code_text)
        return html_result