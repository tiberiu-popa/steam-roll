#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>

#define MAX_N 100
#define NUM_PIXELS 256

using namespace std;

int dp[MAX_N][NUM_PIXELS];

int solve_problem(int num_case)
{
	int del_cost, ins_cost, m, n;
	int a[MAX_N];

	if (scanf("%d %d %d %d", &del_cost, &ins_cost, &m, &n) != 4)
		return 1;

	for (int i = 0; i < n; i++)
		if (scanf("%d", &a[i]) != 1)
			return 1;

	for (int j = 0; j < NUM_PIXELS; j++)
		dp[0][j] = abs(a[0] - j);

	for (int i = 1; i < n; i++) {
		for (int j = 0; j < NUM_PIXELS; j++)
			dp[i][j] = i * del_cost + min(abs(a[i] - j), del_cost);
		for (int j = 0; j < NUM_PIXELS; j++) {
			int cand = dp[i - 1][j] + del_cost;
			if (cand < dp[i][j])
				dp[i][j] = cand;
			if (m > 0) {
				for (int k = 0; k < NUM_PIXELS; k++) {
					int num_inserts = 0;
					if (k != j)
						num_inserts = (abs(k - j) - 1) / m;
					cand = dp[i - 1][j] + abs(a[i] - k) + num_inserts * ins_cost;
					if (cand < dp[i][k])
						dp[i][k] = cand;
				}
			} else {
				cand = dp[i - 1][j] + abs(a[i] - j);
				if (cand < dp[i][j])
					dp[i][j] = cand;
			}
		}
	}

	int result = *min_element(dp[n - 1], dp[n - 1] + NUM_PIXELS);
	printf("Case #%d: %d\n", num_case, result);

	return 0;
}

int main(int argc, char *argv[])
{
	const char *filenames[] = {
		"make-smooth.in",
		"make-smooth.out",
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
