#include <algorithm>
#include <limits>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#define MOD_PRIME 1000000007
#define MAX_K 316

using namespace std;

// dp[i][j][k] = i intervals on {0, 1, ..., j-1} such that the first k are
// [0, r0], [1, r1], ..., [k-1, r(k-1)], ri >= k for all i and
// there is one i s.t. li = x - m + i when the quantity is >= 0
int dp[2][MAX_K + 1][MAX_K + 2];

int addmod(int x, int y)
{
	int sum = x + y;
	if (sum >= MOD_PRIME)
		sum -= MOD_PRIME;
	return sum;
}

int mulmod(int x, int y, int p)
{
	long long prod = x;
	prod = (prod * y) % p;
	return (int) prod;
}

void add_to_mod(int &x, int y)
{
	x = addmod(x, y);
}

int run_dp(int n, int m, int x)
{
	int result = 0;

	if (n > m)
		return result;

	int next = 0;
	dp[next][0][0] = 1;
	for (int i = 1; i <= m; i++) {
		int prev = next;
		next ^= 1;

		for (int j = n; j > 0; j--)
			for (int k = j; k >= 0; k--) {
				dp[next][j][k] = addmod(dp[prev][j - 1][k], dp[next][j][k + 1]);
				if (m - i != x - k) {
					int s = dp[prev][j][k];
					if (k > 0)
						add_to_mod(s, dp[next][j - 1][k - 1]);
					add_to_mod(dp[next][j][k], s);
				}
			}
		dp[next][0][0] = (m - i) > x;
	}

	result = dp[next][n][0];
	for (int i = 2; i <= n; i++)
		result = mulmod(result, i, MOD_PRIME);

	return result;
}

int solve_problem()
{
	int n, m, x;

	if (scanf("%d %d %d", &n, &m, &x) != 3)
		return 1;
	--x;

	int result = run_dp(n, m, x);
	printf("%d\n", result);

	return 0;
}

int main()
{
	solve_problem();
	return 0;
}
