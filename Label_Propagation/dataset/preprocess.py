"""
file: preprocess.py
date: 01/08/2017
author:leihou
"""
#!/usr/bin/python
import string
import re

def preprocess(originfile,edgefile,vertexfile):
	filein = open(originfile)
	fileoute = open(edgefile,'w')
	fileoutv = open(vertexfile,'w')
	edges = []
	vertices = {}
	save_vertices = []
	numline = 0
	t = 1
	while 1:
		linee = filein.readline()
		if not linee :
			break

		numline += 1
		line = re.split(' |\n|\t',linee)
		if line[0] not in vertices.keys():
			vertices[line[0]] = line[0]
			save_vertices.append(line[0] + '\t' + line[0] + '\n')

		if line[1] not in vertices.keys():
			vertices[line[1]] = line[1]
			save_vertices.append(line[1] + '\t' + line[1] + '\n')

		edges.append(line[0] + '\t' + line[1] + '\t' + '1' + '\n')
		
		t += 1
		print line[0] + '\t' + line[1] + '\t' + '1'

	print len(edges)
	
	fileoute.writelines(edges)
	fileoutv.writelines(save_vertices)
	filein.close()
	fileoute.close()
	fileoutv.close()
	
if __name__ == '__main__':
	print 'start to load data'
	path = './Amazon'
	originfile = path + '/com-amazon.ungraph.txt'
	edgefile = path + '/com-amazon.ungraph.e'
	vertexfile = path + '/com-amazon.ungraph.v'
	preprocess(originfile, edgefile, vertexfile)
print 'Finish !'
