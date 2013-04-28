from __future__ import print_function
import random

t = 1
r = 8000
n = 12
m = 8
k = 12

with open('good-luck.in', 'w') as f:
	with open('good-luck.res', 'w') as g:
		print(t, file=f)
		print(r, n, m, k, file=f)
		for _ in xrange(r):
			v = []
			for _ in xrange(n):
				v.append(random.randint(2, m))
			u = sorted(v)
			print(''.join(map(str, u)), file=g)
			sub_values = []
			for _ in xrange(k):
				prod = 1
				for elem in v:
					if random.randint(0, 1) == 1:
						prod *= elem
				sub_values.append(prod)
			print(' '.join(map(str, sub_values)), file=f)
