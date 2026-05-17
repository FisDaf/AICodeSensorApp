import os
from PySide6.QtWidgets import (QApplication, QMainWindow, QFileDialog, QListWidgetItem, QSplitter, QVBoxLayout, QWidget)
from PySide6.QtGui import QColor, QIcon
from PySide6.QtCore import Qt

from ui_form import Ui_MainWindow
from HtmlVisualizer import HtmlVisualizer

class MainWindow(QMainWindow):
    def __init__(self, sensorModels, parent=None):
        super().__init__(parent)
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)
        self.setWindowTitle("AICodeSensor")
        basedir = os.path.dirname(__file__)
        icon_path = os.path.join(basedir, "AICodeSensorLogo.png")
        self.setWindowIcon(QIcon(icon_path))

        self.splitter = QSplitter(Qt.Orientation.Horizontal)
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


        self.setStyleSheet("""
            QMainWindow { background-color: #2b2b2b; }
            QSplitter::handle { background-color: #323232; width: 4px; }
            QListWidget { background-color: #3c3f41; border: none; color: #afb1b3; outline: none; }
            QListWidget::item { padding: 8px; border-bottom: 1px solid #323232; }
            QTextBrowser { background-color: #2b2b2b; border: none; color: #a9b7c6; }
            QPushButton { background-color: #365880; color: white; border-radius: 3px; padding: 10px; font-weight: bold; }
            QPushButton:hover { background-color: #4572a7; }
        """)

        self.sensors = sensorModels
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

                lang = path.split('.')[-1].lower()
                result = self.sensors[lang].analyze(content)
                html_result = HtmlVisualizer.render(content, result)
                
                item = QListWidgetItem()
                item.setData(Qt.ItemDataRole.UserRole, file_name) 

                if result.verdict == 2:
                    item.setText(f"⚠ {file_name}")
                    item.setForeground(QColor("#ff5555"))
                elif result.verdict == 1:
                    item.setText(f"? {file_name}")
                    item.setForeground(QColor("#ffb86c"))
                else:
                    item.setText(f"✓ {file_name}")
                    item.setForeground(QColor("#50fa7b"))
                
                self.results_storage[file_name] = html_result
                self.ui.file_list.addItem(item)

            except Exception as e:
                print(f"Ошибка с файлом {file_name}: {e}")


    def display_selected_file(self, item):
        file_name = item.data(Qt.ItemDataRole.UserRole)
        html_to_show = self.results_storage.get(file_name, "Ошибка: данные не найдены")
        self.ui.result_view.setHtml(html_to_show)