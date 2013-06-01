#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>

using namespace std;

long long pow2ll(int k)
{
	return 1LL << k;
}

int solve_problem(int num_case)
{
	int n;
	long long p;

	if (scanf("%d %lld", &n, &p) != 2)
		return 1;

	long long m = 1;
	if (p < pow2ll(n)) {
		long long r = 0;
		int k = n - 1;
		int l = 1;
		while (r + pow2ll(k) < p) {
			m += pow2ll(l);
			r += pow2ll(k);
			--k;
			++l;
		}
	} else {
		m = pow2ll(n);
	}

	long long result = m - 1;

	m = 1;
	if (p < pow2ll(n)) {
		long long r = 0;
		int k = 0;
		int l = n - 1;
		while (r + pow2ll(k) < p) {
			m += pow2ll(l);
			r += pow2ll(k);
			++k;
			--l;
		}
	} else {
		m = pow2ll(n);
	}

	printf("Case #%d: %lld %lld\n", num_case, result, m - 1);

	return 0;
}

int main(int argc, char *argv[])
{
	const char *filenames[] = {
		"many-prizes.in",
		"many-prizes.out",
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
