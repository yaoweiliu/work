#-*- coding:utf-8 -*-

from PyQt4.QtCore import *
from PyQt4.QtGui import *
import sys

class warningBox(QDialog):
    def __init__(self,str_title,str_text,list_bool):#####自己写一个warningbox
        super(warningBox,self).__init__(parent = None)
        self.return_value = list_bool
        self.setWindowTitle(str_title)
        self.mainlayout = QGridLayout(self)
        self.labelText = QLabel()
        self.setFont(QFont("Roman times",16))#####字体设置
        self.mainlayout.addWidget(self.labelText,0,0,1,10)
        self.labelText.setText(str_text)
        self.resize(400,100)
        self.buttonSure = QPushButton()
        self.buttonSure.setText(u"确定" )
        self.buttonCancel = QPushButton()
        self.buttonCancel.setText(u"取消" )
        self.mainlayout.addWidget(self.buttonSure,1,2,1,2)
        self.mainlayout.addWidget(self.buttonCancel,1,6,1,2)
        self.setLayout(self.mainlayout)
        self.buttonSure.clicked.connect(self.sureOpra)
        self.buttonCancel.clicked.connect(self.cancelOpra)
        self.show()

    def sureOpra(self):
        self.close()
        self.return_value.append(1)

    def cancelOpra(self):
        self.close()
        self.return_value.append(0)

class MyWindow(QDialog,QWidget):
    def __init__(self,parent = None):
        super(MyWindow,self).__init__(parent)
        self.resize(400,400)
        QMessageBox.warning(None, u"注意", QString(u"数据库错误: %1").arg(u"连接服务器失败"))
        self.mainlayout = QGridLayout(self)
        self.myButton = QPushButton()
        self.myButton.setText(u"点击弹出警告子窗口")
        self.myButton.clicked.connect(self.newWindow)
        self.mainlayout.addWidget(self.myButton)

    def newWindow(self):
        list_run = []
        dilogUi = warningBox(u"警告示例",u"你被警告了！",list_run)
        if dilogUi.exec_():
            return
        print list_run

app=QApplication(sys.argv)
window=MyWindow()
window.show()
app.exec_()
