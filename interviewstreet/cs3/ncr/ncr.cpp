#include <cstdio>
#include <cstdlib>

#define MOD_NUMBER 142857

// Chinese Remainder Theorem FTW
int crt[27][11][13][37];
int comb27[730][730];
int comb11[11][730];
int comb13[13][730];
int comb37[37][730];
int g27[27][27][9][9];

int add_modulo(int x, int y, int n)
{
	int sum = x + y;
	if (sum >= n)
		sum -= n;
	return sum;
}

int mul_modulo(int x, int y, int n)
{
	long long prod = x;
	prod *= y;
	return prod % n;
}

void build_crt()
{
	for (int i = 0; i < 142857; i++)
		crt[i % 27][i % 11][i % 13][i % 37] = i;
}

void build_limit_comb(int comb[][730], int limit, int p)
{
	comb[0][0] = 1;
	for (int i = 1; i <= limit; i++) {
		comb[i][0] = 1;
		for (int j = 1; j <= i; j++)
			comb[i][j] = add_modulo(comb[i - 1][j - 1], comb[i - 1][j], p);
	}
}

void build_comb(int comb[][730], int p)
{
	build_limit_comb(comb, p - 1, p);
}

void build_g27()
{
	for (int i = 0; i < 27; i++)
		for (int j = 0; j < 27; j++)
			for (int x = 0; x < 9; x++)
				for (int y = 0; y < 9; y++) {
					// Use x + 9 instead of x, such that u >= v
					int u = 27 * (x + 9) + i;
					int v = 27 * y + j;
					if (u >= v) {
						int p = comb27[u][v];
						int q = comb27[x + 9][y];
						for (int r = 26; r >= 0; r--)
							if ((q * r - p) % 27 == 0) {
								g27[i][j][x][y] = r;
								break;
							}
					}
				}
}

void init_ncr()
{
	build_crt();
	build_limit_comb(comb27, 27 * 27, 27);
	build_comb(comb11, 11);
	build_comb(comb13, 13);
	build_comb(comb37, 37);
	build_g27();
}

int comb_modulo_prime(int comb[][730], int p, int n, int r)
{
	if (n < r)
		return 0;

	int prod = 1;
	while (n > 0) {
		int u = n % p;
		int v = r % p;
		n /= p;
		r /= p;
		prod = mul_modulo(prod, comb[u][v], p);
	}
	return prod;
}

int comb_modulo_27(int n, int r)
{
	if (n < r)
		return 0;

	int prod = 1;
	while (n > 0) {
		int u = n % 27;
		int v = r % 27;
		n /= 27;
		r /= 27;
		prod = mul_modulo(prod, g27[u][v][n % 9][r % 9], 27);
	}
	return prod;
}

int get_answer(int n, int r)
{
	int a = comb_modulo_27(n, r);
	int b = comb_modulo_prime(comb11, 11, n, r);
	int c = comb_modulo_prime(comb13, 13, n, r);
	int d = comb_modulo_prime(comb37, 37, n, r);
	return crt[a][b][c][d];
}

void solve_problem()
{
	int n, r;

	if (scanf("%d %d", &n, &r) != 2)
		exit(EXIT_FAILURE);

	printf("%d\n", get_answer(n, r));
}

int main()
{
	int num_tests;

	if (scanf("%d", &num_tests) != 1)
		exit(EXIT_FAILURE);

	init_ncr();

	for (int i = 0; i < num_tests; i++)
		solve_problem();

	return 0;
}
