#!/usr/bin/python

import random
import sys

# input
if len(sys.argv) == 2:
	tmp = int(sys.argv[1])
	print tmp*(tmp-1)/2
	exit(0)
	
if len(sys.argv) != 3:
	print "Wrong usage"
	exit(1)

vert_count = int(sys.argv[1])
edge_count = int(sys.argv[2])

# processing (random graph)
edge_list = []
for i in range(vert_count):
	edge_list.append([])

radius = 10
for i in range(edge_count):
	vert0 = random.randint(0, vert_count - 1)
	vert1 = random.randint(max(0, vert0 - radius), min(vert_count - 1, vert0 + radius))
	while vert1 == vert0:
		vert1 = random.randint(max(0, vert0 - radius), min(vert_count - 1, vert0 + radius))
	edge_list[vert0].append(vert1)
	
# processing (adding two paths)
r0 = range(0, vert_count / 2 - 1)
for i in r0:
	edge_list[i].append(i+1)
edge_list[vert_count / 2 - 1].append(vert_count - 1)

r1 = range(vert_count / 2, vert_count - 1)
for i in r1:
	edge_list[i].append(i+1)
edge_list[0].append(vert_count / 2)

# ouput	
print vert_count, edge_count + vert_count
for i in range(vert_count):
	for v in edge_list[i]:
		print i, v, 1
