#-*- coding:utf-8 -*-

from ProfileBase import *
#from MessageLog import *
from Views.DefaultView import *
from SerialOperations import *
import threading
from bitarray import bitarray
from collections import defaultdict, OrderedDict

class PGBoardProfile(ProfileBase):
    def __init__(self, name, view):
        super(PGBoardProfile, self).__init__(name, view)

        self.connect(view, QtCore.SIGNAL("PGBoardStartTest(PyQt_PyObject)"), self.test)

        #self.serial = SerialOperations()

    def test(self):
        #header, packlen, cmd, data, checksum [0x55,0x05,0xB0,0x00,0xB0]
        #serialOpera.sendData([chr(0x55)], [chr(0x05)], [chr(0xB0)], [chr(0x00)], [chr(0xB0)])
        if not serialOpera.ser.isOpen():
            serialOpera.ser.open()
        serialOpera.sendData([0x55, 0x05, 0xB0, 0x00, 0xB0])

        self.timer1 = threading.Timer(2, self.checkLink)
        self.timer1.start()

        #thread_checkdata = threading.Thread(target=self.checkData, name="check_data")
        #thread_checkdata.start()

    def checkLink(self):
        link = serialOpera.checkLink()  # first step, check link status
        if link == "pg_link_is_ok":
            logMessage("盘古电路板通讯链路正常!")
            self.view.ui.textEdit.append(u"盘古电路板通讯链路正常!")
            self.timer2 = threading.Timer(8, self.checkData)
            self.timer2.start()
        elif link == "cmd_error":
            logMessage("指令错误!")
            self.view.ui.textEdit.append(u"指令错误!")
            self.view.ui.pushButton.setDisabled(False)
        else:
            logMessage("盘古电路板通讯链路异常，请检查通讯链路!")
            self.view.ui.textEdit.append(u"盘古电路板通讯链路异常，请检查通讯链路!")
            self.view.ui.pushButton.setDisabled(False)

    def int2bin(self, num):
        la = []
        if num < 0:
            return '-' + self.int2bin(abs(num))
        while True:
            num, remainder = divmod(num, 2)
            la.append(str(remainder))
            if num == 0:
                return ''.join(la[::-1])

    def checkData(self):
        #data1,data2,data3 = serialOpera.revData()
        d1 = OrderedDict([("releaseStatIn",1),("touchStatIn",1),("encoderAIn",1),("encoderBIn",1),("serialTxIn",1),("elevHallAIn",1),("elevHallBIn",1),("footHomeIn",1)])
        d2 = OrderedDict([("footFuncIn",1),("relayCtrlIn",1),("petExoffIn",1),("stepPulseIn",1),("stepDirIn",1),("servoPulseIn",1),("servoDirIn",1),("stepClutchIn",1)])
        d3 = OrderedDict([("servoClutchIn",1),("hpotClkIn",1),("hpotNcsIn",1),("vpotClkIn",1),("vpotNcsIn",1),("elev1UppDownnIn",1),("elev1UpnDownpIn",1),("elev2UppDownnIn",1)])
        d4 = OrderedDict([("elev2UpnDownpIn",1),("encoderVccIn",1),("hpotVccIn",1),("vpotVccIn",1),("elev1VccIn",1),("elev2VccIn",1),("stepMotorEnIn",1),("servoDisableIn",1)])
        d5 = OrderedDict([("servoAlarmRstIn",1),("servoCwDisableIn",1),("servoCcwDisableIn",1),("reserved0",0),("none",2),("none2",2),("none3",2),("none4",2)])
        d6 = OrderedDict([("motorPulseOut",1),("motorDirOut",1),("motorDisableOut",1),("clutchDisableOut",1),("boardRstOut",1),("elevUpOut",1),("elevDownOut",1),("stepReleaseOut",1)])
        d7 = OrderedDict([("stepTouchOut",1),("servoTouchOut",1),("stepEncoderAOut",1),("servoEncoderAOut",1),("stepEncoderBOut",1),("servoEncderBOut",1),("cradleInLimitOut",1),("cradleOutLimitOut",1)])
        d8 = OrderedDict([("cradleFixPosOut",1),("elevUpLimitOut",1),("elevDownLimitOut",1),("elev1HallAOut",1),("elev1HallBOut",1),("elev2HallAOut",1),("elev2HallBOut",1),("elev2OvcStat",1)])
        d9 = OrderedDict([("elev1OvcStat",1),("servoAlarmStatOut",0),("servoPrepareStatOut",0),("servoWorkStatOut",0),("serialRxOut",1),("hpotMisoOut",1),("vpotMisoOut",1),("framMemory",1)])
        result = [d1,d2,d3,d4,d5,d6,d7,d8,d9]

        data = serialOpera.revData()
        success = True
        for i in range(len(result)):
            print len(result[i].items())
            valLt = []
            keyLt = []
            for key, val in result[i].items():
                valLt.append(val)
                keyLt.append(key)
            print "keyLt: ", keyLt
            print "valLt: ", valLt

            for b in range(8):
                if (int(data[i], 16) & 0xff) & (0x1 << b) == (valLt[b] << b):
                    print "success: ", ((int(data[i], 16) & 0xff) & (0x1 << b)), (valLt[b] << b), valLt[b], b, keyLt[b]
                    #logMessage(("盘古电路板:%s link normal" % keyLt[b]))
                else:
                    if keyLt[b] == "none" or keyLt[b] == "none2" or keyLt[b] == "none3" or keyLt[b] == "none4":
                        pass
                    else:
                        print "failed: ", ((int(data[i], 16) & 0xff) & (0x1 << b)), (valLt[b] << b), valLt[b], b, keyLt[b]
                        logMessage(("盘古电路板: %s link error" % keyLt[b]))
                        success = False
                        #self.view.ui.label_2.setText(u"测试失败，请查看log文件信息")
                        self.view.ui.textEdit.append((u"盘古电路板: %s link error" % keyLt[b]))
                        self.view.ui.pushButton.setDisabled(False)
        if success:
            #self.view.ui.label_2.setText(u"测试通过")
            self.view.ui.textEdit.append(u"盘古电路板测试通过!")
            self.view.ui.pushButton.setDisabled(False)
            logMessage("盘古电路板测试通过!")

        # this here do check accord of data
        # if pass
        # self.view.ui.label_2.setText(u"测试通过")
        # msgLogger.logToFile("盘古电路板测试通过!")
        #logMessage("盘古电路板测试通过!")
        # else
        # self.view.ui.label_2.setText(u"测试失败")
        # msgLogger.logToFile("盘古电路板测试失败!")
        # self.view.ui.pushButton.setDisabled(False)
        """
        if data == None:
            self.view.ui.label_2.setText(u"测试失败")
            self.view.ui.pushButton.setDisabled(False)
            logMessage("盘古电路板测试失败!")
        
        self.view.ui.label_2.setText(u"测试通过")
        self.view.ui.pushButton.setDisabled(False)
        logMessage("盘古电路板测试通过!")
        """

if __name__ == "__main__":
    def int2bin(num):
        la = []
        if num < 0:
            return '-' + int2bin(abs(num))
        while True:
            num, remainder = divmod(num, 2)
            la.append(str(remainder))
            if num == 0:
                return ''.join(la[::-1])

    a = bitarray(8)
    a = int2bin(int('0xff', 16))
    print "a = ", a