#include <algorithm>
#include <cstdio>
#include <cstring>

#if 0

#define dprintf(fmt, ...) \
	fprintf(stderr, fmt , ##__VA_ARGS__)

#else

#define dprintf(fmt, ...) \
	(void) 0

#endif

#define MAX_N 37

using namespace std;

long long x[MAX_N + 1];

double update_largest(int left, int right, long long inf, long long remaining, long long spent)
{
	double result = 0.0;
	long long delta = 0LL;

	remaining -= inf * right;
	spent += inf * right;
	for (int k = right; k > 0 && right - k <= remaining; k--) {
		if (k < right)
			delta += x[left] + inf - x[k];
		double cand = (spent - delta) * (36. / k) - (spent + right - k);
		if (cand > result)
			result = cand;
	}

	return result;
}

int solve_problem(int num_case)
{
	long long b;
	int n;

	if (scanf("%lld %d", &b, &n) != 2)
		return 1;

	fill(x, x + MAX_N, 0);
	for (int i = 0; i < n; i++)
		if (scanf("%lld", &x[i]) != 1)
			return 1;
	sort(x, x + MAX_N);
	x[MAX_N] = x[MAX_N - 1] + b + 1;

	double result = 0.0;

	long long remaining = b;
	long long spent = 0LL;
	for (int i = 0; i < MAX_N && remaining >= 0;) {
		int j = i + 1;
		while (j < MAX_N && x[j] == x[i])
			++j;

		long long diff = x[j] - x[i];
		long long inf = min(remaining / j, diff - 1);
		if (inf >= 0) {
			result = max(result, update_largest(i, j, inf, remaining, spent));
			if (--inf >= 0)
				result = max(result, update_largest(i, j, inf, remaining, spent));
		}

		remaining -= diff * j;
		spent += diff * j;
		i = j;
	}

	printf("Case #%d: %.7le\n", num_case, result);

	return 0;
}

int main(int argc, char *argv[])
{
	const char *filenames[] = {
		"cheaters.in",
		"cheaters.out",
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
