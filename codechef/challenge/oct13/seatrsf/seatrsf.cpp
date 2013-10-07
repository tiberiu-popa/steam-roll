#include <algorithm>
#include <complex>
#include <limits>
#include <map>
#include <queue>
#include <vector>
#include <cmath>
#include <cstdio>

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

int submod(int x, int y)
{
	int diff = x - y;
	if (diff < 0)
		diff += MOD_PRIME;
	return diff;
}

int mulmod(int x, int y)
{
	long long prod = x;
	prod = (prod * y) % MOD_PRIME;
	return (int) prod;
}

int powmod(int x, int e)
{
	long long result = 1;
	long long base = x;

	if (e > 0) {
		while (true) {
			if (e & 1) {
				result = (result * base) % MOD_PRIME;
			}
			e >>= 1;
			if (e <= 0)
				break;
			base = (base * base) % MOD_PRIME;
		}
	}

	return (int) result;
}

int solve_problem()
{
	int n, m, q;

	n = read_unsigned_integer<int>();
	m = read_unsigned_integer<int>();
	q = read_unsigned_integer<int>();
	read_unsigned_integer<int>();

	int result = 0;
	if (m >= q) {
		int delta = powmod(q, n);
		result = powmod(q + 1, n);
		result = addmod(result, powmod(q - 1, n));
		result = submod(result, addmod(delta, delta));
		result = mulmod(result, m - q);
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
