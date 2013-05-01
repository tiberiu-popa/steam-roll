#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>

#define MAX_C 40

using namespace std;

long long comb[MAX_C + 1][MAX_C + 1];
double matrix[MAX_C + 1][MAX_C + 1];

void build_comb()
{
	for (int i = 0; i <= MAX_C; i++) {
		comb[i][0] = 1;
		for (int j = 1; j <= MAX_C; j++)
			comb[i][j] = comb[i - 1][j] + comb[i - 1][j - 1];
	}
}

int solve_problem(int num_case)
{
	double solutions[MAX_C + 1];
	int c, n;

	if (scanf("%d %d", &c, &n) != 2)
		return 1;

	for (int i = 0; i <= c; i++)
		for (int j = 0; j <= c; j++)
			if (j < i || j > i + n)
				matrix[i][j] = 0.0;
			else
				matrix[i][j] = (double) comb[i][n - (j - i)] / comb[c][n] * comb[c - i][j - i];

	solutions[c] = 0.0;
	for (int i = c - 1; i >= 0; i--) {
		double sum = 1.0;
		for (int j = i + 1; j <= c; j++)
			sum += matrix[i][j] * solutions[j];
		solutions[i] = sum / (1.0 - matrix[i][i]);
	}

	printf("Case #%d: %.6lf\n", num_case, solutions[0]);

	return 0;
}

int main(int argc, char *argv[])
{
	const char *filenames[] = {
		"collecting-cards.in",
		"collecting-cards.out",
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

	build_comb();

	if (scanf("%d", &num_tests) != 1)
		return 1;
	for (int i = 0; i < num_tests; i++)
		solve_problem(i + 1);

	return 0;
}
