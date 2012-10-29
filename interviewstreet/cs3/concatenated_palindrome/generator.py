import random

n = 100
m = 100000
print n, m

for _ in xrange(n):
	v = [ ]
	for _ in xrange(m):
		x = random.randint(1, 26 + 26 + 10) - 1
		if x < 26:
			v.append(chr(ord('a') + x))
		else:
			x -= 26
			if x < 26:
				v.append(chr(ord('A') + x))
			else:
				x -= 10
				v.append(chr(ord('0') + x))
	print ''.join(v)
