#!/usr/bin/python
#-*- coding:utf-8 -*-

import threading
import time

class ThreadClass(threading.Thread):
    def __init__(self, threadName, num):
        #threading.Thread.__init__(self)
        super(ThreadClass, self).__init__()
        self.name = threadName
        self.num = num

    def run(self):
        self.printParam()

    def printParam(self):
        print ("%s %d") % (threading.current_thread().getName(), self.num)

if __name__=="__main__":
    thread1 = ThreadClass("t1", 1)
    thread2 = ThreadClass("t2", 2)

    thread1.start()
    time.sleep(1)
    thread2.start()
    time.sleep(1)

    thread1.join()
    thread2.join()

    print threading.current_thread().getName()
