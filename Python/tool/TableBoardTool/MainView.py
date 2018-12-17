#-*- coding:utf-8 -*-

import os
import sys
sys.path.append("C:\Users\del\PycharmProjects\TableBoardJig\Views")
from PyQt4 import QtGui,QtCore
from Views.PGBoardTest import *
from Views.PXBoardTest import *
from Views.AudioTestView import *
from Views.DefaultView import *
from Views.ResultView import *

class MainView(QtGui.QMainWindow):
    def __init__(self, parent=None):
        super(MainView, self).__init__(parent)
        self.setMinimumSize(1210, 768)  # 1366, 768

        self.setWindowTitle(u'1004112电路板工装软件 V0.0')
        self.setUnifiedTitleAndToolBarOnMac(True)

        self.createToolBar()
        # add subviews
        self.viewID = {}
        self.viewBox = QtGui.QStackedWidget()
        self.createSubViews()
        self.viewBox.setCurrentIndex(self.viewID['default_view'])
        self.setCentralWidget(self.viewBox)
        self.currentView = []
        self.bindHandlers()

    def bindHandlers(self):
        view = self.getSubView('default_view')
        self.connect(view, QtCore.SIGNAL("startSystem(PyQt_PyObject)"), self.startTest)

    def startTest(self):
        self.enableToolBar()

    def createAction(self, imageFile, text, cbFunction):
        font = QtGui.QFont(u"微软雅黑", 10, QtGui.QFont.Bold)

        action = QtGui.QAction(QtGui.QIcon(imageFile), text, self, shortcut=QtGui.QKeySequence.Undo, statusTip=text, triggered=cbFunction)
        action.setFont(font)
        self.commonToolBar.addAction(action)
        self.commonToolBar.addSeparator()

        return action

    def createToolBar(self):
        self.commonToolBar = QtGui.QToolBar("CommonTool", self)
        self.commonToolBar.setAllowedAreas(QtCore.Qt.LeftToolBarArea)
        self.addToolBar(QtCore.Qt.LeftToolBarArea, self.commonToolBar)

        self.createAction(":/pxboard.png", u"PX链路测试", self.actionPxTableBoardTestView)
        self.createAction(":/pgboard.png", u"PG链路测试", self.actionPgTableBoardTestView)
        self.createAction(":/audio.png", u"音频测试", self.actionAudioTestView)
        self.createAction(":/result.png", u"结果浏览", self.actionTestResultView)
        self.createAction(":/shutdown.png", u"关闭系统", self.actionShutdown)

        self.setIconSize(QtCore.QSize(48, 48))
        self.setToolButtonStyle(QtCore.Qt.ToolButtonTextUnderIcon)

    def actionTestResultView(self):
        self.emit(QtCore.SIGNAL("viewChanged(PyQt_PyObject)"), "result_view")

    def actionPxTableBoardTestView(self):
        self.emit(QtCore.SIGNAL("viewChanged(PyQt_PyObject)"), "PX_view")

    def actionPgTableBoardTestView(self):
        self.emit(QtCore.SIGNAL("viewChanged(PyQt_PyObject)"), "PG_view")

    def actionAudioTestView(self):
        self.emit(QtCore.SIGNAL("viewChanged(PyQt_PyObject)"), "audio_view")

    def actionShutdown(self):
        self.emit(QtCore.SIGNAL("exitAppl(PyQt_PyObject)"), "exit_app")
        #self.close()

    def createSubViews(self):
        # create default view
        self.defaultView = DefaultView()
        id = self.viewBox.addWidget(self.defaultView)
        self.viewID['default_view'] = id

        # creare PX view
        view = PXBoardTest()
        id = self.viewBox.addWidget(view)
        self.viewID['PX_view'] = id

        # create PG view
        view = PGBoardTest()
        id = self.viewBox.addWidget(view)
        self.viewID['PG_view'] = id

        # create result view
        view = TestResultView()
        id = self.viewBox.addWidget(view)
        self.viewID['result_view'] = id

        # create audio view
        view = AudioTestView()
        id = self.viewBox.addWidget(view)
        self.viewID['audio_view'] = id

    def getSubView(self, name):
        if not self.viewID.has_key(name):
            return None
        return self.viewBox.widget(self.viewID[name])

    def activeView(self, name):
        if not self.viewID.has_key(name):
            return
        if self.currentView is None:
            return
        self.viewBox.setCurrentIndex(self.viewID[name])

    def enableToolBar(self):
        self.commonToolBar.setEnabled(True)

    def disableToolBar(self):
        self.commonToolBar.setDisabled(True)

if __name__ == '__main__':
    app = QtGui.QApplication(sys.argv)
    main = MainView()
    main.show()
    sys.exit(app.exec_())