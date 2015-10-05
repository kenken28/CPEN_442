#!/usr/bin/env python

import operator

wordlenth = 5

rfile = open("final.txt", "r")
wfile = open("final1.txt", "w")

count = 0

while True:
    word = rfile.read(wordlenth)
    rfile.seek(1-wordlenth,1)

    if len(word) != wordlenth:
        break
    else:
        if word == "comma":
            count = wordlenth
            wfile.write(",")
        if count == 0:
            wfile.write(word[0])
        else:
            count -= 1


rfile.close()
wfile.close()
