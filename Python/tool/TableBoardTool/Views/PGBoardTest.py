#-*- coding:utf-8 -*-

from PyQt4 import QtGui, QtCore
from UiPGBoardTestView import *

class PGBoardTest(QtGui.QWidget):
    def __init__(self, parent=None):
        super(PGBoardTest, self).__init__(parent)
        self.ui = Ui_PGBoardTestView()
        self.ui.setupUi(self)

        #self.ui.pushButton.click(self.startTest)
        QtCore.QObject.connect(self.ui.pushButton, QtCore.SIGNAL("clicked()"), self.startTest)

    def startTest(self):
        #self.ui.label_2.setText(u"测试中")
        #self.ui.label_2.setText(u"测试通过")
        #self.ui.label_2.setText(u"测试失败")
        self.ui.textEdit.append(u"盘古电路板测试中...")
        self.ui.pushButton.setDisabled(True)
        self.emit(QtCore.SIGNAL("PGBoardStartTest(PyQt_PyObject)"), "pg_board_test")
        pass