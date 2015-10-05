#!/usr/bin/env python

import operator
import math

wordlenth = 3
mydict = {}

with open("count_3.txt", "r") as f:
    content = f.readlines()
f.close()

alist = []
total=0
for line in content:
    items = line.split()
    total += int(items[1])

wfile = open("countSorted.txt", "w")
for line in sorted(content):
    items = line.split()
    wfile.write(items[0] + "  ")
    perc ='{0:.10f}'.format( math.log(float(items[1])/total) )
    wfile.write(str(perc))
    wfile.write("\n")
wfile.close()
