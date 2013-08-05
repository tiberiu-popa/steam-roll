#include <cstdio>

#define MAX_N 100000
#define NUM_PRIMES 25

using namespace std;

const int primes[NUM_PRIMES] = {
	2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41,
	43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97,
};
int a[MAX_N];
int v[NUM_PRIMES][MAX_N + 1];

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

int mulmod(int x, int y, int p)
{
	long long prod = x;
	prod = (prod * y) % p;
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
	int n, t;

	n = read_unsigned_integer<int>();
	for (int i = 0; i < n; i++)
		a[i] = read_unsigned_integer<int>();

	for (int j = 0; j < NUM_PRIMES; j++)
		v[j][0] = 0;
	for (int i = 0; i < n; i++) {
		int x = a[i];
		for (int j = 0; j < NUM_PRIMES; j++) {
			int p = primes[j];
			int e = 0;
			while (x % p == 0) {
				x /= p;
				e++;
			}
			v[j][i + 1] = v[j][i] + e;
		}
	}

	t = read_unsigned_integer<int>();
	for (int i = 0; i < t; i++) {
		int l = read_unsigned_integer<int>();
		int r = read_unsigned_integer<int>();
		int m = read_unsigned_integer<int>();
		--l;
		--r;
		int result = 1;
		for (int j = 0; j < NUM_PRIMES; j++) {
			int p = primes[j];
			int e = v[j][r + 1] - v[j][l];
			result = mulmod(result, powmod(p, e, m), m);
		}
		printf("%d\n", result);
	}

	return 0;
}

int main()
{
	solve_problem();
	return 0;
}
