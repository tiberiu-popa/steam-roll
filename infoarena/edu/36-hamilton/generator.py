import random

n = 18
print n

m = n * (n - 1)
print m

for i in xrange(n):
	for j in xrange(n):
		if i != j:
			print i, j, random.randint(1, 1000000)
