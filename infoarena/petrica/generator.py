import random

n = 200
print n

for _ in xrange(n):
	print random.randint(1, 1000)

v = range(1, n + 1)
random.shuffle(v)
for i in xrange(1, n):
	j = random.randint(0, i - 1)
	print v[i], v[j]

