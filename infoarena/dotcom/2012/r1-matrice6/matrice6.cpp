#include <algorithm>
#include <cstdio>
#include <cstdlib>

#define MOD_PRIME 666013
#define MAX_N 5
#define MATRIX_SIZE 81

using namespace std;

int table[MATRIX_SIZE][MATRIX_SIZE];
int conv_left[MATRIX_SIZE][MATRIX_SIZE];
int conv_diff[MATRIX_SIZE][MATRIX_SIZE];
int conv_right[MATRIX_SIZE][MATRIX_SIZE];

void read_input(const char *filename, int &n, int &m, int &y)
{
	FILE *f;
	int x;

	f = fopen(filename, "rt");
	if (f == NULL) {
		fprintf(stderr, "Could not open file: %s\n", filename);
		exit(EXIT_FAILURE);
	}

	if (fscanf(f, "%d %d %d %d", &n, &m, &x, &y) != 4)
		exit(EXIT_FAILURE);

	fclose(f);
}

void write_output(const char *filename, int result)
{
	FILE *f;

	f = fopen(filename, "wt");
	if (f == NULL) {
		fprintf(stderr, "Could not open file: %s\n", filename);
		exit(EXIT_FAILURE);
	}

	fprintf(f, "%d\n", result);
	fclose(f);
}

void add_product_mod(int &result, int x, int y)
{
	long long accum = x;
	accum *= y;
	accum += result;
	result = accum % MOD_PRIME;
}

int powint(int x, int n)
{
	int result = 1;
	int base = x;
	while (n > 0) {
		if (n & 1)
			result = result * base;
		base = base * base;
		n >>= 1;
	}
	return result;
}

void matrix_identity(int x[MATRIX_SIZE][MATRIX_SIZE], int size)
{
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			x[i][j] = (i == j);
}

void matrix_assign(int x[MATRIX_SIZE][MATRIX_SIZE],
	int y[MATRIX_SIZE][MATRIX_SIZE], int size)
{
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			x[i][j] = y[i][j];
}

void matrix_mulmod(int c[MATRIX_SIZE][MATRIX_SIZE],
	int a[MATRIX_SIZE][MATRIX_SIZE],
	int b[MATRIX_SIZE][MATRIX_SIZE],
	int size)
{
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++) {
			long long elem = 0;
			for (int k = 0; k < size; k++) {
				long long prod = a[i][k];
				prod *= b[k][j];
				elem += prod;
			}
			c[i][j] = elem % MOD_PRIME;
		}
}

void matrix_powmod(int result[MATRIX_SIZE][MATRIX_SIZE], int size, int n)
{
	int partial_result[MATRIX_SIZE][MATRIX_SIZE];
	int first_base[MATRIX_SIZE][MATRIX_SIZE];
	int second_base[MATRIX_SIZE][MATRIX_SIZE];
	int (*p)[MATRIX_SIZE] = result;
	int (*q)[MATRIX_SIZE] = partial_result;
	int (*base)[MATRIX_SIZE] = first_base;
	int (*temp_base)[MATRIX_SIZE] = second_base;

	matrix_identity(p, size);
	matrix_assign(base, table, size);

	while (n > 0) {
		if (n & 1) {
			matrix_mulmod(q, p, base, size);
			swap(p, q);
		}
		matrix_mulmod(temp_base, base, base, size);
		swap(base, temp_base);
		n >>= 1;
	}
	if (p != result)
		matrix_assign(result, partial_result, size);
}

int solve_problem(int n, int m, int y)
{
	int result = 0;
	int table_size = powint(3, n - 1);

	for (int i = 0; i < table_size; i++) {
		int diff[MAX_N - 1];
		int crt[MAX_N], next[MAX_N];
		for (int j = 0, tmp = i; j < n - 1; j++, tmp /= 3) {
			int r = tmp % 3;
			diff[j] = (r == 2) ? -1 : r;
		}
		crt[0] = 0;
		for (int j = 1; j < n; j++)
			crt[j] = crt[j - 1] + diff[j - 1];
		for (int k = 0; k < 3 * table_size; k++) {
			for (int j = 0, tmp = k; j < n; j++, tmp /= 3) {
				int r = tmp % 3;
				next[j] = crt[j] + ((r == 2) ? -1 : r);
			}
			int index = 0;
			for (int j = n; j > 1;) {
				--j;
				int gap = next[j] - next[j - 1];
				if (gap >= -1 && gap <= 1) {
					index = 3 * index + ((gap >= 0) ? gap : 2);
				} else {
					index = -1;
					break;
				}
			}
			if (index >= 0)
				table[i][index]++;
		}
	}

	if (y > (m + 1) / 2)
		y = m + 1 - y;
	matrix_powmod(conv_left, table_size, y - 1);
	matrix_powmod(conv_diff, table_size, m + 1 - 2 * y);
	matrix_mulmod(conv_right, conv_left, conv_diff, table_size);

	for (int i = 0; i < table_size; i++) {
		int p = 0;
		for (int j = 0; j < table_size; j++) {
			p += conv_left[i][j];
		}
		p %= MOD_PRIME;
		int q = 0;
		for (int j = 0; j < table_size; j++) {
			q += conv_right[i][j];
		}
		q %= MOD_PRIME;
		add_product_mod(result, p, q);
	}

	return result;
}

int main()
{
	int n, m, y;
	int result;

	read_input("matrice6.in", n, m, y);
	result = solve_problem(n, m, y);
	write_output("matrice6.out", result);

	return 0;
}
