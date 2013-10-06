#include <algorithm>
#include <complex>
#include <limits>
#include <map>
#include <queue>
#include <vector>
#include <cmath>
#include <cstdio>

#define MAX_N 100000

using namespace std;

int a[MAX_N];
int b[MAX_N];

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
		a[i] = read_unsigned_integer<int>();

	for (int i = 0; i < n; i++)
		b[i] = read_unsigned_integer<int>();

	sort(a, a + n);
	sort(b, b + n);

	int num_ones = 0, num_twos = 0;
	for (int i = 0; i < n; i++) {
		if (b[i] == 1)
			num_ones++;
		else if (b[i] == 2)
			num_twos++;
		else
			break;
	}

	long long result = 0;
	for (int i = 0, j = 0; i < n; i++) {
		while (j < n && b[j] <= a[i])
			j++;
		if (a[i] > 1) {
			if (a[i] == 2) {
				while (j < n && b[j] <= 3)
					j++;
				int k = j;
				while (k < n && b[k] <= 4)
					k++;
				result += n - k + num_ones;
			} else {
				result += n - j + num_ones;
				if (a[i] == 3)
					result += num_twos;
			}
		}
	}
	printf("%.7lf\n", (double) result / n);

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
