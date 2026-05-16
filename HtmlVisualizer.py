from CodeSensor import CodeSensorResult
import html

class HtmlVisualizer:
    @staticmethod
    def __render_suspicious(text, csm_result: CodeSensorResult, status, status_color):
        html_out = f"<h3>Анализ файла с использованием AI:</h3>"
        html_out += f"<p style='color: {status_color}; font-weight: bold;'>Статус: {status}</p>"
        html_out += f"<p>Найдено {csm_result.suspicious_count} подозрительных токенов из {csm_result.total_tokens}. "
        html_out += f"Макс. уверенность: {csm_result.confidence:.2%}, порог: {csm_result.threshold:.2f}</p>"

        html_out += "<pre style='background-color: #1e1e1e; color: #ccc; padding: 10px; white-space: pre-wrap; word-wrap: break-word;'>"

        last_idx = 0
        for (start, end), prob in zip(csm_result.offsets, csm_result.array):
            if start == end:
                continue

            html_out += html.escape(text[last_idx:start])
            
            chunk_raw = text[start:end]
            chunk_escaped = html.escape(chunk_raw)

            is_meaningful = len(chunk_raw.strip()) > 0 and chunk_raw.strip() not in ['{', '}', ';', '\n', '\r']

            if prob > csm_result.threshold and is_meaningful:
                color = "rgba(255, 85, 85, 0.4)" 
                html_out += f'<span style="background-color: {color}; border-radius: 2px;">{chunk_escaped}</span>'
            else:
                html_out += chunk_escaped
                
            last_idx = end

        html_out += html.escape(text[last_idx:]) + "</pre>"
        return html_out
    
    @staticmethod
    def __render_safe():
        return "<h2 style='color: #50fa7b;'>Файл безопасен</h2>"
    
    @staticmethod
    def render(text, csm_result: CodeSensorResult):
        if csm_result.verdict >= 1:
            status = "Подозрительно" if csm_result.verdict == 1 else "Опасно"
            status_color = "#ffb86c" if csm_result.verdict == 1 else "#ff5555"
            return HtmlVisualizer.__render_suspicious(text, csm_result, status, status_color)
        else:
            return HtmlVisualizer.__render_safe()