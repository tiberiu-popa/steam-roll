#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>

using namespace std;

template<class T>
T gcd(T x, T y)
{
	if (x < y)
		swap(x, y);
	while (y > 0) {
		T r = x % y;
		x = y;
		y = r;
	}
	return x;
}

void print_answer(long long p, long long q)
{
	long long g = gcd(p, q);
	cout << p / g << '/' << q / g << endl;
}

void solve_problem()
{
	long long a, b, c;

	if (scanf("%lld %lld %lld", &a, &b, &c) != 3)
		exit(EXIT_FAILURE);

	if (a > b)
		swap(a, b);

	if (c <= a) {
		print_answer(c * c, 2 * a * b);
	} else if (c <= b) {
		print_answer(a * a + 2 * a * (c - a), 2 * a * b);
	} else {
		int diff = a + b - c;
		if (diff < 0)
			diff = 0;
		print_answer(2 * a * b - diff * diff, 2 * a * b);
	}
}

int main()
{
	int num_tests;

	if (scanf("%d", &num_tests) != 1)
		exit(EXIT_FAILURE);
	for (int i = 0; i < num_tests; i++)
		solve_problem();

	return 0;
}
