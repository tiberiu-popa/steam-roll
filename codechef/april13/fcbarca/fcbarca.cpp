#include <algorithm>
#include <vector>
#include <cmath>
#include <cstdio>
#include <cstring>

#define MOD_PRIME 1000000007

using namespace std;

void solve_problem()
{
	int n, k;

	if (scanf("%d %d", &n, &k) != 2)
		return;

	int a = 0;
	int b = k;
	for (int i = 2; i < n; i++) {
		long long prod;
		int c = 0;

		prod = a;
		c += (prod * k) % MOD_PRIME;

		prod = b;
		c += (prod * (k - 1)) % MOD_PRIME;

		if (c >= MOD_PRIME)
			c -= MOD_PRIME;

		a = b;
		b = c;
	}
	printf("%d\n", b);
}

int main()
{
	int num_tests;

	if (scanf("%d", &num_tests) != 1)
		return 1;
	for (int i = 0; i < num_tests; i++)
		solve_problem();

	return 0;
}
