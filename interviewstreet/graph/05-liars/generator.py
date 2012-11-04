import random
import subprocess
import sys

def generate_input(filename):
	with open(filename, 'w') as f:
		n = 4
		f.write('%d\n' % (n,))

		v = [ ]
		for _ in xrange(n):
			v.append(random.randint(0, 1))

		m = random.randint(n / 2, 3 * n / 2)
		f.write('%d\n' % (m,))

		u = [ ]
		for _ in xrange(m):
			x = random.randint(0, n - 1)
			y = random.randint(x, n - 1)
			z = 0
			for i in xrange(x, y + 1):
				z += v[i]
			u.append((x, y, z))
			f.write('%d %d %d\n' % (x + 1, y + 1, z))

		min_elems = sys.maxint
		max_elems = -sys.maxint - 1
		for i in xrange(1 << n):
			bits = n * [ 0 ]
			for j in xrange(n):
				if i & (1 << j) != 0:
					bits[j] = 1
			for x, y, z in u:
				num_elems = 0
				for j in xrange(x, y + 1):
					num_elems += bits[j]
				if num_elems != z:
					break
			else:
				candidate = sum(bits)
				min_elems = min(min_elems, candidate)
				max_elems = max(max_elems, candidate)
		return min_elems, max_elems

done = False
while not done:
	min_elems, max_elems = generate_input('gen_input.txt')
	p = subprocess.Popen('./liars', stdin=open('gen_input.txt'), stdout=subprocess.PIPE, stderr=subprocess.PIPE)
	while True:
		line = p.stdout.readline()
		if not line:
			break
		tokens = line.split()
		min_ilp = int(tokens[0])
		max_ilp = int(tokens[1])
		print min_elems, max_elems
		if min_ilp != min_elems or max_ilp != max_elems:
			done = True
