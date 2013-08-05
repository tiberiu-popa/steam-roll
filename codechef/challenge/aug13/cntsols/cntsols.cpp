#include <algorithm>
#include <cstdio>

#define MAX_N 40
#define MOD_PRIME 1000000007

using namespace std;

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

int addmod(int x, int y)
{
	int sum = x + y;
	if (sum >= MOD_PRIME)
		sum -= MOD_PRIME;
	return sum;
}

int mulmod(int x, int y)
{
	long long prod = x;
	prod = (prod * y) % MOD_PRIME;
	return (int) prod;
}

int powmod(int x, int e, int p)
{
	long long result = 1;
	long long base = x;

	if (e > 0) {
		while (true) {
			if (e & 1) {
				result = (result * base) % p;
			}
			e >>= 1;
			if (e <= 0)
				break;
			base = (base * base) % p;
		}
	}

	return (int) result;
}

int solve_problem()
{
	int v[MAX_N];
	int u[MAX_N];
	int upper, d, m, n;

	upper = read_unsigned_integer<int>();
	d = read_unsigned_integer<int>();
	m = read_unsigned_integer<int>();
	n = read_unsigned_integer<int>();

	++upper;
	int q = upper / n;
	int r = upper % n;

	fill(v, v + n, 0);
	for (int i = 0; i < n; i++) {
		int p = (n > 1) ? powmod(i, d, n) : 0;
		v[p] = addmod(v[p], q + (i < r));
	}

	fill(u, u + n, 0);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++) {
			int s = (i + j) % n;
			u[s] = addmod(u[s], mulmod(v[i], v[j]));
		}

	int result = 0;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			if ((i + j) % n == m)
				result = addmod(result, mulmod(v[i], u[j]));
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
