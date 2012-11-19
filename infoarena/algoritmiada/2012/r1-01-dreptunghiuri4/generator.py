import random

n = 1000
k = 100
print n, k

for _ in xrange(n):
	x = random.randint(-1000000000, 1000000000)
	y = random.randint(-1000000000, 1000000000)
	z = random.randint(-1000000000, 1000000000)
	t = random.randint(-1000000000, 1000000000)
	print x, y, z, t
