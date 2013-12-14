#include <algorithm>
#include <limits>
#include <cstdio>

#define MAX_N 100000

using namespace std;

int v[MAX_N];
long long sum[MAX_N + 1];
long long u[MAX_N];
long long w[MAX_N];

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

char read_character()
{
	return skip_whitespace();
}

int solve_problem()
{
	int n, k;

	n = read_unsigned_integer<int>();
	k = read_unsigned_integer<int>();

	for (int i = 0; i < n; i++)
		v[i] = read_signed_integer<int>();

	sum[0] = 0;
	for (int i = 0; i < n; i++)
		sum[i + 1] = sum[i] + v[i];

	long long min_val = sum[0];
	for (int i = 1; i <= n; i++) {
		u[i - 1] = sum[i] - min_val;
		if (sum[i] < min_val)
			min_val = sum[i];
	}

	long long max_val = sum[n];
	for (int i = n - 1; i >= 0; i--) {
		w[i] = max_val - sum[i];
		if (sum[i] > max_val)
			max_val = sum[i];
	}

	for (int i = 1; i < n; i++)
		if (u[i - 1] > u[i])
			u[i] = u[i - 1];

	for (int i = n - 2; i >= 0; i--)
		if (w[i + 1] > w[i])
			w[i] = w[i + 1];

	long long result = numeric_limits<long long>::min();
	for (int i = k + 1; i < n; i++) {
		long long cand = w[i] + u[i - k - 1];
		if (cand > result)
			result = cand;
	}
	printf("%lld\n", result);

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
