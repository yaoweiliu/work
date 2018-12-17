#-*- coding:utf-8 -*-

import os
from PyQt4 import QtGui, QtCore
from UiTestResultInfo import *

class TestResultView(QtGui.QWidget):
    def __init__(self, parent=None):
        super(TestResultView, self).__init__(parent)
        self.ui = Ui_TestResultInfo()
        self.ui.setupUi(self)

        QtCore.QObject.connect(self.ui.pushButton, QtCore.SIGNAL("clicked()"), self.displayResult)
        QtCore.QObject.connect(self.ui.comboBox, QtCore.SIGNAL("currentIndexChanged(int)"), self.onLogItemClicked)
        QtCore.QObject.connect(self.ui.comboBox_2, QtCore.SIGNAL("currentIndexChanged(int)"), self.onSiItemClicked)

        self.updateSerialItem()

    def displayResult(self):
        self.loadFile("D:\\BoardJigLog\\" + self.si + "\\" + self.log)

    def loadFile(self, fileName):
        self.ui.textBrowser.setText(u'载入日志: ' + self.si + "\\" + self.log)
        with open(fileName, "r") as fd:
            info = fd.read()
            self.ui.textBrowser.append(unicode(info, "utf-8", "ignore"))

    #def serialItemClicked(self):
        #self.updateSerialItem()

    def onSiItemClicked(self, idx):
        self.si = self.serial[idx]
        self.updateFileItems()

    def getSerialItems(self):
        filePath = "D:\\BoardJigLog\\"
        isExists = os.path.exists(filePath)
        if not isExists:
            os.makedirs(filePath)
        for dirpath, dirnames, filenames in os.walk("D:\\BoardJigLog\\"):
            self.serials = dirnames
            return self.serials

    def updateSerialItem(self):
        self.serial = []
        for i in self.getSerialItems():
            self.serial.append(i)

        model = self.ui.comboBox_2.model()
        model.clear()
        for i in self.getSerialItems():
            item = QtGui.QStandardItem()
            item.setText(i)
            model.appendRow(item)

    def getFiles(self):
        self.path = "D:\\BoardJigLog\\" + self.si
        for dirpath, dirnames, filenames in os.walk(self.path):
            self.files = filenames
            return filenames

    def updateFileItems(self):
        self.logs = []
        for i in self.getFiles():
            if i[-3:] == 'log':
                self.logs.append(i)

        model = self.ui.comboBox.model()
        model.clear()
        for i in self.logs:
            item = QtGui.QStandardItem()
            item.setText(i)
            model.appendRow(item)

    def onLogItemClicked(self, idx):
        self.log = self.logs[idx]