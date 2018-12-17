#from UiBoardTestInfo import Ui_Dialog
#from UiDefaultInfo import Ui_DefaultWindow
from UiDefaultEnterInfo import Ui_DefaultEnter
from UiTestResultInfo import Ui_TestResultInfo
from UiPXBoardTestView import Ui_PXBoardTestView
from UiPGBoardTestView import Ui_PGBoardTestView
from UiAudioTestView import Ui_AudioTestView
from PyQt4 import QtGui,QtCore

class TableBoardTest(QtGui.QMainWindow):
#class TableBoardTest(QtGui.QWidget):
    def __init__(self, parent=None):
        super(TableBoardTest, self).__init__(parent)
        #obj = QtGui.QTableView()
        #self.ui = Ui_Dialog()
        self.ui = Ui_PXBoardTestView()
        self.ui.setupUi(self)

if __name__ == '__main__':
    import sys
    app = QtGui.QApplication(sys.argv)
    #obj = QTableView()
    mainWindow = TableBoardTest()
    #mainWindow.setupUi(obj)
    mainWindow.show()

    sys.exit(app.exec_())