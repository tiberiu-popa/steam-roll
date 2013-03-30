#include <algorithm>
#include <map>
#include <set>
#include <vector>
#include <cstdio>
#include <cstring>

#define MAX_N 60
#define MAX_M 60
#define MAX_X 5000

using namespace std;

int grid[MAX_N][MAX_M];
int right[MAX_N][MAX_M + 1];
int dp[MAX_N][MAX_M][MAX_M + 1][2];
int comb[MAX_N][MAX_M + 1];
int ans[MAX_N][MAX_X + 2];

void solve_problem()
{
	int n, m, x;
	char str[MAX_M + 3];

	fgets(str, sizeof(str), stdin);
	char *p = strtok(str, " ");
	n = atoi(p);
	p = strtok(NULL, " ");
	m = atoi(p);
	p = strtok(NULL, " ");
	x = atoi(p);

	for (int i = 0; i < n; i++) {
		fgets(str, sizeof(str), stdin);
		for (int j = 0; j < m; j++)
			grid[i][j] = (str[2 * j] == 'C');
	}

	for (int i = 0; i < n; i++) {
		right[i][0] = 0;
		for (int j = 0; j < m; j++)
			right[i][j + 1] = grid[i][j] + right[i][j];
	}
#if 0
	for (int i = 0; i < n; i++) {
		for (int j = 0; j <= m; j++)
			printf("%d ", right[i][j]);
		printf("\n");
	}
	printf("\n");
#endif
	for (int i = 0; i < n; i++) {
		for (int k = 1; k <= m; k++) {
			dp[i][0][k][grid[i][0]] = 1;
		}
		for (int j = 1; j < m; j++) {
			for (int k = 1; k <= m; k++) {
				int val = grid[i][j];
				int cand = dp[i][j - 1][k][val] + 1;
				if (k > 0)
					cand = max(cand, dp[i][j - 1][k - 1][val ^ 1] + 1);
				dp[i][j][k][val] = cand;
				cand = dp[i][j - 1][k][val ^ 1];
				if (k > 0)
					cand = max(cand, dp[i][j - 1][k - 1][val]);
				dp[i][j][k][val ^ 1] = cand;
			}
		}
	}

	for (int i = 0; i < n; i++)
		for (int k = 0; k <= m; k++)
			comb[i][k] = max(dp[i][m - 1][k][0], dp[i][m - 1][k][1]);

	for (int k = 0; k <= m; k++)
		ans[0][k] = comb[0][k];
	for (int i = 1; i < n; i++) {
		for (int k = 0; k == 0 || ans[i - 1][k] != 0; k++)
			for (int l = 0; l <= m && k + l <= x; l++) {
				int cand = ans[i - 1][k] + comb[i][l];
				if (cand > ans[i][k + l])
					ans[i][k + l] = cand;
			}
	}
#if 0
	for (int i = 0; i < n; i++) {
		for (int k = 0; k <= m; k++)
			printf("%d ", comb[i][k]);
		printf("\n");
	}
	printf("\n");

	for (int i = 0; i < n; i++) {
		for (int k = 0; k <= x; k++)
			printf("%d ", ans[i][k]);
		printf("\n");
	}
	printf("\n");
#endif
	printf("%d\n", ans[n - 1][x]);
}

int main()
{
	if (freopen("gradinarit.in", "rt", stdin) == NULL)
		return 1;
	if (freopen("gradinarit.out", "wt", stdout) == NULL)
		return 1;

	solve_problem();

	return 0;
}
