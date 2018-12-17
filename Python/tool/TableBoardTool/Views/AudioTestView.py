#-*- coding:utf-8 -*-

from PyQt4 import QtCore, QtGui
from UiAudioTestView import *

class AudioTestView(QtGui.QWidget):
    def __init__(self, parent=None):
        super(AudioTestView, self).__init__(parent)
        self.ui = Ui_AudioTestView()
        self.ui.setupUi(self)

        QtCore.QObject.connect(self.ui.pushButton, QtCore.SIGNAL("clicked()"), self.audioTestStart)
        QtCore.QObject.connect(self.ui.pushButton_2, QtCore.SIGNAL("clicked()"), self.downloadCodeStart)

    def audioTestStart(self):
        self.ui.label_2.setText(u"测试成功!")
        self.ui.pushButton.setDisabled(False)
        self.emit(QtCore.SIGNAL("AudioStartTest(PyQt_PyObject)"), "audio_test")
        pass

    def downloadCodeStart(self):
        self.ui.label_3.setText(u"下载成功!")
        self.ui.pushButton_2.setDisabled(False)
        self.emit(QtCore.SIGNAL("downloadCodeTest(PyQt_PyObject)"), "download_code_test")
        pass

if __name__=="__main__":
    import sys
    app = QtGui.QApplication(sys.argv)
    view = AudioTestView()
    view.show()
    sys.exit(app.exec_())