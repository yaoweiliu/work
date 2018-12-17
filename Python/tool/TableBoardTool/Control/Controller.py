#-*- coding:utf-8 -*-

from PyQt4 import QtCore, QtGui
from MainView import *
from PXBoardProfile import *
from PGBoardProfile import *
from AudioProfile import *
from ResultProfile import *

class Controller(QtGui.QApplication):
    def __init__(self, list_of_str, *__args):
        super(Controller, self).__init__(list_of_str, *__args)
        self.mainView = MainView()
        self.profiles = {}

        view = self.mainView.getSubView('result_view')
        if view is not None:
            profile = ResultProfile("result_view", view)
            self.profiles[profile.whoAmI()] = profile
            self.mainView.currentView.append(profile)

        view = self.mainView.getSubView('PX_view')
        if view is not None:
            profile = PXBoardProfile("PX_view", view)
            self.profiles[profile.whoAmI()] = profile
            self.mainView.currentView.append(profile)

        view = self.mainView.getSubView('PG_view')
        if view is not None:
            profile = PGBoardProfile("PG_view", view)
            self.profiles[profile.whoAmI()] = profile
            self.mainView.currentView.append(profile)

        view = self.mainView.getSubView('audio_view')
        if view is not None:
            profile = AudioProfile("audio_view", view)
            self.profiles[profile.whoAmI()] = profile
            self.mainView.currentView.append(profile)

        self.bindHandlers()

    def bindHandlers(self):
        self.connect(self.mainView, QtCore.SIGNAL("exitAppl(PyQt_PyObject)"), self.shutDown)
        self.connect(self.mainView, QtCore.SIGNAL("viewChanged(PyQt_PyObject)"), self.notifyViewChanged)

    def shutDown(self):
        self.mainView.close()
        self.exit(0)

    def notifyViewChanged(self, name):
        if self.profiles.has_key(name):
            self.mainView.activeView(name)

    def run(self):
        self.mainView.disableToolBar()
        self.mainView.show()
        sys.exit(self.exec_())

app = Controller(sys.argv)
app.run()