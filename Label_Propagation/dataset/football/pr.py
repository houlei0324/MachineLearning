"""
file: preprocess.py
date: 01/08/2017
author:leihou
"""
#!/usr/bin/python
import string
import re

def pr(originfile,edgefile,vertexfile):
	filein = open(originfile)
	fileoute = open(edgefile,'w')
	fileoutv = open(vertexfile,'w')
	edges = []
	vertices = {}
	save_vertices = []
	temp1 = 'a'
	temp2 = 'b'
	numline = 0
	t = 1
	while 1:
		linee = filein.readline()
		if not linee :
			break

		numline += 1
		if t % 5 == 3:
			line = re.split(' |\n|\t',linee)
			print line
			temp1 = line[5]
			if line[5] not in vertices.keys():
				vertices[line[5]] = line[5]
				save_vertices.append(line[5] + '\t' + line[5] + '\n')
		
		if t % 5 == 4:
			line = re.split(' |\n|\t',linee)
			print line
			temp2 = line[5]
			if line[5] not in vertices.keys():
				vertices[line[5]] = line[5]
				save_vertices.append(line[5] + '\t' + line[5] + '\n')
			edges.append(temp1 + '\t' + temp2 + '\t' + '1' + '\n')
			print temp1 + '\t' + temp2 + '\t' + '1' + '\n'

		
		t += 1
		

	print len(edges)
	
	fileoute.writelines(edges)
	fileoutv.writelines(save_vertices)
	filein.close()
	fileoute.close()
	fileoutv.close()
	
if __name__ == '__main__':
	print 'start to load data'
	path = '.'
	originfile = path + '/football.gml'
	edgefile = path + '/football.e'
	vertexfile = path + '/football.v'
	pr(originfile, edgefile, vertexfile)
print 'Finish !'
