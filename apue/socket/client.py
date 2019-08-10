#!/usr/bin/python
#-*- coding:utf-8 -*-

import socket

if __name__=="__main__":
	sd = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
	sd.connect(('192.168.8.1', 8000))

	while True:
		msg = raw_input("please enter: ")
		if not msg:
			continue
		sd.send(msg)

	sd.close()