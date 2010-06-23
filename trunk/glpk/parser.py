#!/usr/bin/env python

import sys

if len(sys.argv) < 2:
	print '\n\tUSO: parser.py arquivo.txt arquivo.dat\n'
	exit(0)

handlerRead = open(sys.argv[1], 'r')
lines = handlerRead.readlines()
lines = map(lambda x: x.strip(), lines)
elements = map(lambda x: x.split(' '), lines)
sEls = []

for element in elements:
	sEls.extend(element)

planes = int(sEls[0])
freeze = sEls[1]

ep = 2
planesList = [] 
for i in range(planes+1)[1:]:
	planeInfo = {}
	for param in ['R','E','T','L','g','h']:
		planeInfo[param] = sEls[ep]
		ep += 1

	planeInfo['S'] = []
	for j in range(planes+1)[1:]:
		planeInfo['S'].append((i,j,sEls[ep],))
		ep += 1
	planesList.append(planeInfo)

handlerRead.close()
handlerWrite = open(sys.argv[2], 'w')
handlerWrite.write("param n := %s;\n" % planes)
handlerWrite.write("param freezetime := %s;\n\n" % freeze)

for param in ['R','E','T','L','g','h']:
	handlerWrite.write("param %s :=\n" % param)
	for i in range(planes):
		handlerWrite.write("%s %s\n" % (i+1, planesList[i][param]))
	handlerWrite.write(";\n\n")

handlerWrite.write("param S :=\n")

for i in range(planes):
	for j in planesList[i]['S']:
		handlerWrite.write("%s %s %s\n" % j)

handlerWrite.write(";\n")
handlerWrite.close()
