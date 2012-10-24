import random

n = 50
print n
for _ in xrange(n):
	if random.randint(0, 1):
		print 'a', random.randint(-10, 10)
	else:
		print 'r', random.randint(-10, 10)
