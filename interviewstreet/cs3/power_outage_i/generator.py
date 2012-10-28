import random

num_tests = 1
print num_tests

for _ in xrange(num_tests):
	n = 2000
	m = n * (n - 1) / 2
	k = n / 7
	print n, m, k
	for i in xrange(n):
		for j in xrange(i + 1, n):
			print i + 1, j + 1, random.randint(1, 1000000)
