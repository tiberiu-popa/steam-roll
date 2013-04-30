#include <algorithm>
#include <vector>
#include <cmath>
#include <cstdio>
#include <cstring>

#define MATRIX_SIZE 2
#define MOD_NUMBER 1000

using namespace std;

typedef int matrix_t[MATRIX_SIZE][MATRIX_SIZE];

void matrix_assign(matrix_t x, const matrix_t y)
{
	memcpy(x, y, MATRIX_SIZE * MATRIX_SIZE * sizeof(**x));
}

void matrix_mod(matrix_t x, int mod)
{
	for (int i = 0; i < MATRIX_SIZE; i++)
		for (int j = 0; j < MATRIX_SIZE; j++) {
			x[i][j] %= mod;
			if (x[i][j] < 0)
				x[i][j] += mod;
		}
}

// mod <= 1000
void matrix_mulmod(matrix_t x, const matrix_t y, const matrix_t z, int mod)
{
	for (int i = 0; i < MATRIX_SIZE; i++)
		for (int j = 0; j < MATRIX_SIZE; j++) {
			x[i][j] = 0;
			for (int k = 0; k < MATRIX_SIZE; k++)
				x[i][j] += y[i][k] * z[k][j];
			x[i][j] %= mod;
		}
}

void matrix_powmod(matrix_t x, const matrix_t y, int power, int mod)
{
	matrix_t base[2], result[2];
	int b_index = 0;
	int r_index = 0;

	matrix_assign(base[b_index], y);
	for (int i = 0; i < MATRIX_SIZE; i++)
		for (int j = 0; j < MATRIX_SIZE; j++)
			result[r_index][i][j] = (i == j);

	for (; power > 0; power >>= 1) {
		if (power & 1) {
			int new_r_index = r_index ^ 1;
			matrix_mulmod(result[new_r_index], result[r_index], base[b_index], mod);
			r_index = new_r_index;
		}
		int new_b_index = b_index ^ 1;
		matrix_mulmod(base[new_b_index], base[b_index], base[b_index], mod);
		b_index = new_b_index;
	}
	matrix_assign(x, result[r_index]);
}

int solve_problem(int num_case)
{
	matrix_t x = { { 0, 1 }, { -4, 6 } };
	matrix_t result;
	int n;

	if (scanf("%d", &n) != 1)
		return 1;

	matrix_mod(x, MOD_NUMBER);
	matrix_powmod(result, x, n, MOD_NUMBER);

	int nth_value = 2 * result[0][0] + 6 * result[0][1];
	printf("Case #%d: %03d\n", num_case, (nth_value + MOD_NUMBER - 1) % MOD_NUMBER);

	return 0;
}

int main(int argc, char *argv[])
{
	const char *filenames[] = {
		"numbers.in",
		"numbers.out",
	};
	int num_tests;

	for (int i = 1; i < argc && i <= 2; i++)
		filenames[i - 1] = argv[i];

	if (freopen(filenames[0], "rt", stdin) == NULL) {
		fprintf(stderr, "Could not reopen stdin\n");
		return 1;
	}
	if (freopen(filenames[1], "wt", stdout) == NULL) {
		fprintf(stderr, "Could not reopen stdout\n");
		return 1;
	}

	if (scanf("%d", &num_tests) != 1)
		return 1;
	for (int i = 0; i < num_tests; i++)
		solve_problem(i + 1);

	return 0;
}
