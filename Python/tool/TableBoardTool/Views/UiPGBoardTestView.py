# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'UiPGBoardTestView.ui'
#
# Created: Wed Dec 05 16:22:11 2018
#      by: PyQt4 UI code generator 4.11.2
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    def _fromUtf8(s):
        return s

try:
    _encoding = QtGui.QApplication.UnicodeUTF8
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig, _encoding)
except AttributeError:
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig)

class Ui_PGBoardTestView(object):
    def setupUi(self, PGBoardTestView):
        PGBoardTestView.setObjectName(_fromUtf8("PGBoardTestView"))
        PGBoardTestView.resize(1024, 768)
        self.label = QtGui.QLabel(PGBoardTestView)
        self.label.setGeometry(QtCore.QRect(412, 50, 200, 25))
        font = QtGui.QFont()
        font.setPointSize(20)
        self.label.setFont(font)
        self.label.setAlignment(QtCore.Qt.AlignCenter)
        self.label.setObjectName(_fromUtf8("label"))
        self.pushButton = QtGui.QPushButton(PGBoardTestView)
        self.pushButton.setGeometry(QtCore.QRect(220, 310, 150, 50))
        font = QtGui.QFont()
        font.setPointSize(20)
        self.pushButton.setFont(font)
        self.pushButton.setObjectName(_fromUtf8("pushButton"))
        self.textEdit = QtGui.QTextEdit(PGBoardTestView)
        self.textEdit.setGeometry(QtCore.QRect(453, 150, 501, 391))
        font = QtGui.QFont()
        font.setPointSize(16)
        self.textEdit.setFont(font)
        self.textEdit.setObjectName(_fromUtf8("textEdit"))

        self.retranslateUi(PGBoardTestView)
        QtCore.QMetaObject.connectSlotsByName(PGBoardTestView)

    def retranslateUi(self, PGBoardTestView):
        PGBoardTestView.setWindowTitle(_translate("PGBoardTestView", "Form", None))
        self.label.setText(_translate("PGBoardTestView", "盘古电路板测试", None))
        self.pushButton.setText(_translate("PGBoardTestView", "开始测试", None))

