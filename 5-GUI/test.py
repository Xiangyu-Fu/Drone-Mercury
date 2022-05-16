#!/usr/bin/python3
# -*- coding: utf-8 -*-

"""
ZetCode PyQt5 tutorial 

This example shows a tooltip on
a window and a button.

Author: Jan Bodnar
Website: zetcode.com
Last edited: August 2017
"""

import sys
from PyQt5.QtWidgets import (QWidget, QToolTip,
                             QPushButton, QApplication, QMessageBox, QDesktopWidget)
from PyQt5.QtCore import QCoreApplication
from PyQt5.QtGui import QFont


#
class Example(QWidget):

    def __init__(self):
        super().__init__()
        # 使用initUI()方法创建一个GUI
        self.initUI()


    def initUI(self):
        # 设置了提示框的字体
        QToolTip.setFont(QFont('SansSerif', 10))

        # 调用setToolTip（）创建提示框可以使用富文本格式的内容
        self.setToolTip('This is a <b>QWidget</b> widget')

        # 创建一个按钮，并为按钮创建了一个提示框
        btn = QPushButton('Button', self)
        btn.setToolTip('This is a <b>QPushButton</b> widget')
        # 调整按钮大小，并且为按钮添加了一个提示框
        btn.resize(btn.sizeHint())
        btn.move(50, 50)


        # 创建一个退出按钮
        qbtn = QPushButton('Quit', self)  # 第二个参数为按钮的父级组件
        qbtn.clicked.connect(QCoreApplication.instance().quit)
        qbtn.setToolTip('This is a quit button!')
        # 调整按钮大小，并且为按钮添加了一个提示框
        qbtn.resize(btn.sizeHint())
        qbtn.move(150, 50)

        self.resize(300, 200)  # x, y, wide, high
        self.center()
        self.setWindowTitle('Tooltips')  # title
        self.show()  # 令控件在桌面上显示出来，在内存里创建控件

    def center(self):

        qr = self.frameGeometry()
        cp = QDesktopWidget().availableGeometry().center()
        qr.moveCenter(cp)
        self.move(qr.topLeft())


    def closeEvent(self, event):
        reply = QMessageBox.question(self, 'Message',
                                     "Are you sure to quit?", QMessageBox.Yes |
                                     QMessageBox.No, QMessageBox.No)

        if reply == QMessageBox.Yes:
            event.accept()
        else:
            event.ignore()


if __name__ == '__main__':

    app = QApplication(sys.argv)  # 创建应用对象，sys.argv是一组命令行参数的列表
    ex = Example()  # 调用类
    sys.exit(app.exec_())  # 保证主循环安全退出，exec_防止与python冲突
