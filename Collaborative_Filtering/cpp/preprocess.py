"""
file: preprocess.py
date: 01/08/2017
author:leihou
"""
#!/usr/bin/python
import string

def preprocess(originfile,edgefile,vertexfile):
	filein = open(originfile)
	fileoute = open(edgefile,'w')
	fileoutv = open(vertexfile,'w')
	lines = []
	vuser = {}
	vusers = []
	vitem = {}
	vitems = []
	vs = {}
	v = []
	numline = 0
	i = 0
	t = 1
	while 1:
		linee = filein.readline()
		if not linee :
			break
		numline += 1
		if numline < 2:
			continue
		line = linee.split('::')
		
		if line[0] not in vuser.keys():
			vuser[line[0]] = i
			vs[i] = '0'
			vusers.append(str(i) + '\t' + '0' + '\n')
			i = i + 1
		if line[1] not in vitem.keys():
			vitem[line[1]] = i
			vitems.append(str(i) + '\t' + '1' + '\n')
			vs[i] = '1'
			i = i + 1
		if t % 10 == 5 or t % 10 == 9:
			lines.append(str(vuser[line[0]]) + '\t' + str(vitem[line[1]]) + '\t' +line[2] + '\n')
		else:
			lines.append(str(vuser[line[0]]) + '\t' + str(vitem[line[1]]) + '\t' + line[2] + '\n')
		t += 1
		print str(vuser[line[0]]) + '\t' + str(vitem[line[1]]) + '\t' +'-'+ line[2]
	print len(vitems)
	for lin in vs.keys():
		v.append(str(lin) + '\t' + vs[lin] + '\n')	
	fileoute.writelines(lines)
	fileoutv.writelines(v)
	filein.close()
	fileoute.close()
	fileoutv.close()
	
if __name__ == '__main__':
	print 'start to transfrom data'
	path = '../datasets/ml-10M100K/'
	#path = '../datasets/ml-latest-small/'
	originfile = path + 'ratings.dat'
	edgefile = path + 'user-movie.e'
	vertexfile = path + 'user-movie.v'
	preprocess(originfile, edgefile, vertexfile)
print 'Finish !'
