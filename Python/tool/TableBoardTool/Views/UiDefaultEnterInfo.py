# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'UiDefaultEnterInfo.ui'
#
# Created: Wed Dec 05 15:54:46 2018
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

class Ui_DefaultEnter(object):
    def setupUi(self, DefaultEnter):
        DefaultEnter.setObjectName(_fromUtf8("DefaultEnter"))
        DefaultEnter.resize(1024, 768)
        self.label = QtGui.QLabel(DefaultEnter)
        self.label.setGeometry(QtCore.QRect(310, 70, 401, 51))
        font = QtGui.QFont()
        font.setPointSize(20)
        self.label.setFont(font)
        self.label.setAlignment(QtCore.Qt.AlignCenter)
        self.label.setObjectName(_fromUtf8("label"))
        self.label_2 = QtGui.QLabel(DefaultEnter)
        self.label_2.setGeometry(QtCore.QRect(590, 130, 271, 21))
        font = QtGui.QFont()
        font.setPointSize(18)
        self.label_2.setFont(font)
        self.label_2.setAlignment(QtCore.Qt.AlignCenter)
        self.label_2.setObjectName(_fromUtf8("label_2"))
        self.label_3 = QtGui.QLabel(DefaultEnter)
        self.label_3.setGeometry(QtCore.QRect(720, 170, 141, 21))
        font = QtGui.QFont()
        font.setPointSize(18)
        self.label_3.setFont(font)
        self.label_3.setAlignment(QtCore.Qt.AlignCenter)
        self.label_3.setObjectName(_fromUtf8("label_3"))
        self.label_4 = QtGui.QLabel(DefaultEnter)
        self.label_4.setGeometry(QtCore.QRect(370, 0, 291, 71))
        self.label_4.setText(_fromUtf8(""))
        self.label_4.setPixmap(QtGui.QPixmap(_fromUtf8("D:/PXDOC/logo-zh.jpg")))
        self.label_4.setAlignment(QtCore.Qt.AlignCenter)
        self.label_4.setObjectName(_fromUtf8("label_4"))
        self.label_5 = QtGui.QLabel(DefaultEnter)
        self.label_5.setGeometry(QtCore.QRect(300, 250, 200, 25))
        font = QtGui.QFont()
        font.setPointSize(18)
        self.label_5.setFont(font)
        self.label_5.setLocale(QtCore.QLocale(QtCore.QLocale.Chinese, QtCore.QLocale.China))
        self.label_5.setObjectName(_fromUtf8("label_5"))
        self.label_6 = QtGui.QLabel(DefaultEnter)
        self.label_6.setGeometry(QtCore.QRect(300, 300, 200, 25))
        font = QtGui.QFont()
        font.setPointSize(18)
        self.label_6.setFont(font)
        self.label_6.setLocale(QtCore.QLocale(QtCore.QLocale.Chinese, QtCore.QLocale.China))
        self.label_6.setObjectName(_fromUtf8("label_6"))
        self.label_7 = QtGui.QLabel(DefaultEnter)
        self.label_7.setGeometry(QtCore.QRect(300, 350, 200, 25))
        font = QtGui.QFont()
        font.setPointSize(18)
        self.label_7.setFont(font)
        self.label_7.setLocale(QtCore.QLocale(QtCore.QLocale.Chinese, QtCore.QLocale.China))
        self.label_7.setObjectName(_fromUtf8("label_7"))
        self.lineEdit = QtGui.QLineEdit(DefaultEnter)
        self.lineEdit.setGeometry(QtCore.QRect(540, 250, 200, 25))
        font = QtGui.QFont()
        font.setPointSize(18)
        self.lineEdit.setFont(font)
        self.lineEdit.setLocale(QtCore.QLocale(QtCore.QLocale.Chinese, QtCore.QLocale.China))
        self.lineEdit.setObjectName(_fromUtf8("lineEdit"))
        self.lineEdit_2 = QtGui.QLineEdit(DefaultEnter)
        self.lineEdit_2.setGeometry(QtCore.QRect(540, 300, 200, 25))
        font = QtGui.QFont()
        font.setPointSize(18)
        self.lineEdit_2.setFont(font)
        self.lineEdit_2.setLocale(QtCore.QLocale(QtCore.QLocale.Chinese, QtCore.QLocale.China))
        self.lineEdit_2.setObjectName(_fromUtf8("lineEdit_2"))
        self.lineEdit_3 = QtGui.QLineEdit(DefaultEnter)
        self.lineEdit_3.setGeometry(QtCore.QRect(540, 350, 200, 25))
        font = QtGui.QFont()
        font.setPointSize(18)
        self.lineEdit_3.setFont(font)
        self.lineEdit_3.setLocale(QtCore.QLocale(QtCore.QLocale.Chinese, QtCore.QLocale.China))
        self.lineEdit_3.setObjectName(_fromUtf8("lineEdit_3"))
        self.label_8 = QtGui.QLabel(DefaultEnter)
        self.label_8.setGeometry(QtCore.QRect(240, 510, 620, 30))
        font = QtGui.QFont()
        font.setPointSize(18)
        self.label_8.setFont(font)
        self.label_8.setObjectName(_fromUtf8("label_8"))
        self.pushButton = QtGui.QPushButton(DefaultEnter)
        self.pushButton.setGeometry(QtCore.QRect(437, 420, 150, 50))
        font = QtGui.QFont()
        font.setPointSize(20)
        self.pushButton.setFont(font)
        self.pushButton.setObjectName(_fromUtf8("pushButton"))

        self.retranslateUi(DefaultEnter)
        QtCore.QMetaObject.connectSlotsByName(DefaultEnter)

    def retranslateUi(self, DefaultEnter):
        DefaultEnter.setWindowTitle(_translate("DefaultEnter", "Form", None))
        self.label.setText(_translate("DefaultEnter", "赛诺威盛科技（北京）有限公司", None))
        self.label_2.setText(_translate("DefaultEnter", "1004112电路板测试工装", None))
        self.label_3.setText(_translate("DefaultEnter", "版本v0.0", None))
        self.label_5.setText(_translate("DefaultEnter", "测试人：", None))
        self.label_6.setText(_translate("DefaultEnter", "测试时间：", None))
        self.label_7.setText(_translate("DefaultEnter", "电路板序列号：", None))
        self.label_8.setText(_translate("DefaultEnter", "注：此工装用于对1004112电路板进行制板加工出厂检验", None))
        self.pushButton.setText(_translate("DefaultEnter", "开始", None))

