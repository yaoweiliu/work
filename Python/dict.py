#!/usr/bin/python
#-*- coding:utf-8 -*-

from collections import defaultdict, OrderedDict

if __name__ == "__main__":
	"""
	data = OrderedDict([("abit1":1, "abit2":1, "abit3":1, "abit4":1, "abit5":1, "abit6":1, "abit7":1, "abit8":1),
						("bbit1":0, "bbit2":0, "bbit3":0, "bbit4":1, "bbit5":1, "bbit6":1, "bbit7":1, "bbit8":1),])
	"""
	data = OrderedDict({'a':1,'b':2,'c':3})

	print "first data: ", data

	d1 = OrderedDict([("abit1",1), ("abit2",1), ("abit3",1), ("abit4",1), ("abit5",1), ("abit6",1), ("abit7",1), ("abit8",1)])
	d2 = OrderedDict([("bbit1",0), ("bbit2",0), ("bbit3",0), ("bbit4",1), ("bbit5",1), ("bbit6",1), ("bbit7",1), ("bbit8",1)])
	d3 = OrderedDict([("cbit1",0), ("cbit2",0), ("cbit3",0), ("cbit4",0), ("cbit5",1), ("cbit6",1), ("cbit7",1), ("cbit8",1)])
	d4 = OrderedDict([("dbit1",0), ("dbit2",0), ("dbit3",0), ("dbit4",1), ("dbit5",1), ("dbit6",1), ("dbit7",1), ("dbit8",1)])
	d5 = OrderedDict([("ebit1",0), ("ebit2",0), ("ebit3",0), ("ebit4",1), ("ebit5",1), ("ebit6",1), ("ebit7",1), ("ebit8",1)])
	d6 = OrderedDict([("fbit1",0), ("fbit2",0), ("fbit3",0), ("fbit4",1), ("fbit5",1), ("fbit6",1), ("fbit7",1), ("fbit8",1)])
	d7 = OrderedDict([("hbit1",0), ("hbit2",0), ("hbit3",0), ("hbit4",1), ("hbit5",1), ("hbit6",1), ("hbit7",1), ("hbit8",1)])
	d8 = OrderedDict([("ibit1",1), ("ibit2",1), ("ibit3",1), ("ibit4",1), ("ibit5",0), ("ibit6",0), ("ibit7",0), ("ibit8",1)])


	for k,v in d1.items():
		#print k, v
		pass


	data = [d1, d2, d3, d4, d5, d6, d7, d8]

	result1 = 0xff
	result2 = 0x1f
	result3 = 0xf
	result4 = 0x1f
	result5 = 0x1f
	result6 = 0x1f
	result7 = 0x1f
	result8 = 0xf1

	result = [result1, result2, result3, result4, result5, result6, result7, result8]

	if True == 1:
		print "True is equl to 1"

	for b in range(8):
		#print result1 & (0x1 << b) == (0x1 << b), result2 & (0x1 << b) == (0x1 << b)
		pass

	cunt = 0
	for i in range(len(data)):
		for key,val in data[i].items():
			#print key, val, data[i][key]
			print data[i][key], cunt
			cunt += 1
			for b in range(8):
				if (result[i] & (0x1 << b) == (0x1 << b)) == data[i][key]:
					#print "is ", key, data[i][key]
					pass
				else:
					#print "not ", key, data[i][key]
					pass

		for key in data[i].keys():
			#print data[i][key]
			pass

	

