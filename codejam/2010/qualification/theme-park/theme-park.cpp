#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>

#define MAX_N 1000
#define NUM_LEVELS 30

using namespace std;

int g[MAX_N];
long long sum[NUM_LEVELS][MAX_N];
int next_sum[NUM_LEVELS][MAX_N];
long long ride[NUM_LEVELS][MAX_N];
int next_ride[NUM_LEVELS][MAX_N];

int compute_lower_exp(int n)
{
	int k = -1;
	while (n > 0) {
		k++;
		n >>= 1;
	}
	return k;
}

void build_logtable(long long table[][MAX_N], int next[][MAX_N], int n, int max_level)
{
	for (int i = 1; i <= max_level; i++)
		for (int j = 0; j < n; j++) {
			int mid = next[i - 1][j];
			table[i][j] = table[i - 1][j] + table[i - 1][mid];
			next[i][j] = next[i - 1][mid];
		}
}

int solve_problem(int num_case)
{
	int r, n;
	long long k;

	if (scanf("%d %lld %d", &r, &k, &n) != 3)
		return 1;

	for (int i = 0; i < n; i++)
		if (scanf("%d", &g[i]) != 1)
			return 1;

	for (int j = 0; j < n; j++) {
		sum[0][j] = g[j];
		next_sum[0][j] = (j + 1) % n;
	}

	int nexp = compute_lower_exp(n);
	build_logtable(sum, next_sum, n, nexp);

	for (int j = 0; j < n; j++) {
		long long partial = 0;
		int p = j;
		int path_length = 0;
		while (true) {
			int i = nexp;
			for (; i >= 0; i--) {
				int mask = 1 << i;
				if (partial + sum[i][p] <= k && path_length + mask <= n) {
					partial += sum[i][p];
					path_length += mask;
					break;
				}
			}
			if (i >= 0) {
				p = next_sum[i][p];
			} else {
				break;
			}
		}
		ride[0][j] = partial;
		next_ride[0][j] = p;
	}

	build_logtable(ride, next_ride, n, NUM_LEVELS - 1);

	long long result = 0;
	int x = 0;
	for (int i = 0; i < NUM_LEVELS; i++)
		if (r & (1 << i)) {
			result += ride[i][x];
			x = next_ride[i][x];
		}

	printf("Case #%d: %lld\n", num_case, result);

	return 0;
}

int main(int argc, char *argv[])
{
	const char *filenames[] = {
		"theme-park.in",
		"theme-park.out",
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
