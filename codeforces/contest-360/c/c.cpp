#include <algorithm>
#include <cstdio>
#include <cstring>

#define MOD_PRIME 1000000007
#define MAX_N 2000
#define MAX_K 2000

using namespace std;

int dp[MAX_N + 1][MAX_K + 1];
int sp[MAX_K + 1];
char str[MAX_K + 10];

char *trim_line(char *str)
{
	return strtok(str, "\r\n");
}

int addmod(int x, int y, int p)
{
	long long sum = x;
	sum += y;
	if (sum >= p)
		sum -= p;
	return (int) sum;
}

int submod(int x, int y, int p)
{
	long long diff = x;
	diff -= y;
	if (diff < 0)
		diff += p;
	return (int) diff;
}

int mulmod(int x, int y, int p)
{
	long long prod = x;
	prod = (prod * y) % p;
	return (int) prod;
}

int muladdmod(long long x, int y, int z, int p)
{
	long long prod = (x * y + z) % p;
	return (int) prod;
}

int solve_problem()
{
	char *p;
	int n, k;

	if (fgets(str, sizeof(str), stdin) == NULL)
		return 1;
	p = strtok(str, " ");
	n = atoi(p);
	p = strtok(NULL, " ");
	k = atoi(p);

	if (fgets(str, sizeof(str), stdin) == NULL)
		return 1;

	sp[0] = dp[0][0] = 1;

	for (int i = 1; i <= n; i++)
		for (int j = 0; j <= k; j++) {
			char ch = str[i - 1];
			int step = n + 1 - i;
			long long delta = 0;
			dp[i][j] = muladdmod(sp[j], ch - 'a', dp[i][j], MOD_PRIME);
			for (int r = 0, q = j - step; r < i && q >= 0; r++, q -= step)
				delta += dp[i - 1 - r][q];
			dp[i][j] = muladdmod(delta, 'z' - ch, dp[i][j], MOD_PRIME);
			sp[j] = addmod(sp[j], dp[i][j], MOD_PRIME);
		}

	int result = 0;
	for (int i = 0; i <= n; i++)
		result = addmod(result, dp[i][k], MOD_PRIME);
	printf("%d\n", result);

	return 0;
}

int main()
{
	solve_problem();
	return 0;
}
