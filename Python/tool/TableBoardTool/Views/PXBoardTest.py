#-*- coding:utf-8 -*-

from PyQt4 import QtGui, QtCore
from UiPXBoardTestView import *

class PXBoardTest(QtGui.QWidget):
    def __init__(self, parent=None):
        super(PXBoardTest, self).__init__(parent)
        self.ui = Ui_PXBoardTestView()
        self.ui.setupUi(self)

        #self.ui.pushButton.click(self.startTest)
        QtCore.QObject.connect(self.ui.pushButton, QtCore.SIGNAL("clicked()"), self.startTest)

    def startTest(self):
        #self.ui.label_2.setText(u"测试中")
        #self.ui.label_2.setText(u"测试通过")
        #self.ui.label_2.setText(u"测试失败")
        self.ui.textEdit.append(u"貔貅电路板测试中...")
        self.ui.pushButton.setDisabled(True)
        self.emit(QtCore.SIGNAL("PXBoardStartTest(PyQt_PyObject)"), "px_board_test")
        pass

if __name__=='__main__':
    import sys

    app = QtGui.QApplication(sys.argv)
    main = PXBoardTest()
    main.show()
    sys.exit(app.exec_())