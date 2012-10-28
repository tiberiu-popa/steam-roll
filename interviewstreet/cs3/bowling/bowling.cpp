#include <algorithm>
#include <queue>
#include <set>
#include <vector>
#include <cstdio>
#include <cstdlib>

#define MAX_N 100
#define MAX_M (30 * MAX_N + 1)
#define MAX_POINTS 10
#define MOD_NUMBER 1000000007

#define dprintf(fmt, ...) ((void) 0)
//#define dprintf printf

using namespace std;

int dp[MAX_N][MAX_M][4];

int add_modulo(int x, int y)
{
	int sum = x + y;
	if (sum >= MOD_NUMBER)
		sum -= MOD_NUMBER;
	return sum;
}

void add_to_modulo(int &x, int y)
{
	x = add_modulo(x, y);
}

int mul_modulo(int x, int y)
{
	long long prod = x;
	prod *= y;
	return prod % MOD_NUMBER;
}

void run_dp(int n)
{
	int m = 30 * n;

	for (int i = 0; i < n; i++)
		for (int j = 0; j <= m; j++)
			for (int k = 0; k < 4; k++)
				dp[i][j][k] = 0;

	for (int j = 0; j < MAX_POINTS; j++)
		dp[0][j][0] = j + 1;
	dp[0][MAX_POINTS][1] = 10;
	dp[0][MAX_POINTS][2] = 1;

	for (int i = 1; i < n; i++)
		for (int j = 0; j <= m; j++) {
			int term;
			for (int k = 0; k < MAX_POINTS && j + k <= m; k++) {
				term = mul_modulo(dp[i - 1][j][0], k + 1);
				add_to_modulo(dp[i][j + k][0], term);
				for (int l = 0; l <= k && j + k + l <= m; l++)
					add_to_modulo(dp[i][j + k + l][0], dp[i - 1][j][1]);
				if (j + 2 * k <= m) {
					term = mul_modulo(dp[i - 1][j][2], k + 1);
					add_to_modulo(dp[i][j + 2 * k][0], term);
					term = dp[i - 1][j][3];
					for (int l = 0; l <= k && j + 2 * k + l <= m; l++)
						add_to_modulo(dp[i][j + 2 * k + l][0], term);
				}
			}
			if (j + MAX_POINTS <= m) {
				term = mul_modulo(dp[i - 1][j][0], MAX_POINTS);
				add_to_modulo(dp[i][j + MAX_POINTS][1], term);
				for (int l = 0; l < MAX_POINTS && j + MAX_POINTS + l <= m; l++)
					add_to_modulo(dp[i][j + MAX_POINTS + l][1], dp[i - 1][j][1]);
				if (j + 2 * MAX_POINTS <= m) {
					term = mul_modulo(dp[i - 1][j][2], MAX_POINTS);
					add_to_modulo(dp[i][j + 2 * MAX_POINTS][1], term);
					term = dp[i - 1][j][3];
					for (int l = 0; l < MAX_POINTS && j + 2 * MAX_POINTS + l <= m; l++)
						add_to_modulo(dp[i][j + 2 * MAX_POINTS + l][1], term);
				}

				add_to_modulo(dp[i][j + MAX_POINTS][2], dp[i - 1][j][0]);
				if (j + 2 * MAX_POINTS <= m) {
					add_to_modulo(dp[i][j + 2 * MAX_POINTS][2], dp[i - 1][j][1]);
					term = dp[i - 1][j][2];
					add_to_modulo(dp[i][j + 2 * MAX_POINTS][3], term);
					term = dp[i - 1][j][3];
					if (j + 3 * MAX_POINTS <= m)
						add_to_modulo(dp[i][j + 3 * MAX_POINTS][3], term);
				}
			}
		}
}

void print_answer(int n, int m)
{
	int sum = dp[n - 1][m][0];
	dprintf("sum=%d\n", sum);
	for (int j = m; j >= m - MAX_POINTS && j >= 0; j--)
		add_to_modulo(sum, dp[n - 1][j][1]);
	dprintf("sum=%d\n", sum);
	for (int j = 0; j <= m; j++) {
		for (int p = 0; p <= MAX_POINTS; p++)
			for (int q = 0; q <= MAX_POINTS; q++) {
				if (p < MAX_POINTS && p + q > MAX_POINTS)
					break;
				int cand = j + p + q;
				if (cand == m) {
					add_to_modulo(sum, dp[n - 1][j][2]);
					if (dp[n - 1][j][2] != 0)
						dprintf("(A) new=%d j=%d\n", dp[n - 1][j][2], j);
				}
				cand = j + 2 * p + q;
				if (cand == m) {
					add_to_modulo(sum, dp[n - 1][j][3]);
					if (dp[n - 1][j][3] != 0)
						dprintf("(B) new=%d j=%d\n", dp[n - 1][j][3], j);
				}
			}
	}

	printf("%d\n", sum);
}

void solve_problem()
{
	int n, m;

	if (scanf("%d %d", &n, &m) != 2)
		exit(EXIT_FAILURE);

	print_answer(n, m);
}

int main()
{
	int num_tests;

	if (scanf("%d", &num_tests) != 1)
		exit(EXIT_FAILURE);

	run_dp(MAX_N);
	for (int i = 0; i < num_tests; i++)
		solve_problem();

	return 0;
}
