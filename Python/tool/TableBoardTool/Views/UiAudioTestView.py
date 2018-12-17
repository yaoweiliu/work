# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'UiAudioTestView.ui'
#
# Created: Wed Dec 05 15:54:34 2018
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

class Ui_AudioTestView(object):
    def setupUi(self, AudioTestView):
        AudioTestView.setObjectName(_fromUtf8("AudioTestView"))
        AudioTestView.resize(1024, 768)
        self.label = QtGui.QLabel(AudioTestView)
        self.label.setGeometry(QtCore.QRect(412, 50, 200, 25))
        font = QtGui.QFont()
        font.setPointSize(20)
        self.label.setFont(font)
        self.label.setAlignment(QtCore.Qt.AlignCenter)
        self.label.setObjectName(_fromUtf8("label"))
        self.pushButton = QtGui.QPushButton(AudioTestView)
        self.pushButton.setGeometry(QtCore.QRect(220, 240, 180, 50))
        font = QtGui.QFont()
        font.setPointSize(20)
        self.pushButton.setFont(font)
        self.pushButton.setObjectName(_fromUtf8("pushButton"))
        self.label_2 = QtGui.QLabel(AudioTestView)
        self.label_2.setGeometry(QtCore.QRect(550, 240, 380, 50))
        font = QtGui.QFont()
        font.setPointSize(20)
        self.label_2.setFont(font)
        self.label_2.setAlignment(QtCore.Qt.AlignCenter)
        self.label_2.setObjectName(_fromUtf8("label_2"))
        self.pushButton_2 = QtGui.QPushButton(AudioTestView)
        self.pushButton_2.setGeometry(QtCore.QRect(220, 350, 180, 50))
        font = QtGui.QFont()
        font.setPointSize(20)
        self.pushButton_2.setFont(font)
        self.pushButton_2.setObjectName(_fromUtf8("pushButton_2"))
        self.label_3 = QtGui.QLabel(AudioTestView)
        self.label_3.setGeometry(QtCore.QRect(550, 350, 380, 50))
        font = QtGui.QFont()
        font.setPointSize(20)
        self.label_3.setFont(font)
        self.label_3.setAlignment(QtCore.Qt.AlignCenter)
        self.label_3.setObjectName(_fromUtf8("label_3"))

        self.retranslateUi(AudioTestView)
        QtCore.QMetaObject.connectSlotsByName(AudioTestView)

    def retranslateUi(self, AudioTestView):
        AudioTestView.setWindowTitle(_translate("AudioTestView", "Form", None))
        self.label.setText(_translate("AudioTestView", "音频测试", None))
        self.pushButton.setText(_translate("AudioTestView", "开始测试音频", None))
        self.label_2.setText(_translate("AudioTestView", "测试中/测试通过/测试失败", None))
        self.pushButton_2.setText(_translate("AudioTestView", "下载目标代码", None))
        self.label_3.setText(_translate("AudioTestView", "下载中/下载成功/下载失败", None))

