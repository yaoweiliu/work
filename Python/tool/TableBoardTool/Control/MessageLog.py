# coding=utf-8
#!/usr/bin/env python

import os
import sys
import time

class MessageLog(object):
    __instance = None
    def __init__(self):
        self.logFile = ""

    def __new__(cls, *args, **kwargs):
        if not cls.__instance:
            cls.__instance = object.__new__(cls)
        return cls.__instance

    def init(self, userName, testTime, serialID, logFile):
        self.testUserName  = userName
        self.testTime = testTime
        self.serialID = serialID
        self.logFile = logFile

        self.logToFile("测试人：" + self.testUserName)
        self.logToFile("测试时间：" + self.testTime)
        self.logToFile("电路板序列号：" + self.serialID)

    def logToFile(self, msg):
        with open(self.logFile, "a+") as fd:
            ISOTIMEFORMAT='%Y-%m-%d %X'
            fd.write(time.strftime(ISOTIMEFORMAT, time.localtime(time.time())) + '\t' + str(msg + '\n'))

    def displayResult(self):
        resultList = []
        with open(self.logFile, "r") as fd:
            for line in fd.readlines():
                resultList.append(line)
        return resultList
		
msgLogger = MessageLog()

def logMessage(msg):
    msgLogger.logToFile(msg)
	
def clearLogMessage():
    #msgLogger.clearLog()
    pass

if __name__=="__main__":
    msgLogger.init("lyw", "2018-10-31", "1024", "D:\\BoardJigLog\\test.log")
    logMessage("test")
    logMessage("sv")