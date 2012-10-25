import random

t = 50
print t
#max_deadline = 100000
max_deadline = 10
for _ in xrange(t):
	d = random.randint(1, max_deadline)
	m = random.randint(1, 1000)
	print d, m
