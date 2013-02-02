#include <stdio.h>

#define MOD_PRIME 1000000007

#define MAX_M 1000

int x[MAX_M + 1];
int y[MAX_M + 1];
int z[MAX_M + 1];

void add_to_mod(int *lhs, int rhs)
{
	int value = *lhs + rhs;
	if (value >= MOD_PRIME)
		value -= MOD_PRIME;
	*lhs = value;
}

void subtract_from_mod(int *lhs, int rhs)
{
	int value = *lhs - rhs;
	if (value < 0)
		value += MOD_PRIME;
	*lhs = value;
}

void mul_by_mod(int *lhs, int rhs)
{
	long long value = *lhs;
	value *= rhs;
	*lhs = value % MOD_PRIME;
}

int pow_mod(int x, int k)
{
	long long result;
	long long base;

	result = 1;
	base = x;
	while (k > 0) {
		if (k & 1) {
			result *= base;
			result %= MOD_PRIME;
		}
		if (k > 1) {
			base *= base;
			base %= MOD_PRIME;
		}
		k >>= 1;
	}

	return (int) result;
}

void build_table()
{
	int value;
	int i;

	x[0] = 1;
	x[1] = 1;
	x[2] = 2;
	x[3] = 4;

	for (i = 4; i <= MAX_M; i++) {
		value = x[i - 1];
		add_to_mod(&value, x[i - 2]);
		add_to_mod(&value, x[i - 3]);
		add_to_mod(&value, x[i - 4]);
		x[i] = value;
	}
}

void solve_problem()
{
	int n, m;
	int i, j;
	int value;

	scanf("%d %d", &n, &m);

	for (i = 0; i <= m; i++)
		y[i] = pow_mod(x[i], n);

	z[0] = 1;
	for (i = 1; i <= m; i++) {
		z[i] = y[i];
		for (j = 1; j < i; j++) {
			value = z[j];
			mul_by_mod(&value, y[i - j]);
			subtract_from_mod(&z[i], value);
		}
	}
	printf("%d\n", z[m]);
}

int main()
{
	int num_tests;
	int i;

	build_table();

	scanf("%d", &num_tests);
	for (i = 0; i < num_tests; i++)
		solve_problem();

	return 0;
}
