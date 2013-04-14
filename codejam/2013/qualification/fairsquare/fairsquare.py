from __future__ import print_function

max_length = 50

half_table = [ [ 1 if x == 1 or x == 4 else 0 for x in xrange(5) ] ]
for _ in xrange(1, max_length):
	prev = half_table[-1]
	next = 5 * [ 0 ]
	for i in xrange(5):
		for j in xrange(3):
			if j * j <= i:
				next[i] += prev[i - j * j]
	half_table.append(next)

full_table = [ [ 1 if x == 1 or x == 4 or x == 9 else 0 for x in xrange(10) ] ]
for i in xrange(len(half_table)):
	full_table.append(reduce(lambda x, y: x + y, [ [ x , 0 ] for x in half_table[i] ], [ ]))
	row = 10 * [ 0 ]
	for j, elem in enumerate(half_table[i]):
		row[2 * j] += elem
		row[2 * j + 1] += elem
		if j <= 2:
			row[2 * j + 4] += elem
	full_table.append(row)
sum_full_table = map(sum, full_table)

accum_table = [ ]
for i in xrange(len(full_table)):
	if i < 2:
		accum_table.append([ 1 if j == 0 else x for j, x in enumerate(full_table[i]) ])
	else:
		accum_table.append(map(lambda (x, y): x + y, zip(accum_table[i - 2], full_table[i])))
sum_accum_table = map(sum, accum_table)

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

def get_digits(x):
	digits = [ ]
	while x > 0:
		digits.append(x % 10)
		x /= 10
	digits.reverse()
	return digits

def count_fairsquares(x):
	digits = get_digits(x)
	num_digits = len(digits)

	if num_digits == 0:
		return 0

	result = sum(sum_full_table[:(num_digits - 1)])
	digit_sum = 0
	for i, digit in enumerate(digits):
		if 2 * (i + 1) <= num_digits:
			rem_digits = num_digits - 2 * (i + 1)
		elif 2 * i + 1 == num_digits:
			rem_digits = 0
		else:
			break
		for j in xrange(1 if i == 0 else 0, digit):
			new_digit_sum = digit_sum + j * j
			if 2 * i + 1 != num_digits:
				new_digit_sum += j * j
			if new_digit_sum >= 10:
				break
			if rem_digits == 0:
				result += 1
			else:
				result += sum(accum_table[rem_digits - 1][:(10 - new_digit_sum)])
		digit_sum += digit * digit
		if 2 * i + 1 != num_digits:
			digit_sum += digit * digit
		if digit_sum >= 10:
			break
		if rem_digits == 0:
			new_digits = num_digits * [ 0 ]
			for j in xrange((num_digits + 1) / 2):
				new_digits[j] = digits[j]
				new_digits[num_digits - 1 - j] = digits[j]
			y = 0
			for elem in new_digits:
				y = 10 * y + elem
			if y <= x:
				result += 1

	return result

def solve_problem(x, y):
	p = int_sqrt(x)
	q = int_sqrt(y)
	if p * p < x:
		p += 1
	u = count_fairsquares(p - 1)
	v = count_fairsquares(q)
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
