#!/usr/bin/python
#-*- coding:utf-8 -*-

import serial

class SerialTest(object):
	def __init__(self, arg = None):
		super(SerialTest, self).__init__()
		pass

	def sendData(self):
		ser = serial.Serial("/dev/ttyUSB0", 115200, 0.5)
		ser.write("data")
		ser.close()

	def recvData(self):
		ser = serial.Serial("/dev/ttyUSB0", 115200, 0.5)
		ser.read(4)
		ser.close()

if __name__ == "__main__":
	serObject = SerialTest()
	serObject.sendData()
	serObject.recvData()
		