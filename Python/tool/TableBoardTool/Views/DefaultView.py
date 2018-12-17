#-*- coding:utf-8 -*-

import sys
import os
import time
from UiDefaultEnterInfo import *
from PyQt4.QtCore import *
from PyQt4.QtGui import *
from Control.MessageLog import *

class DefaultView(QtGui.QWidget):
    def __init__(self, parent=None):
        super(DefaultView, self).__init__(parent)
        self.ui = Ui_DefaultEnter()
        self.ui.setupUi(self)
        #regExp = QRegExp('[A-Za-z][1-9][0-9]{0,2}')
        regExp = QRegExp('[a-zA-Z0-9]{3,20}')
        self.ui.lineEdit_3.setValidator(QRegExpValidator(regExp, self))

        #self.ui.pushButton.setDisabled(True)
        self.signalHandler()

    def signalHandler(self):
        QtCore.QObject.connect(self.ui.lineEdit, QtCore.SIGNAL("textEdited(QString)"), self.userNameInfo)
        QtCore.QObject.connect(self.ui.lineEdit_2, QtCore.SIGNAL("textEdited(QString)"), self.testTimeInfo)
        QtCore.QObject.connect(self.ui.lineEdit_3, QtCore.SIGNAL("textEdited(QString)"), self.boardSerialInfo)
        QtCore.QObject.connect(self.ui.pushButton, QtCore.SIGNAL("clicked()"), self.startSystem)

    def userNameInfo(self, info):
        str = unicode(info.toUtf8(), 'utf-8', 'ignore')
        """
        if self.ui.lineEdit.text()==None and self.ui.lineEdit_2.text()==None \
                and self.ui.lineEdit_3.text()==None:
            self.ui.pushButton.setDisabled(True)
        else:
            self.ui.pushButton.setDisabled(False)
        """

    def testTimeInfo(self, info):
        str = unicode(info.toUtf8(), 'utf-8', 'ignore')

    def boardSerialInfo(self, info):
        str = unicode(info.toUtf8(), 'utf-8', 'ignore')
        #self.ui.pushButton.setDisabled(False)

    def startSystem(self):
        self.emit(QtCore.SIGNAL("startSystem(PyQt_PyObject)"), "start_system")
        self.ui.pushButton.setDisabled(True)
        # write bin file
        filePath = "D:\\BoardJigLog\\" + str(self.ui.lineEdit_3.text().toUtf8())
        isExists = os.path.exists(filePath)
        if not isExists:
            os.makedirs(filePath)

        ISOTIMEFORMAT = "%Y-%m-%d-%H-%M-%S"
        #ISOTIMEFORMAT = "%Y%m%d%H%M%S"
        self.fileName = filePath + "\\" + str(time.strftime(ISOTIMEFORMAT, time.localtime(time.time()))) + ".log"
        msgLogger.init(str(self.ui.lineEdit.text().toUtf8()), str(self.ui.lineEdit_2.text().toUtf8()), \
                       str(self.ui.lineEdit_3.text().toUtf8()), self.fileName)

if __name__=='__main__':
    import sys

    app = QtGui.QApplication(sys.argv)
    main = DefaultView()
    main.show()
    sys.exit(app.exec_())