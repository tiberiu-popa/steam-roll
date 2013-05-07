#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>

#define MOD_PRIME 1000000007

using namespace std;

int mulmod(int x, int y)
{
	long long prod = x;
	return (prod * y) % MOD_PRIME;
}

int sqrmod(int x)
{
	return mulmod(x, x);
}

int powmod(int x, int e)
{
	//assert(x > 0 || e > 0); !!!
	int result = 1;
	int base = x;

	if (e > 0) {
		while (true) {
			if (e & 1) {
				result = mulmod(result, base);
			}
			e >>= 1;
			if (e <= 0)
				break;
			base = sqrmod(base);
		}
	}

	return result;
}

int solve_problem()
{
	int n, k, m;

	if (scanf("%d %d", &n, &k) != 2)
		return 1;

	m = n * k;

	int result = 0;
	if (k == 1) {
		result = powmod(m, m - 2);
	} else if (k == 2) {
		if (n >= 2) {
			result = powmod(m, n - 2);
			result = mulmod(result, powmod(m - 2, n));
		} else {
			result = 0;
		}
	} else {
		if (n >= 2) {
			result = powmod(m, n - 2);
			result = mulmod(result, powmod(m - 3, 2 * n));
		} else {
			result = 0;
		}
	}
	printf("%d\n", result);

	return 0;
}

int main()
{
	int num_tests = 1;

	for (int i = 0; i < num_tests; i++) {
		int ret = solve_problem();
		if (ret != 0) {
			fprintf(stderr, "Read error at case #%d\n", i + 1);
			break;
		}
	}

	return 0;
}
