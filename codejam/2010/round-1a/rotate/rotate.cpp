#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>

#define MAX_N 50

using namespace std;

char grid[MAX_N][MAX_N + 1];
int dp[4][2][MAX_N][MAX_N];

int dz[][2] = {
	{ -1, 0 },
	{ 0, -1 },
	{ -1, -1 },
	{ -1, 1 },
};
char player[] = { 'R', 'B' };

const char * result_to_str(bool result[])
{
	if (result[0])
		return result[1] ? "Both" : "Red";
	else
		return result[1] ? "Blue" : "Neither";
}

int solve_problem(int num_case)
{
	int n, k;
	bool result[] = { false, false };

	if (scanf("%d %d", &n, &k) != 2)
		return 1;

	for (int i = 0; i < n; i++)
		if (scanf("%s", grid[i]) != 1)
			return 1;

	for (int i = 0; i < n / 2; i++)
		for (int j = i; j < n - 1 - i; j++) {
			char swp = grid[i][j];
			grid[i][j] = grid[n - 1 - j][i];
			grid[n - 1 - j][i] = grid[n - 1 - i][n - 1 - j];
			grid[n - 1 - i][n - 1 - j] = grid[j][n - 1 - i];
			grid[j][n - 1 - i] = swp;
		}

	for (int j = 0; j < n; j++) {
		int k = n - 1;
		for (int i = n - 1; i >= 0; i--)
			if (grid[i][j] != '.') {
				swap(grid[i][j], grid[k][j]);
				k--;
			}
	}

	for (int p = 0; p < 2; p++)
		for (int l = 0; l < 4; l++)
			for (int i = 0; i < n; i++)
				for (int j = 0; j < n; j++) {
					dp[l][p][i][j] = 0;
					if (player[p] == grid[i][j]) {
						dp[l][p][i][j]++;
						if (i != 0 || j != 0) {
							int ni = i + dz[l][0];
							int nj = j + dz[l][1];
							if (ni >= 0 && ni < n && nj >= 0 && nj < n)
								dp[l][p][i][j] += dp[l][p][ni][nj];
						}
						if (dp[l][p][i][j] >= k) {
							result[p] = true;
							l = 3;
							i = n - 1;
							break;
						}
					}
				}

	printf("Case #%d: %s\n", num_case, result_to_str(result));

	return 0;
}

int main(int argc, char *argv[])
{
	const char *filenames[] = {
		"rotate.in",
		"rotate.out",
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
