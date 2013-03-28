#include <algorithm>
#include <map>
#include <set>
#include <vector>
#include <cstdio>

#define MAX_N 70

using namespace std;

int dp[MAX_N + 1][MAX_N];
int sums[MAX_N + 1][MAX_N];
int comb[MAX_N + 1][MAX_N + 1];

int add_modulo(int x, int y, int mod)
{
	int result = x + y;
	if (result >= mod)
		result -= mod;
	return result;
}

long long mul_modulo(long long x, long long y, int mod)
{
	return (x * y) % mod;
}

void build_comb(int n, int p)
{
	for (int i = 0; i <= n; i++) {
		comb[i][0] = 1;
		for (int j = 1; j <= i; j++)
			comb[i][j] = add_modulo(comb[i - 1][j], comb[i - 1][j - 1], p);
	}
}

void solve_problem()
{
	int n, x, p;

	if (scanf("%d %d %d", &n, &x, &p) != 3)
		return;
	--x;

	build_comb(n, p);

	dp[1][0] = 1;
	sums[1][0] = dp[1][0];

	for (int i = 2; i <= n; i++) {
		for (int j = 0; j < i; j++) {
			int d = i - j - 1;
			int value = 0;
			for (int s = 1; s <= i / 2; s++) {
				int term = 0;
				int t = i - s;
				for (int k = max(0, t - d - 1); k <= j && k < t; k++) {
					long long product = dp[t][k];
					if (s == t)
						product = mul_modulo(product, sums[s][j - k - 1], p);
					product = mul_modulo(product, comb[j][k], p);
					product = mul_modulo(product, comb[d][t - k - 1], p);
					term = add_modulo(term, product, p);
				}
				if (s != t)
					term = mul_modulo(term, sums[s][s - 1], p);
				term = mul_modulo(term, comb[i - 2][s - 1], p);
				value = add_modulo(value, term, p);
			}
			dp[i][j] = value;
			if (j > 0)
				sums[i][j] = add_modulo(sums[i][j - 1], dp[i][j], p);
			else
				sums[i][j] = dp[i][j];
		}
	}

	printf("%d\n", dp[n][x]);
}

int main()
{
	if (freopen("radacina2.in", "rt", stdin) == NULL)
		return 1;
	if (freopen("radacina2.out", "wt", stdout) == NULL)
		return 1;

	solve_problem();

	return 0;
}
