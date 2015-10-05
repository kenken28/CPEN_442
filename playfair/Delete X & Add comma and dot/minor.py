#!/usr/bin/env python

import operator

wordlenth = 3

rfile = open("plainNoSpace.txt", "r")
wfile = open("final.txt", "w")
flag0 = 0
flag1 = 0
state = 1

while True:
    word = rfile.read(wordlenth)
    rfile.seek(1-wordlenth,1)

    if len(word) != wordlenth:
        break
    else:
        if state == 0:
            if word[0] == word[2] and word[1] == 'x':
                word == word[0] + word[2]
                flag1 = 1
            if flag0 == 0:
                wfile.write(word[0])
            else:
                flag0 = 0
            state = 1
        else:
            if word[0] == word[2] and word[1] == 'x':
                word == word[0] + word[2]
                flag0 = 1
            if flag1 == 0:
                wfile.write(word[0])
            else:
                flag1 = 0
            state = 0


rfile.close()
wfile.close()
