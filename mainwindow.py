# This Python file uses the following encoding: utf-8
import sys
import os
from PySide6.QtWidgets import (QApplication, QMainWindow, QFileDialog,
                             QListWidgetItem, QSplitter, QVBoxLayout, QWidget, QPushButton)
from PySide6.QtGui import QColor
from PySide6.QtCore import Qt

from model import CodeSensorModel_first, CodeSensorModel_second, visualize_long_ai_code
from model_loader import model, tokenizer, device
from ui_form import Ui_MainWindow

class MainWindow(QMainWindow):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)
        self.setWindowTitle("AICodeSensor")

        # 1. Настройка разделителя (Splitter)
        self.splitter = QSplitter(Qt.Horizontal)

        # Контейнер для левой части (список + кнопки)
        left_widget = QWidget()
        left_layout = QVBoxLayout(left_widget)
        left_layout.setContentsMargins(0, 0, 0, 0)
        left_layout.addWidget(self.ui.file_list)
        left_layout.addWidget(self.ui.btn_load_files)

        # Собираем сплиттер
        self.splitter.addWidget(left_widget)
        self.splitter.addWidget(self.ui.result_view)
        self.splitter.setSizes([250, 550])

        # Установка главного Layout
        main_layout = QVBoxLayout(self.ui.centralwidget)
        main_layout.addWidget(self.splitter)

        # 2. Стили (QSS)
        self.setStyleSheet("""
            QMainWindow { background-color: #2b2b2b; }
            QSplitter::handle { background-color: #323232; width: 4px; }
            QSplitter::handle:hover { background-color: #4e5052; }
            QListWidget { background-color: #3c3f41; border: none; color: #afb1b3; }
            QTextBrowser { background-color: #2b2b2b; border: none; color: #a9b7c6; }
            QPushButton { background-color: #365880; color: white; border-radius: 3px; padding: 8px; }
        """)

        # 3. Инициализация моделей и сигналов (БЕЗ ДУБЛЕЙ)
        self.sensor_first = CodeSensorModel_first()
        self.sensor_second = CodeSensorModel_second()
        self.results_storage = {}
        self.file_content_storage = {}

        self.ui.btn_load_files.clicked.connect(self.load_multiple_files)
        self.ui.file_list.itemClicked.connect(self.display_selected_file)

    def load_multiple_files(self):
        file_paths, _ = QFileDialog.getOpenFileNames(self, "Выбор файлов")
        if not file_paths:
            return

        for path in file_paths:
            file_name = os.path.basename(path)
            try:
                with open(path, 'r', encoding='utf-8') as f:
                    content = f.read()

                self.file_content_storage[file_name] = content
                
                # Получаем вердикт и результат анализа
                verdict = self.sensor_first.get_verdict(content)
                item = QListWidgetItem(file_name)

                if verdict == 1:
                    print(f'АНАЛИЗ: {file_name} подозрителен')
                    item.setForeground(QColor("red"))
                else:
                    print(f'АНАЛИЗ: {file_name} чист')
                    item.setForeground(QColor("green"))
                
                # Сохраняем результат анализа
                self.results_storage[file_name] = self.sensor_second.process_code(content)
                self.ui.file_list.addItem(item)

            except Exception as e:
                print(f"Ошибка с файлом {file_name}: {e}")

    def display_selected_file(self, item):
        file_name = item.text()
        html_to_show = self.results_storage.get(file_name, "Ошибка: данные не найдены")
        self.ui.result_view.setHtml(html_to_show)

    def visualize_selected_file(self):
        current_item = self.ui.file_list.currentItem()
        if not current_item:
            self.ui.result_view.setHtml("<h3 style='color: orange;'>Пожалуйста, выберите файл из списка</h3>")
            return
        
        file_name = current_item.text()
        content = self.file_content_storage.get(file_name)
        
        if not content:
            self.ui.result_view.setHtml("<h3 style='color: red;'>Ошибка: содержимое файла не найдено</h3>")
            return
        
        try:
            html_visualization = visualize_long_ai_code(content, model, tokenizer, device)
            self.ui.result_view.setHtml(html_visualization)
        except Exception as e:
            self.ui.result_view.setHtml(f"<h3 style='color: red;'>Ошибка визуализации: {e}</h3>")

if __name__ == "__main__":
    app = QApplication(sys.argv)
    widget = MainWindow()
    widget.show()
    sys.exit(app.exec())