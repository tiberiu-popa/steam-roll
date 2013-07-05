#include <algorithm>
#include <map>
#include <cmath>
#include <cstdio>
#include <cstring>

#define MAX_S 1000

using namespace std;

double comb[MAX_S + 1][MAX_S + 1];

void build_comb()
{
	comb[0][0] = 1.0;
	for (int i = 1; i <= MAX_S; i++) {
		comb[i][0] = comb[i - 1][0] / 2;
		for (int j = 1; j <= i; j++)
			comb[i][j] = (comb[i - 1][j] + comb[i - 1][j - 1]) / 2;
	}
}

int solve_problem()
{
	int s, n, m, k;

	if (scanf("%d %d %d %d", &s, &n, &m, &k) != 4)
		return 1;

	double result = 0.0;
	for (int i = max(k + 1, n - s + m); i <= m && i <= n; i++) {
		result += comb[m - 1][i - 1] * comb[s - m][n - i] / comb[s][n];
	}
	printf("%.8e\n", .5 * result * s / n);
	return 0;
}

int main()
{
	int num_tests;

	build_comb();

	if (scanf("%d", &num_tests) != 1)
		return 1;
	for (int i = 0; i < num_tests; i++) {
		int ret = solve_problem();
		if (ret != 0) {
			fprintf(stderr, "Read error at case #%d\n", i + 1);
			break;
		}
	}

	return 0;
}
