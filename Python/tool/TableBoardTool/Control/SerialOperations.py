#-*- coding:utf-8 -*-

import serial
import serial.tools.list_ports
import sys
import time

class SerialOperations():
    def __init__(self):
        plist = list(serial.tools.list_ports.comports())
        #self.portSend = str(plist[1])[:4]
        #self.portRecv = str(plist[2])[:4]
        #self.port = str(plist[0])[:4]
        self.port = "COM5"
        #self.ser = serial.Serial('COM5', 115200)
        self.ser = serial.Serial(str(plist[len(plist)-1])[:4], 115200, timeout=2)
        print self.ser
        pass

    def revData(self):
        #with serial.Serial(self.port, 115200, timeout=2) as ser: #timeout = 2
        data = self.ser.read(13)
        for i in range(13):
            print str(hex(ord(data[i:i+1])))

        try:
            header = str(hex(ord(data[:1])))
            print "header: ", header
            if header == "0x55":
                packlen = str(hex(ord(data[1:2])))
                print "packlen: ", packlen
                if packlen == "0xd":
                    cmd = str(hex(ord(data[2:3])))
                    print "cmd: ", cmd
                    if cmd == "0xd0":
                        respData = data[3:12]
                        value = []
                        for i in range(len(respData)):
                            print "data: ", str(hex(ord(respData[i:i+1])))
                            value.append(str(hex(ord(respData[i:i+1]))))
                        self.ser.close()
                        return value
                        #return str(hex(ord(respData[:1]))), str(hex(ord(respData[1:2]))), str(hex(ord(respData[2:3])))
        except:
            print "read serial port error!"


    def checkLink(self):
        try:
            data = self.ser.read(5)
            for i in range(5):
                print str(hex(ord(data[i:i + 1])))
        except:
            print "link error"
            return "link_error"

        try:
            header = str(hex(ord(data[:1])))
            print "header: ", header
            if header == "0x55":
                packlen = str(hex(ord(data[1:2])))
                print "packlen: ", packlen
                if packlen == "0x5":
                    cmd = str(hex(ord(data[2:3])))
                    print "cmd: ", cmd
                    if cmd == "0xb0":
                        respData = str(hex(ord(data[3:4])))
                        print "respData: ", respData
                        if respData == "0x0":
                            checkSum = str(hex(ord(data[4:5])))
                            print "checkSum: ", checkSum
                            if checkSum == "0xb0":
                                return "pg_link_is_ok"
                    elif cmd == "0xa0":
                        respData = str(hex(ord(data[3:4])))
                        print "respData: ", respData
                        if respData == "0x0":
                            checkSum = str(hex(ord(data[4:5])))
                            print "checkSum: ", checkSum
                            if checkSum == "0xa0":
                                return "px_link_is_ok"
                    elif cmd == "0xc0":
                        #self.ser.close()
                        return "cmd_error"
                    else:
                        return "link_error"
                else:
                    return "link_error"
            else:
                return "link_error"
        except:
            print "link_error"

    """
    def sendData(self, header, packlen, cmd, data, checksum):
        print header, packlen, cmd, data, checksum
        #with serial.Serial(self.port, 115200, write_timeout=1) as ser: #, write_timeout=1
        #ser.baudrate = 115200
        #ser.port = self.port
        #ser.write_timeout = 1
        #ser.open()
        str = self.ser.write(header)
        str = self.ser.write(packlen)
        str = self.ser.write(cmd)
        str = self.ser.write(data)
        str = self.ser.write(checksum)
        print 'write str: ', str
        #ser.close()
        print 'send'
    """

    def sendData(self, protol):
        print "sendData: ", protol
        #map(lambda x: hex(x, 10), protol)
        #with serial.Serial(self.port, 115200, write_timeout=1) as ser:  # , write_timeout=1
        str = self.ser.write(protol)
        print 'write str: ', str
        #ser.close()
        print 'send'


    def test_write(self):
        ser = serial.Serial('COM1', 115200, timeout = 1)
        print ser
        len1 = ser.write("1")
        ser.close()
        print "len: ", len1

    def test_read(self):
        ser = serial.Serial('COM1', 115200, timeout=1)
        print ser
        data = ser.read(1)
        print "data: ", data
        ser.close()

serialOpera = SerialOperations()

if __name__=="__main__":
    """
    sr = SerialOperations()
    #sr.sendData("0x55", "0x05", "0xA0", "0x00", "0xA0")
    print 'start send'
    sr.test_write()
    #time.sleep(1)

    sr.test_read()
    plist = list(serial.tools.list_ports.comports())
    for i in range(len(plist)):
        print "list[%d]: %s" % (i, plist[i])
        print plist[0], type(plist[0])

    print [port for port in serial.tools.list_ports.comports() if port[2] != 'n/a']
    """

    print chr(0x55), chr(0xB0)

    result = [
        {"releaseStatIn": 1, "touchStatIn": 1, "encoderAIn": 1, "encoderBIn": 1, "serialTxIn": 1, "elevHallAIn": 1,
         "elevHallBIn": 1, "footHomeIn": 1},
        {"footFuncIn": 1, "relayCtrlIn": 1, "petExoffIn": 1, "stepPulseIn": 1, "stepDirIn": 1, "servoPulseIn": 1,
         "servoDirIn": 1, "stepClutchIn": 1},
        {"servoClutchIn": 1, "hpotClkIn": 1, "hpotNcsIn": 1, "vpotClkIn": 1, "vpotNcsIn": 1, "elev1UppDownnIn": 1,
         "elev1UpnDownpIn": 1, "elev2UppDownnIn": 1},
        {"elev2UpnDownpIn": 1, "encoderVccIn": 1, "hpotVccIn": 1, "vpotVccIn": 1, "elev1VccIn": 1, "elev2VccIn": 1,
         "stepMotorEnIn": 1, "servoDisableIn": 1},
        {"servoAlarmRstIn": 1, "servoCwDisableIn": 1, "servoCcwDisableIn": 1, "reserved0": 0, "none": 2, "none2": 2,
         "none3": 2, "none4": 2},
        {"motorPulseOut": 1, "motorDirOut": 1, "motorDisableOut": 1, "clutchDisableOut": 1, "boardRstOut": 1,
         "elevUpOut": 1, "elevDownOut": 1, "stepReleaseOut": 1},
        {"stepTouchOut": 1, "servoTouchOut": 1, "stepEncoderAOut": 1, "servoEncoderAOut": 1, "stepEncoderBOut": 1,
         "servoEncderBOut": 1, "cradleInLimitOut": 1, "cradleOutLimitOut": 1},
        {"cradleFixPosOut": 1, "elevUpLimitOut": 1, "elevDownLimitOut": 1, "elev1HallAOut": 1, "elev1HallBOut": 1,
         "elev2HallAOut": 1, "elev2HallBOut": 1, "elev2OvcStat": 1},
        {"elev1OvcStat": 1, "servoAlarmStatOut": 0, "servoPrepareStatOut": 0, "servoWorkStatOut": 0, "serialRxOut": 1,
         "hpotMisoOut": 1, "vpotMisoOut": 1, "framMemory": 1},
    ]
    #print result

    plist = list(serial.tools.list_ports.comports())
    portSend = str(plist[1])[:4]
    portRecv = str(plist[2])[:4]

    with serial.Serial(portSend, 115200, timeout=19) as ser:
        print ser.inWaiting(), ser
        #while True:
        data = ''
        data += ser.read(5)#14
        # data = ser.readall()
        print "data: ", data, type(data)
        #try:
        header = str(hex(ord(data[:1])))
        print "header: ", header
        if header == "0x55":
            packlen = str(hex(ord(data[1:2])))
            print "packlen: ", packlen
            if packlen == "0x5":
                cmd = str(hex(ord(data[2:3])))
                print "cmd: ", cmd
                if cmd == "0xa1":
                    respData = data[3:13]
                    # checksum = str(hex(ord(data[13:14])))
                    # if checksum == ""
                    ser.close()
                    print "respData: ", str(ord(respData[:1])), str(ord(respData[1:2])), str(ord(respData[2:3])), str(ord(respData[3:4])), \
                            str(ord(respData[4:5])), str(ord(respData[5:6])), str(ord(respData[6:7])), str(ord(respData[7:8])), str(ord(respData[8:9])), \
                            str(ord(respData[9:10]))
                elif cmd == "0xA0":
                    ser.close()
                    print "start_test_px"
                elif cmd == "0xB0":
                    ser.close()
                    print "start_test_pg"
                elif cmd == "0xC0":
                    ser.close()
                    print "cmd_error"
        #except:
            #print "read serial port error!"
        """
        header = ser.read(2)
        #header = ser.readlines()
        print type(header), header[:1]
        print "header: ", hex(ord(header[:1]))
        #if hex(ord(header)) == 0x55:
        if str(hex(ord(header[:1]))) == "0x55":
            print "ttt: "
            #packlen = ser.read(1)
            packlen = header[1:2]
            print "packlen: ", hex(ord(packlen))
        """

"""
    with serial.Serial(portRecv, 115200, timeout=1) as ser:
        header = ser.read(1)
        print "header: ", header
        if header == 0x55:
            packlen = ser.read(1)
            print "packlen: ", packlen
"""