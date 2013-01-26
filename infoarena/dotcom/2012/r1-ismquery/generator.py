import random

n = 400000
m = 1200000
g = 500
h = 600

print n, m

v = [ ]
for _ in xrange(n):
	v.append(random.randint(-1000000, 1000000))
print ' '.join([ str(elem) for elem in v ])

print g, h
