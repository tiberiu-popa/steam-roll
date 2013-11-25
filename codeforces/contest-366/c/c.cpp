#include <algorithm>
#include <cstdio>

#define MAX_N 100
#define MIN_VALUE (-999)
#define MAX_VALUE 9900
#define MIN_SUM (MIN_VALUE * MAX_N)
#define MAX_SUM (MAX_VALUE * MAX_N)
#define DELTA_SUM (MAX_SUM - MIN_SUM + 1)

using namespace std;

int a[MAX_N];
int b[MAX_N];
int dp[DELTA_SUM];

int solve_problem()
{
	int n, k;

	if (scanf("%d %d", &n, &k) != 2)
		return 1;

	for (int i = 0; i < n; i++)
		if (scanf("%d", &a[i]) != 1)
			return 1;

	for (int i = 0; i < n; i++)
		if (scanf("%d", &b[i]) != 1)
			return 1;

	fill(dp, dp + DELTA_SUM, -1);
	dp[-MIN_SUM] = 0;
	for (int l = 0; l < n; l++) {
		int diff = a[l] - k * b[l];
		if (diff >= 0) {
			for (int i = MAX_SUM; i >= MIN_SUM; i--) {
				int j = i - MIN_SUM;
				if (dp[j] >= 0) {
					int q = j + diff;
					if (dp[j] + a[l] > dp[q])
						dp[q] = dp[j] + a[l];
				}
			}
		} else {
			for (int i = MIN_SUM; i <= MAX_SUM; i++) {
				int j = i - MIN_SUM;
				if (dp[j] >= 0) {
					int q = j + diff;
					if (dp[j] + a[l] > dp[q])
						dp[q] = dp[j] + a[l];
				}
			}
		}
	}

	int result = dp[-MIN_SUM];
	if (result == 0)
		result = -1;
	printf("%d\n", result);

	return 0;
}

int main()
{
	solve_problem();
	return 0;
}
