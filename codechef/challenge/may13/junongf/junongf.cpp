#include <algorithm>
#include <map>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstring>

#define MAX_N 100
#define MOD_PRIME 1000000007

using namespace std;

long long mulmod(long long x, long long y, long long mod)
{
	return (x * y) % mod;
}

long long sqrmod(long long x, long long mod)
{
	return (x * x) % mod;
}

long long powmod(long long x, long long e)
{
	// assert(x > 0 || e > 0); !!!
	long long result = 1;
	long long base = x;

	if (e > 0) {
		while (true) {
			if (e & 1) {
				result = (result * base) % MOD_PRIME;
			}
			e >>= 1;
			if (e <= 0)
				break;
			base = (base * base) % MOD_PRIME;
		}
	}

	return result;
}

int solve_problem()
{
	long long v;
	int n;
	long long a0, b0, c0, m0;
	long long a1, b1, c1, m1;
	long long p[MAX_N], q[MAX_N];

	if (scanf("%lld %d", &v, &n) != 2)
		return 1;

	if (scanf("%lld %lld %lld %lld %lld %lld", &p[0], &p[1], &a0, &b0, &c0, &m0) != 6)
		return 1;

	if (scanf("%lld %lld %lld %lld %lld %lld", &q[0], &q[1], &a1, &b1, &c1, &m1) != 6)
		return 1;

	a0 = sqrmod(a0 % m0, m0);
	a1 = sqrmod(a1 % m1, m1);
	b0 %= m0;
	b1 %= m1;
	c0 %= m0;
	c1 %= m1;
	for (int i = 2; i < n; i++) {
		long long x = mulmod(a0, p[i - 1] % m0, m0);
		long long y = mulmod(b0, p[i - 2] % m0, m0);
		p[i] = (x + y + c0) % m0;

		x = mulmod(a1, q[i - 1] % m1, m1);
		y = mulmod(b1, q[i - 2] % m1, m1);
		q[i] = (x + y + c1) % m1;
	}

	long long e = 1;
	long long emod = MOD_PRIME - 1;
	bool is_flat = false;
	for (int i = 0; i < n && e > 0; i++) {
		long long l = p[i] * m1 + q[i] + 1;
		e = mulmod(e, (l - 1) % emod, emod);
		if (l <= 1)
			is_flat = true;
	}
	long long result = 1;
	if (!is_flat) {
		if (e == 0)
			result = (v % MOD_PRIME) != 0;
		else
			result = powmod(v % MOD_PRIME, e);
	}

	printf("%lld\n", result);
	return 0;
}

int main()
{
	int num_tests;

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
