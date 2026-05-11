"""
setup.py - Инициализация моделей и загрузка библиотек для AICodeSensor
Запустите перед первым использованием приложения:
    python setup.py
"""

import os
import sys
import torch
from transformers import AutoTokenizer, AutoModel


def check_requirements():
    """Проверка установленных зависимостей"""
    print("=" * 60)
    print("Проверка зависимостей...")
    print("=" * 60)
    
    try:
        import PySide6
        print("✓ PySide6 установлен")
    except ImportError:
        print("✗ PySide6 не установлен")
        print("  Установите: pip install PySide6")
        return False
    
    try:
        import torch
        print(f"✓ PyTorch установлен (версия {torch.__version__})")
    except ImportError:
        print("✗ PyTorch не установлен")
        print("  Установите: pip install torch")
        return False
    
    try:
        import transformers
        print(f"✓ Transformers установлен")
    except ImportError:
        print("✗ Transformers не установлен")
        print("  Установите: pip install transformers")
        return False
    
    return True


def download_codebert_tokenizer():
    """Загрузка CodeBERT токенайзера"""
    print("\n" + "=" * 60)
    print("Загрузка CodeBERT токенайзера...")
    print("=" * 60)
    
    try:
        tokenizer = AutoTokenizer.from_pretrained(
            "microsoft/codebert-base",
            use_fast=True,
            cache_dir="./models"
        )
        print("✓ CodeBERT токенайзер успешно загружен")
        return True
    except Exception as e:
        print(f"✗ Ошибка загрузки токенайзера: {e}")
        return False


def check_model_file():
    """Проверка наличия файла модели"""
    print("\n" + "=" * 60)
    print("Проверка файла модели...")
    print("=" * 60)
    
    model_path = "detector_v2_ep10.pth"
    
    if os.path.exists(model_path):
        size_mb = os.path.getsize(model_path) / (1024 * 1024)
        print(f"✓ Файл модели найден: {model_path}")
        print(f"  Размер: {size_mb:.2f} МБ")
        return True
    else:
        print(f"✗ Файл модели не найден: {model_path}")
        print("  Убедитесь что файл находится в том же директории")
        return False


def check_device():
    """Проверка доступности GPU"""
    print("\n" + "=" * 60)
    print("Проверка вычислительных устройств...")
    print("=" * 60)
    
    if torch.cuda.is_available():
        print(f"✓ GPU доступна: {torch.cuda.get_device_name(0)}")
        print(f"  CUDA версия: {torch.version.cuda}")
    else:
        print("ℹ GPU не доступна, будет использоваться CPU")
    
    return True


def main():
    """Основная функция инициализации"""
    print("\n")
    print("╔" + "=" * 58 + "╗")
    print("║" + " " * 10 + "AICodeSensor - Инициализация проекта" + " " * 11 + "║")
    print("╚" + "=" * 58 + "╝")
    
    all_ok = True
    
    # Проверка зависимостей
    if not check_requirements():
        all_ok = False
    
    # Загрузка токенайзера
    if not download_codebert_tokenizer():
        all_ok = False
    
    # Проверка модели
    if not check_model_file():
        all_ok = False
    
    # Проверка устройства
    check_device()
    
    # Итоги
    print("\n" + "=" * 60)
    if all_ok:
        print("✓ Инициализация завершена успешно!")
        print("  Теперь можно запустить приложение: python mainwindow.py")
    else:
        print("✗ Обнаружены проблемы. Пожалуйста исправьте их перед запуском.")
        sys.exit(1)
    print("=" * 60 + "\n")


if __name__ == "__main__":
    main()
