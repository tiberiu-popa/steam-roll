from __future__ import print_function
import sys

def get_digits(x):
	digits = [ ]
	while x > 0:
		digits.append(x % 10)
		x /= 10
	digits.reverse()
	return digits

def is_palindrome(x):
	digits = get_digits(x)
	digits.reverse()
	y = 0
	for digit in digits:
		y = 10 * y + digit
	return x == y

pal = [ ]

for i in xrange(10 ** 7 + 1):
	ok = False
	if i > 0:
		ok = is_palindrome(i)
		if ok:
			ok = is_palindrome(i * i)
	if i % 500000 == 0:
		print('i', float(100 * i) / (10 ** 7 + 1), file=sys.stderr)
	pal.append(1 if ok else 0)

sum_pal = [ pal[0] ]
for i in xrange(1, len(pal)):
	sum_pal.append(sum_pal[-1] + pal[i])

def int_sqrt(x):
	left = 0
	right = x
	while left + 1 < right:
		mid = left + (right - left) / 2
		if mid * mid > x:
			right = mid - 1
		else:
			left = mid
	if right * right <= x:
		return right
	else:
		return left

def solve_problem(x, y):
	p = int_sqrt(x)
	q = int_sqrt(y)
	if p * p < x:
		p += 1
	u = sum_pal[p - 1]
	v = sum_pal[q]
	return v - u

with open('fairsquare.in', 'r') as f:
	first = True
	i = 0
	for line in f:
		if first:
			num_tests = int(line)
			first = False
		else:
			if i < num_tests:
				tokens = line.split()
				a = int(tokens[0])
				b = int(tokens[1])
				print('Case #%d:' % (i + 1,), solve_problem(a, b))
				i += 1
