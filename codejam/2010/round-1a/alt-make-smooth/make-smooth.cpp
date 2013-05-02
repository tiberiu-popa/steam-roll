#include <algorithm>
#include <deque>
#include <cmath>
#include <cstdio>
#include <cstring>

#define MAX_N 100
#define NUM_PIXELS 256

using namespace std;

int dp[MAX_N][NUM_PIXELS];

void build_p(int *p, const int *dp, int m, int ins_cost)
{
	for (int j = NUM_PIXELS - 1; j >= 0; j--) {
		p[j] = dp[j] + j / m * ins_cost;
		if (j + m < NUM_PIXELS && p[j + m] < p[j])
			p[j] = p[j + m];
	}
}

void build_q(int *q, const int *dp, int m, int ins_cost)
{
	for (int j = 0; j < NUM_PIXELS; j++) {
		q[j] = dp[j] + (NUM_PIXELS - 1 - j) / m * ins_cost;
		if (j - m >= 0 && q[j - m] < q[j])
			q[j] = q[j - m];
	}
}

void compute_sliding_minima(int *r, int m)
{
	deque< pair<int, int> > minima;
	for (int j = 0; j < NUM_PIXELS + m; j++) {
		if (j < NUM_PIXELS) {
			while (!minima.empty()) {
				pair<int, int> entry = minima.back();
				if (r[j] <= entry.first)
					minima.pop_back();
				else
					break;
			}
			minima.push_back(pair<int, int>(r[j], j));
		}
		if (minima.front().second == j - 2 * m - 1)
			minima.pop_front();

		if (j >= m)
			r[j - m] = minima.front().first;
	}
}

int solve_problem(int num_case)
{
	int del_cost, ins_cost, m, n;
	int a[MAX_N];
	int p[NUM_PIXELS];
	int q[NUM_PIXELS];
	int r[NUM_PIXELS];

	if (scanf("%d %d %d %d", &del_cost, &ins_cost, &m, &n) != 4)
		return 1;

	for (int i = 0; i < n; i++)
		if (scanf("%d", &a[i]) != 1)
			return 1;

	for (int j = 0; j < NUM_PIXELS; j++)
		dp[0][j] = min(abs(a[0] - j), del_cost);

	for (int i = 1; i < n; i++) {
		for (int j = 0; j < NUM_PIXELS; j++)
			dp[i][j] = dp[i - 1][j] + del_cost;
		if (m > 0) {
			build_p(p, dp[i - 1], m, ins_cost);
			build_q(q, dp[i - 1], m, ins_cost);
			for (int j = 0; j < NUM_PIXELS; j++) {
				int left = p[j] - j / m * ins_cost;
				int right = q[j] - (NUM_PIXELS - 1 - j) / m * ins_cost;
				r[j] = min(left, right);
			}
			compute_sliding_minima(r, m);

			for (int j = 0; j < NUM_PIXELS; j++) {
				int cand = r[j] + abs(a[i] - j);
				if (cand < dp[i][j])
					dp[i][j] = cand;
			}
		} else {
			for (int j = 0; j < NUM_PIXELS; j++) {
				int cand = dp[i - 1][j] + abs(a[i] - j);
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
