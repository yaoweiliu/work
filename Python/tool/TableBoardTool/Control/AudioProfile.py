#-*- coding:utf-8 -*-

from ProfileBase import *
from Views.DefaultView import *

class AudioProfile(ProfileBase):
    def __init__(self, name, view):
        super(AudioProfile, self).__init__(name, view)

        self.connect(view, QtCore.SIGNAL("AudioStartTest(PyQt_PyObject)"), self.audioTest)
        self.connect(view, QtCore.SIGNAL("downloadCodeTest(PyQt_PyObject)"), self.downloadTest)

    def audioTest(self):
        print "audio_start"
        logMessage("音频测试通过!")
        #self.view.ui.pushButton.setDisabled(False)
        pass

    def downloadTest(self):
        print "download_start"
        logMessage("目标代码下载成功!")
        #self.view.ui.pushButton_2.setDisabled(False)