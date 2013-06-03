#!/usr/bin/python
from __future__ import print_function
import sys

def cdiv2(x):
	return (x + 1) // 2

def fdiv2(x):
	return x // 2

def unwrap_intervals(intervals, mod):
	new_intervals = []
	for interval in intervals:
		if interval[0] > interval[1]:
			new_intervals.append([0, interval[1]])
			new_intervals.append([interval[0], mod - 1])
		else:
			new_intervals.append(interval)
	new_intervals.sort()
	return new_intervals

def conjugate_intervals(intervals, mod):
	new_intervals = []
	if intervals[0][0] > 0:
		new_intervals.append([0, intervals[0][0] - 1])
	for left, right in zip(intervals, intervals[1:]):
		new_intervals.append([left[1] + 1, right[0] - 1])
	if intervals[-1][1] < mod - 1:
		new_intervals.append([intervals[-1][1] + 1, mod - 1])
	return new_intervals

def gcd(x, y):
	if x < y:
		x, y = y, x
	while y > 0:
		x, y = y, x % y
	return x

def sift_intervals(interval, g):
	left = (interval[0] + g - 1) // g
	right = interval[1] // g
	if left <= right:
		return [left, right]
	else:
		return []

def reduce_modulo(interval, p):
	return map(lambda x: x % p, interval)

def mulmod(x, y, p):
	return (x * y) % p

def invmod(x, p):
	result = [[1, 0], [0, 1]]
	base = [[0, 1], [1, 0]]

	y = x % p
	x = p
	while y > 0:
		q = x / y
		r = x % y
		base[1][1] = -q
		tmp = [[0, 0], [0, 0]]
		for i in xrange(2):
			for j in xrange(2):
				for k in xrange(2):
					tmp[i][j] += result[i][k] * base[k][j]
		result = tmp
		x = y
		y = r

	inv = result[1][0]
	if inv < 0:
		inv += p
	return inv

def euclid(k, n, interval):
	reflect = lambda x: n - x
	flip = False

	if 2 * k > n:
		k = reflect(k)
		interval = map(reflect, interval)
		interval.reverse()
		flip = True

	turn = sift_intervals(interval, k)
	if turn:
		result = k * turn[0]
	else:
		k_interval = reduce_modulo(interval, k)
		p = k - n % k
		q = euclid(p, k, k_interval)
		result = q + k * (interval[0] // k)

	return reflect(result) if flip else result

def full_euclid(interval, k, n):
	g = gcd(k, n)
	interval = sift_intervals(interval, g)
	if interval:
		if interval[0] == 0:
			return 0
		else:
			k /= g
			n /= g
			e = euclid(k, n, interval)
			return mulmod(e, invmod(k, n), n)
	else:
		return None

def analyze(pos, paddle_speed, ball_speed, n, height):
	mod = 2 * height
	reflect = lambda x: (-x) % mod
	shift_by_pos = lambda x: (x - pos) % mod

	full_ball_speed = (n * ball_speed) % mod
	if full_ball_speed > height:
		ball_speed = reflect(ball_speed)
		full_ball_speed = reflect(full_ball_speed)
		pos = reflect(pos)

	result = None
	if paddle_speed < full_ball_speed:
		rs = reflect(full_ball_speed)
		left = cdiv2(rs - paddle_speed)
		right = fdiv2(rs + paddle_speed)
		intervals = []
		intervals.append([ left, right ])
		intervals.append(map(lambda x: x + height, intervals[0]))
		intervals = map(lambda x: map(shift_by_pos, x), intervals)
		intervals = unwrap_intervals(intervals, mod)
		intervals = conjugate_intervals(intervals, mod)
		for interval in intervals:
			e = full_euclid(interval, ball_speed, mod)
			if e is not None:
				#e = mulmod(e, invmod(ball_speed, mod), mod)
				if result is None or e < result:
					result = e
	return result

def solve(f, g, num_case):
	read_ints = lambda: map(int, f.readline().rstrip().split())
	(a, b) = read_ints()
	(n, m) = read_ints()
	(v, w) = read_ints()
	(y, x, vy, vx) = read_ints()

	flip = False

	if vx == 0:
		print('Case #%d: DRAW' % (num_case), file=g)
		return
	elif vx < 0:
		flip = True
		vx = -vx
		n, m = m, n
		v, w = w, v

	if vy == 0:
		print('Case #%d: DRAW' % (num_case), file=g)
		return
	elif vy < 0:
		y = a - y
		vy = -vy

	a *= vx
	y *= vx
	vx = 1

	p1 = (y + (b - x) * vy) % (2 * a)
	p0 = (p1 + b * vy) % (2 * a)

	maybe_add = lambda x, offset: None if x is None else x + offset
	r1 = analyze(p1, (2 * b * m * w), (2 * b * vy) % (2 * a), m, a)
	r0 = analyze(p0, (2 * b * n * v), (2 * b * vy) % (2 * a), n, a)

	r1 = maybe_add(r1, m)
	r0 = maybe_add(r0, n)
	winner = -1
	other = 0
	if r1 is not None:
		if r0 is not None:
			if r0 < r1:
				winner = 1
				other = r0
			else:
				winner = 0
				other = r1
		else:
			winner = 0
			other = r1
	else:
		if r0 is not None:
			winner = 1
			other = r0

	if flip and winner >= 0:
		winner ^= 1

	if winner == 0:
		print('Case #%d: LEFT %s' % (num_case, other), file=g)
	elif winner == 1:
		print('Case #%d: RIGHT %s' % (num_case, other), file=g)
	else:
		print('Case #%d: DRAW' % (num_case,), file=g)

def solve_problem(f, g):
	line = f.readline().rstrip()
	num_tests = int(line)
	for i in xrange(num_tests):
		solve(f, g, i + 1)

def main(argv):
	filenames = [
		'multi-pong.in',
		'multi-pong.out',
	]
	for i, elem in enumerate(argv[1:]):
		if i >= 2:
			break
		filenames[i] = elem
	with open(filenames[0], 'r') as f:
		with open(filenames[1], 'w') as g:
			solve_problem(f, g)

if __name__ == '__main__':
	main(sys.argv)
