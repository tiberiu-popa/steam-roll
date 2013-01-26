#include <algorithm>
#include <iostream>
#include <map>
#include <utility>
#include <vector>

#define MAX_N 1000

using namespace std;

pair<int, int> v[MAX_N];
int dp[MAX_N + 1][MAX_N + 1];

void solve_problem()
{
	vector<ptrdiff_t> minor;
	int n;

	cin >> n;
	for (int i = 0; i < n; i++)
		cin >> v[i].second >> v[i].first;

	sort(v, v + n);

	for (int i = 0; i < n; i++) {
		pair<int, int> p(v[i].second, 0);
		pair<int, int> *it = lower_bound(v, v + n, p);
		minor.push_back(it - v);
	}

	dp[0][0] = 0;
	for (int i = 1; i <= n; i++) {
		ptrdiff_t left_lower = minor[i - 1];
		for (int j = 0; j <= i; j++) {
			if (i == j) {
				dp[i][j] = dp[left_lower][left_lower] + 1;
				dp[i][j] = max(dp[i][j], dp[i][j - 1]);
			} else {
				int prev_max = dp[i - 1][j];
				if (j > 0)
					prev_max = max(prev_max, dp[i][j - 1]);
				if (left_lower >= j)
					dp[i][j] = dp[left_lower][j] + 1;
				else
					dp[i][j] = dp[j][left_lower] + 1;
				dp[i][j] = max(dp[i][j], prev_max);
			}
		}
	}

	cout << dp[n][n] << endl;
}

int main()
{
	int num_tests;
	int i;

	cin >> num_tests;
	for (i = 0; i < num_tests; i++)
		solve_problem();

	return 0;
}
