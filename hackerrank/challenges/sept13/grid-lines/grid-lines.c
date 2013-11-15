#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MOD_PRIME 1000000007
#define MAX_N 3000

int bincoef[MAX_N + 1][MAX_N + 1];

int skip_whitespace()
{
	int ch;
	while (1) {
		ch = getchar_unlocked();
		if (ch != ' ' && ch != '\n')
			break;
	}
	return ch;
}

int read_unsigned_integer_int()
{
	int result = 0;
	int ch = skip_whitespace();
	while (ch >= '0' && ch <= '9') {
		result = 10 * result + (ch - '0');
		ch = getchar_unlocked();
	}
	return result;
}

int read_signed_integer_int()
{
	int result = 0;
	int flip = 0;
	int ch = skip_whitespace();
	if (ch == '-') {
		flip = 1;
		ch = skip_whitespace();
	}
	while (ch >= '0' && ch <= '9') {
		result = 10 * result + (ch - '0');
		ch = getchar_unlocked();
	}
	return flip ? -result : result;
}

int gcd(int x, int y)
{
	int r;

	if (x < 0)
		x = -x;
	if (y < 0)
		y = -y;
	if (x < y) {
		r = x;
		x = y;
		y = r;
	}
	while (y > 0) {
		r = x % y;
		x = y;
		y = r;
	}
	return x;
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

int count_all(int n, int m, int k)
{
	int result = 0;
	int i, j;

	for (i = 0; i <= n; i++)
		for (j = (i == 0); j <= m; j++) {
			int coef = bincoef[gcd(i, j) - 1][k - 2];
			int delta = mulmod((n + 1 - i) * (m + 1 - j), coef, MOD_PRIME);
			if (i != 0 && j != 0)
				delta = addmod(delta, delta, MOD_PRIME);
			result = addmod(result, delta, MOD_PRIME);
		}

	return result;
}

int solve_problem()
{
	int n, m, k;
	int max_dim;
	int i, j;

	n = read_unsigned_integer_int();
	m = read_unsigned_integer_int();
	k = read_unsigned_integer_int();

	max_dim = (n > m) ? n : m;
	for (i = 0; i <= max_dim; i++) {
		bincoef[i][0] = 1;
		for (j = 1; j <= i; j++)
			bincoef[i][j] = addmod(bincoef[i - 1][j], bincoef[i - 1][j - 1], MOD_PRIME);
	}

	int result = count_all(n, m, k);
	if (n >= 2 && m >= 2)
		result = submod(result, count_all(n - 2, m - 2, k), MOD_PRIME);
	printf("%d\n", result);

	return 0;
}

int main()
{
	solve_problem();
	return 0;
}
