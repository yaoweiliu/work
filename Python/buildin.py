#!/usr/bin/python
#-*- coding=utf-8 -*-

#DataBase must inherit object, then use super().
class DataBase(object):
	def __init__(self, a, b):
		self.a = a
		self.b = b

	def pr(self):
		print "a = %d, b = %c" % (self.a, self.b)

		return self.a

class Data(DataBase):
	def __init__(self, a, b):
		super(Data, self).__init__(a, b)
		pass



if __name__ == "__main__":
	data = Data(21, "t")
	if hasattr(data, "pr"):
		data.pr()

		a = getattr(data, "pr")
		print a, type(a)

	lt = [1,1,10,0,0,0,1,5,0,1,4,8]
	indx = [i for i, x in enumerate(lt) if x == 1]
	print indx


