"""
model_loader.py - Загрузка и инициализация моделей
Этот модуль отвечает за загрузку CodeBERT модели и токенайзера
"""

import torch
from transformers import AutoTokenizer, AutoModel
import torch.nn as nn


# Инициализация устройства
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
print(f"[Model Loader] Используется устройство: {device}")

# Загрузка токенайзера
print("[Model Loader] Загрузка CodeBERT токенайзера...")
tokenizer = AutoTokenizer.from_pretrained("microsoft/codebert-base", use_fast=True)
print("[Model Loader] ✓ Токенайзер загружен")


class CodeBertDetector(nn.Module):
    """Детектор уязвимостей на основе CodeBERT"""
    def __init__(self, unfreeze_layers=2):
        super().__init__()
        self.bert = AutoModel.from_pretrained("microsoft/codebert-base")
        
        # Замораживаем большинство слоёв
        for p in self.bert.parameters():
            p.requires_grad = False
        
        # Размораживаем последние слои
        if unfreeze_layers > 0:
            for layer in self.bert.encoder.layer[-unfreeze_layers:]:
                for p in layer.parameters():
                    p.requires_grad = True

        # Классификатор
        self.classifier = nn.Sequential(
            nn.Linear(768, 256),
            nn.GELU(),
            nn.Dropout(0.5),
            nn.Linear(256, 128),
            nn.GELU(),
            nn.Dropout(0.3),
            nn.Linear(128, 64),
            nn.GELU(),
            nn.Dropout(0.2),
            nn.Linear(64, 2)
        )

    def forward(self, ids, mask):
        out = self.bert(ids, attention_mask=mask).last_hidden_state
        b, s, h = out.shape
        logits = self.classifier(out.view(-1, h))
        return logits.view(b, s, 2)


# Загрузка модели
print("[Model Loader] Загрузка модели детектора...")
try:
    model = CodeBertDetector(unfreeze_layers=2)
    state_dict = torch.load('detector_v2_ep10.pth', map_location=device, weights_only=True)
    model.load_state_dict(state_dict)
    model.to(device)
    model.eval()
    print("[Model Loader] ✓ Модель успешно загружена")
except FileNotFoundError:
    print("[Model Loader] ✗ ОШИБКА: Файл detector_v2_ep10.pth не найден!")
    print("[Model Loader] Убедитесь что файл находится в том же директории что и скрипт")
    model = None
except Exception as e:
    print(f"[Model Loader] ✗ ОШИБКА при загрузке модели: {e}")
    model = None


__all__ = ['model', 'tokenizer', 'device', 'CodeBertDetector']
