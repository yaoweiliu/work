#!/usr/bin/python
#-*- coding:utf-8 -*-

import os
from multiprocessing import Process, Pool

if __name__ == "__main__":

	#1.
	"""
	a = 24
	print "start process(%d), a = %d" % (os.getpid(), a)
	pid = os.fork()
	if pid == 0:
		a = 21
		print "child process(%d), a = %d" % (os.getpid(), a)
		os._exit(0)
	elif pid > 0:
		print "father process(%d), a = %d" % (os.getpid(), a)
	else:
		print "failed"
	"""

	#2.
	"""
	def event_handler(a):
		while a:
			print "a = %d, pid = %d" % (a, os.getpid())
			a -= 1

	print "father pid = %d" % os.getpid()
	p = Process(target=event_handler, args=(5,))
	p.start()
	p.join()
	"""

	#3.

	def process_handler(i):
		print "task(%d) run..." % i

	p = Pool()

	for i in range(5):
		p.apply_async(process_handler, args=(i,))
	p.close()
	p.join()


