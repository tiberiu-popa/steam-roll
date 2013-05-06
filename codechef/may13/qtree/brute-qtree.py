from __future__ import print_function
import collections
import sys

def bfs(src):
	q = collections.deque()
	q.append(src)
	dist = [ 0 for _ in xrange(n) ]
	parent = [ -1 for _ in xrange(n) ]
	visited = [ False for _ in xrange(n) ]
	visited[src] = True
	while len(q) > 0:
		node = q.popleft()
		for k, v in edges[node].iteritems():
			if not visited[k]:
				q.append(k)
				dist[k] = dist[node] + 1
				parent[k] = node
				visited[k] = True
	return dist, parent

lines = [ ]
for line in sys.stdin:
	line = line.rstrip()
	lines.append(line)

n = int(lines[0])

edges = [ {} for _ in xrange(n) ]

for line in lines[1:(n + 1)]:
	tokens = line.split()
	x = int(tokens[0])
	y = int(tokens[1])
	c = int(tokens[2])
	x -= 1
	y -= 1
	edges[x][y] = c
	edges[y][x] = c

#print(edges)

distances = []
parents = []
for i in xrange(n):
	dist, parent = bfs(i)
	distances.append(dist)
	parents.append(parent)

q = int(lines[n + 1])
for line in lines[(n + 2):]:
	tokens = line.split()
	x = int(tokens[1])
	y = int(tokens[2])
	x -= 1
	y -= 1
	if tokens[0] == 'f':
		v = y
		while v != x:
			u = parents[x][v]
			edges[u][v] *= -1
			edges[v][u] *= -1
			v = u
	else:
		xs = []
		v = y
		while v != x:
			xs.append(v)
			u = parents[x][v]
			v = u
		xs.append(v)
		ys = []
		for u, v in zip(xs, xs[1:]):
			ys.append(edges[u][v])
		max_sum = 0
		sum = 0
		for elem in ys:
			sum += elem
			if sum >= 0:
				if sum > max_sum:
					max_sum = sum
			else:
				sum = 0
		print(max_sum)
