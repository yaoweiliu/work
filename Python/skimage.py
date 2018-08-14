#!/usr/bin/python
#coding=utf-8

from skimage import data,io
from matplotlib import pyplot as plt

if __name__=="__main__":
    img = data.coffee()
    plt.imshow(img)
    plt.show()
