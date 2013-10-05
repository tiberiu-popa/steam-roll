#include <algorithm>
#include <complex>
#include <limits>
#include <map>
#include <queue>
#include <vector>
#include <cmath>
#include <cstdio>

#define MAX_N 50

using namespace std;

int h[MAX_N];
int d[MAX_N];
int x[MAX_N];
int y[MAX_N];

int skip_whitespace()
{
	int ch;
	while (true) {
		ch = getchar_unlocked();
		if (ch != ' ' && ch != '\n')
			break;
	}
	return ch;
}

template<typename T>
T read_unsigned_integer()
{
	T result = (T) 0;
	int ch = skip_whitespace();
	while (ch >= '0' && ch <= '9') {
		result = 10 * result + (ch - '0');
		ch = getchar_unlocked();
	}
	return result;
}

template<typename T>
T read_signed_integer()
{
	T result = (T) 0;
	bool flip = false;
	int ch = skip_whitespace();
	if (ch == '-') {
		flip = true;
		ch = skip_whitespace();
	}
	while (ch >= '0' && ch <= '9') {
		result = 10 * result + (ch - '0');
		ch = getchar_unlocked();
	}
	return flip ? -result : result;
}

int solve_problem()
{
	int n;

	n = read_unsigned_integer<int>();

	for (int i = 0; i < n; i++)
		h[i] = read_unsigned_integer<int>();

	for (int i = 0; i < n; i++)
		d[i] = read_unsigned_integer<int>();

	for (int i = 0; i < n; i++)
		y[i] = d[i] - h[i];

	int result = -1;
	if (n == 1) {
		if (y[0] <= 0)
			result = -y[0];
	} else if (n == 2) {
		if (y[0] + y[1] == 0)
			result = max(y[0], y[1]);
	} else {
		int alpha = 3 - n;
		int lower = 2 * n - 4;
		for (int i = 0; i < n; i++) {
			x[i] = 0;
			for (int j = 0; j < n; j++)
				x[i] += ((i == j) ? alpha : 1) * y[j];
		}

		bool valid = true;
		for (int i = 0; i < n; i++)
			if (x[i] < 0 || x[i] % lower != 0) {
				valid = false;
				break;
			}

		if (valid) {
			result = 0;
			for (int i = 0; i < n; i++)
				result += x[i];
			result /= lower;
		}
	}
	printf("%d\n", result);

	return 0;
}

int main()
{
	int num_tests;

	num_tests = read_unsigned_integer<int>();
	for (int i = 0; i < num_tests; i++)
		solve_problem();

	return 0;
}
