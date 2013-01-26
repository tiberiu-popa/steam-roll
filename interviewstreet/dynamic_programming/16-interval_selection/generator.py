import random

num_tests = 1
print num_tests
n = 1000
print n
for _ in xrange(n):
	x = random.randint(1, 1000000)
	y = random.randint(1, 1000000)
	print min(x, y), max(x, y)
