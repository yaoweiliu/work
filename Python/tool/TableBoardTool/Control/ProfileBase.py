#-*- coding:utf-8 -*-

from PyQt4 import QtGui, QtCore

class ProfileBase(QtCore.QObject):
    def __init__(self, name, view):
        super(ProfileBase, self).__init__()
        self.name = name
        self.view = view

    def getView(self):
        return self.view

    def whoAmI(self):
        return self.name