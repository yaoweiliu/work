# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'UiTestResultInfo.ui'
#
# Created: Wed Dec 05 15:55:08 2018
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

class Ui_TestResultInfo(object):
    def setupUi(self, TestResultInfo):
        TestResultInfo.setObjectName(_fromUtf8("TestResultInfo"))
        TestResultInfo.resize(1024, 768)
        self.textBrowser = QtGui.QTextBrowser(TestResultInfo)
        self.textBrowser.setGeometry(QtCore.QRect(360, 80, 641, 531))
        font = QtGui.QFont()
        font.setPointSize(14)
        self.textBrowser.setFont(font)
        self.textBrowser.setObjectName(_fromUtf8("textBrowser"))
        self.comboBox = QtGui.QComboBox(TestResultInfo)
        self.comboBox.setGeometry(QtCore.QRect(130, 300, 206, 31))
        font = QtGui.QFont()
        font.setPointSize(18)
        self.comboBox.setFont(font)
        self.comboBox.setObjectName(_fromUtf8("comboBox"))
        self.pushButton = QtGui.QPushButton(TestResultInfo)
        self.pushButton.setGeometry(QtCore.QRect(90, 380, 141, 35))
        font = QtGui.QFont()
        font.setPointSize(18)
        self.pushButton.setFont(font)
        self.pushButton.setObjectName(_fromUtf8("pushButton"))
        self.label = QtGui.QLabel(TestResultInfo)
        self.label.setGeometry(QtCore.QRect(20, 300, 111, 31))
        font = QtGui.QFont()
        font.setPointSize(18)
        self.label.setFont(font)
        self.label.setObjectName(_fromUtf8("label"))
        self.label_2 = QtGui.QLabel(TestResultInfo)
        self.label_2.setGeometry(QtCore.QRect(20, 220, 111, 31))
        font = QtGui.QFont()
        font.setPointSize(18)
        self.label_2.setFont(font)
        self.label_2.setObjectName(_fromUtf8("label_2"))
        self.comboBox_2 = QtGui.QComboBox(TestResultInfo)
        self.comboBox_2.setGeometry(QtCore.QRect(130, 220, 206, 31))
        font = QtGui.QFont()
        font.setPointSize(18)
        self.comboBox_2.setFont(font)
        self.comboBox_2.setObjectName(_fromUtf8("comboBox_2"))

        self.retranslateUi(TestResultInfo)
        QtCore.QMetaObject.connectSlotsByName(TestResultInfo)

    def retranslateUi(self, TestResultInfo):
        TestResultInfo.setWindowTitle(_translate("TestResultInfo", "Form", None))
        self.pushButton.setText(_translate("TestResultInfo", "确认", None))
        self.label.setText(_translate("TestResultInfo", "载入日志", None))
        self.label_2.setText(_translate("TestResultInfo", "序列号", None))

