#include <algorithm>
#include <cstdio>
#include <cstring>

#define MAX_N 10000
#define MOD_PRIME 1000000007

using namespace std;

int mulmod(int x, int y, int mod)
{
	long long result = x;
	return (result * y) % mod;
}

int invmod(int x, int mod)
{
	int result[2][2] = { { 1, 0}, { 0, 1 } };
	int base[2][2] = { { 0, 1}, { 1, 0 } };
	int tmp[2][2];

	int y = x % mod;
	x = mod;
	while (y > 0) {
		int q = x / y;
		int r = x % y;
		base[1][1] = -q;
		memset(tmp, 0, sizeof(tmp));
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 2; j++) {
				tmp[i][j] = 0;
				for (int k = 0; k < 2; k++)
					tmp[i][j] += result[i][k] * base[k][j];
			}
		memcpy(result, tmp, sizeof(result));
		x = y;
		y = r;
	}
	int inv = result[1][0];
	if (inv < 0)
		inv += mod;
	return inv;
}

int main()
{
	int num_tests;
	int v[MAX_N];
	int n, k;

	scanf("%d", &num_tests);
	for (int i = 0; i < num_tests; i++) {
		scanf("%d %d", &n, &k);
		for (int j = 0; j < n; j++)
			scanf("%d", &v[j]);
		sort(v, v + n);
		int sum = 0;
		int x = 1;
		for (int j = k - 1; j < n; j++) {
			int term = mulmod(x, v[j] % MOD_PRIME, MOD_PRIME);
			sum = (sum + term) % MOD_PRIME;
			x = mulmod(x, j + 1, MOD_PRIME);
			int y = invmod(j + 1 - k + 1, MOD_PRIME);
			x = mulmod(x, y, MOD_PRIME);
		}
		printf("Case #%d: %d\n", i + 1, sum);
	}
}
