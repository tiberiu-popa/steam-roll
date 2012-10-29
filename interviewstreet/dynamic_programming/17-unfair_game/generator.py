import random

num_tests = 1
print num_tests

max_number = 1000000000

for _ in xrange(num_tests):
	n = 15
	print n
	v = [ ]
	for _ in xrange(n):
		v.append(random.randint(1, max_number))
	print ' '.join([ str(elem) for elem in v ])
