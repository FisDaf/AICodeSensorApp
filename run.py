from MainWindow import *
from transformers import AutoTokenizer
import torch
import sys
from CodeBertDetector import CodeBertDetector
from CodeSensor import CodeSensor

WEIGHTS_PATHS = {
    "py": 'weights/py.pth',
    "cpp": 'weights/cpp.pth'
}

def load_tokenizer():
    print(f"[Model Loader] Используется устройство: {device}")
    print("[Model Loader] Загрузка CodeBERT токенайзера...")

    tokenizer = AutoTokenizer.from_pretrained("microsoft/codebert-base", use_fast=True)
    print("[Model Loader] ✓ Токенайзер загружен")

    return tokenizer

def load_model(model_path: str):
    print("[Model Loader] Загрузка модели детектора...")
    try:
        model = CodeBertDetector(unfreeze_layers=2)
        state_dict = torch.load(model_path, map_location=device, weights_only=True)
        model.load_state_dict(state_dict)
        model.to(device)
        model.eval()
        print("[Model Loader] ✓ Модель успешно загружена")
    except FileNotFoundError:
        print(f"[Model Loader] ✗ ОШИБКА: Файл {model_path} не найден!")
        print("[Model Loader] Убедитесь что файл находится в том же директории что и скрипт")
        model = None
    except Exception as e:
        print(f"[Model Loader] ✗ ОШИБКА при загрузке модели: {e}")
        model = None

    return model

def check_model_file(model_path):
    print("\n" + "=" * 60)
    print("Проверка файла модели...")
    print("=" * 60)
    
    if os.path.exists(model_path):
        size_mb = os.path.getsize(model_path) / (1024 * 1024)
        print(f"✓ Файл модели найден: {model_path}")
        print(f"  Размер: {size_mb:.2f} МБ")
        return
    else:
        print(f"✗ Файл модели не найден: {model_path}")
        print("  Убедитесь что файл находится в том же директории")
        sys.exit(1)

def check_device():
    """Проверка доступности GPU"""
    print("\n" + "=" * 60)
    print("Проверка вычислительных устройств...")
    print("=" * 60)
    
    if torch.cuda.is_available():
        print(f"✓ GPU доступна: {torch.cuda.get_device_name(0)}")
        print(f"  CUDA версия: {torch.version.cuda}")
    else:
        print("GPU не доступна, будет использоваться CPU")
    
    return True

if __name__ == "__main__":
    for lang, model_path in WEIGHTS_PATHS.items():
        print(f"\n[Model Loader] Проверка модели для языка: {lang}")
        check_model_file(model_path)

    check_device()

    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    tokenizer = load_tokenizer()

    sensorModels = {}
    for lang, model_path in WEIGHTS_PATHS.items():
        sensorModels[lang] = CodeSensor(tokenizer, load_model(model_path), device)

    app = QApplication(sys.argv)
    widget = MainWindow(sensorModels)
    widget.show()
    sys.exit(app.exec())