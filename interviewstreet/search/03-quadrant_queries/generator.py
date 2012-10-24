import random

n = 100000
print n

for _ in xrange(n):
	x = 2 * random.randint(0, 1) - 1
	y = 2 * random.randint(0, 1) - 1
	print x, y

q = 50
print q
for _ in xrange(q):
	type = random.randint(0, 2)
	x = random.randint(1, n)
	y = random.randint(x, n)
	if type == 0:
		type = 'X'
	elif type == 1:
		type = 'Y'
	else:
		type = 'C'
	print type, x, y
