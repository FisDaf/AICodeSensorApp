import html
import random

class CodeSensorModel_first:
    def __init__(self):
        pass

    def get_verdict(self, code_text: str) -> int:
        return random.randint(0, 1)

class CodeSensorModel_second:
    def __init__(self):
        self.version = "1.0.0-alpha"

    def process_code(self, code_text: str) -> str:
        escaped_code = html.escape(code_text)

        keywords = ['def ', 'class ', 'import ', 'from ', 'return ', 'if ', 'for ', 'while ']
        highlighted_code = escaped_code
        for kw in keywords:
            highlighted_code = highlighted_code.replace(
                kw, f'<span style="color: #569cd6; font-weight: bold;">{kw}</span>'
            )
        return f"""
        <div style="background-color: #1e1e1e; color: #d4d4d4; font-family: monospace; padding: 15px;">
            <pre style="white-space: pre-wrap; margin: 0;">{highlighted_code}</pre>
        </div>
        """