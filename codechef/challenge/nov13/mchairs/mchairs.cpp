#include <algorithm>
#include <cstdio>
#include <cstring>

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

int addmod(int x, int y, int p)
{
	long long sum = x;
	sum += y;
	if (sum >= p)
		sum -= p;
	return (int) sum;
}

int submod(int x, int y, int p)
{
	long long diff = x;
	diff -= y;
	if (diff < 0)
		diff += p;
	return (int) diff;
}

int mulmod(int x, int y, int p)
{
	long long prod = x;
	prod = (prod * y) % p;
	return (int) prod;
}

int sqrmod(int x, int p)
{
	return mulmod(x, x, p);
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

int invmod(int x, int p)
{
	int result[2][2] = { {1, 0}, {0, 1} };
	int base[2][2] = { {0, 1}, {1, 0}};

	int y = x % p;
	x = p;
	while (y > 0) {
		int q = x / y;
		int r = x % y;
		base[1][1] = -q;
		int tmp[2][2] = { {0, 0}, {0, 0} };
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 2; j++)
				for (int k = 0; k < 2; k++)
					tmp[i][j] += result[i][k] * base[k][j];
		memcpy(result, tmp, sizeof(result));
		x = y;
		y = r;
	}

	int inv = result[1][0];
	if (inv < 0)
		inv += p;
	return inv;
}

int solve_problem()
{
	int n;

	n = read_unsigned_integer<int>();

	int result = powmod(2, n, MOD_PRIME);
	result = submod(result, 1, MOD_PRIME);
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
