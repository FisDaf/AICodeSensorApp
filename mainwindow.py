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

        # 1. Настройка разделителя
        self.splitter = QSplitter(Qt.Horizontal)
        left_widget = QWidget()
        left_layout = QVBoxLayout(left_widget)
        left_layout.setContentsMargins(0, 0, 0, 0)
        
        left_layout.addWidget(self.ui.file_list)
        left_layout.addWidget(self.ui.btn_load_files)

        self.splitter.addWidget(left_widget)
        self.splitter.addWidget(self.ui.result_view)
        self.splitter.setSizes([250, 550])

        main_layout = QVBoxLayout(self.ui.centralwidget)
        main_layout.addWidget(self.splitter)

        # 2. Обновленные стили (добавили отступы для списка)
        self.setStyleSheet("""
            QMainWindow { background-color: #2b2b2b; }
            QSplitter::handle { background-color: #323232; width: 4px; }
            QListWidget { background-color: #3c3f41; border: none; color: #afb1b3; outline: none; }
            QListWidget::item { padding: 8px; border-bottom: 1px solid #323232; }
            QTextBrowser { background-color: #2b2b2b; border: none; color: #a9b7c6; }
            QPushButton { background-color: #365880; color: white; border-radius: 3px; padding: 10px; font-weight: bold; }
            QPushButton:hover { background-color: #4572a7; }
        """)

        self.sensor_first = CodeSensorModel_first()
        self.sensor_second = CodeSensorModel_second()
        self.results_storage = {}

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

                # verdict теперь возвращает 0, 1 или 2
                verdict = self.sensor_first.get_verdict(content)
                
                item = QListWidgetItem()
                # Сохраняем чистое имя файла в скрытые данные элемента, чтобы поиск в словаре не ломался
                item.setData(Qt.UserRole, file_name) 

                if verdict == 2:
                    item.setText(f"⚠ {file_name}")
                    item.setForeground(QColor("#ff5555"))
                elif verdict == 1:
                    item.setText(f"? {file_name}")
                    item.setForeground(QColor("#ffb86c"))
                else:
                    item.setText(f"✓ {file_name}")
                    item.setForeground(QColor("#50fa7b"))
                
                # Сохраняем результат анализа
                self.results_storage[file_name] = self.sensor_second.process_code(content)
                self.ui.file_list.addItem(item)

            except Exception as e:
                print(f"Ошибка с файлом {file_name}: {e}")

    def display_selected_file(self, item):
        file_name = item.data(Qt.UserRole)
        html_to_show = self.results_storage.get(file_name, "Ошибка: данные не найдены")
        self.ui.result_view.setHtml(html_to_show)

if __name__ == "__main__":
    app = QApplication(sys.argv)
    widget = MainWindow()
    widget.show()
    sys.exit(app.exec())