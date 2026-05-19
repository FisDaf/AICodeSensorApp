import torch
import numpy as np


def autoMeanFilter(array : np.ndarray):
    k_size = len(array) // 10 + (1 - (len(array) // 10) % 2) 
    kernel = [1/k_size] * k_size
    l = len(kernel) // 2
    extra = [0] * l
    ext_array = np.concatenate([extra, array, extra])
    smoothed = []

    for i in range(l, len(ext_array) - l):
        span = ext_array[i  - l : i + l + 1]
        smoothed.append(np.dot(span, kernel))

    return np.array(smoothed)


class CodeSensorResult:
    def __init__(self, array, offsets, verdict, confidence, total_tokens, suspicious_count, threshold):
        self.array = array
        self.offsets = offsets
        self.verdict = verdict
        self.confidence = confidence
        self.total_tokens = total_tokens
        self.suspicious_count = suspicious_count
        self.threshold = threshold


class CodeSensor:
    def __init__(self, tokenizer, model, device):
        self.tokenizer = tokenizer
        self.model = model
        self.device = device


    def __predict(self, text, window_size=512, stride=256):
        self.model.eval()
        
        inputs = self.tokenizer(
            text, 
            return_tensors="pt", 
            truncation=False, 
            return_offsets_mapping=True
        )
        
        all_input_ids = inputs["input_ids"][0]
        all_offsets = inputs["offset_mapping"][0].tolist()
        seq_len = all_input_ids.size(0)
        
        token_array_sum = np.zeros(seq_len)
        token_counts = np.zeros(seq_len)
        
        for start_idx in range(0, seq_len, stride):
            end_idx = min(start_idx + window_size, seq_len)
            
            window_ids = all_input_ids[start_idx:end_idx].unsqueeze(0).to(self.device)
            window_mask = torch.ones_like(window_ids).to(self.device)
            
            with torch.no_grad():
                logits = self.model(window_ids, window_mask)
                array = torch.softmax(logits, dim=-1)[0, :, 1].cpu().numpy()
                
            token_array_sum[start_idx:end_idx] += array
            token_counts[start_idx:end_idx] += 1
            
            if end_idx == seq_len:
                break
                
        final_array = token_array_sum / token_counts
        tokens = self.tokenizer.convert_ids_to_tokens(all_input_ids)
        
        return tokens, final_array, all_offsets


    def analyze(self, code_text: str):
        _, array, offsets = self.__predict(code_text)
        
        lines_count = len(code_text.split('\n'))
        chars_count = len(code_text)
        
        if lines_count < 25 or chars_count < 200:
            threshold = 0.8
        else:
            threshold = 0.75
            
        array = autoMeanFilter(array)
        
        meaningful_array = []
        for (start, end), prob in zip(offsets, array):
            if start == end:
                continue
                
            chunk_raw = code_text[start:end]
            if len(chunk_raw.strip()) > 0 and chunk_raw.strip() not in ['{', '}', ';', '\n', '\r']:
                meaningful_array.append(prob)
        
        confidence = float(max(meaningful_array)) if len(meaningful_array) > 0 else 0.0
        total_tokens = len(meaningful_array)
        suspicious_count = sum(1 for p in meaningful_array if p > threshold)
        density = suspicious_count / total_tokens if total_tokens > 0 else 0
        
        if density > 0.3:
            verdict = 2
        elif density > 0.15:
            verdict = 1
        else:
            verdict = 0
            
        return CodeSensorResult(array, offsets, verdict, confidence, total_tokens, suspicious_count, threshold)