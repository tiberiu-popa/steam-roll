#include <algorithm>
#include <functional>
#include <vector>
#include <cmath>
#include <cstdio>
#include <cstring>

#define MAX_N 100
#define MAX_TYPES 100

using namespace std;

long long a[MAX_N];
long long b[MAX_N];
int t[MAX_N];
int s[MAX_N];
long long dp[MAX_N + 1][MAX_N + 1];
vector< pair<int, long long> > type_index[MAX_TYPES][2];

void add_entry(vector< pair<int, long long> > &tindex, int pos, long long val)
{
	tindex.push_back(make_pair(pos, val + tindex.back().second));
}

long long get_sum_at_pos(const vector< pair<int, long long> > &tindex, int pos)
{
	vector< pair<int, long long> >::const_iterator it = upper_bound(tindex.begin(), tindex.end(), make_pair(pos, -1LL));
	if (it == tindex.end())
		terminate();
	return it->second;
}

int solve_problem(int num_case)
{
	int n, m;

	if (scanf("%d %d", &n, &m) != 2)
		return 1;

	for (int i = 0; i < n; i++) {
		if (scanf("%lld %d", &a[i], &t[i]) != 2)
			return 1;
		--t[i];
	}

	for (int i = 0; i < m; i++) {
		if (scanf("%lld %d", &b[i], &s[i]) != 2)
			return 1;
		--s[i];
	}

	for (int i = 0; i < MAX_TYPES; i++)
		for (int j = 0; j < 2; j++) {
			type_index[i][j].clear();
			type_index[i][j].push_back(make_pair(-1, 0LL));
		}

	for (int i = 0; i < n; i++)
		add_entry(type_index[t[i]][0], i, a[i]);

	for (int i = 0; i < m; i++)
		add_entry(type_index[s[i]][1], i, b[i]);

	for (int j = 0; j <= m; j++)
		dp[0][j] = 0;
	for (int i = 1; i <= n; i++) {
		dp[i][0] = 0;
		for (int j = 1; j <= m; j++) {
			dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
			if (t[i - 1] == s[j - 1]) {
				const vector< pair<int, long long> > &left_tindex = type_index[t[i - 1]][0];
				const vector< pair<int, long long> > &right_tindex = type_index[s[j - 1]][1];
				long long sums[] = {
					get_sum_at_pos(left_tindex, i - 1),
					get_sum_at_pos(right_tindex, j - 1),
				};
				for (size_t k = 1; k < left_tindex.size() && left_tindex[k].first <= i; k++)
					for (size_t l = 1; l < right_tindex.size() && right_tindex[l].first <= j; l++) {
						long long cand = dp[left_tindex[k].first][right_tindex[l].first];
						cand += min(sums[0] - left_tindex[k - 1].second, sums[1] - right_tindex[l - 1].second);
						if (cand > dp[i][j])
							dp[i][j] = cand;
					}
			}
		}
	}

	printf("Case #%d: %lld\n", num_case, dp[n][m]);

	return 0;
}

int main(int argc, char *argv[])
{
	const char *filenames[] = {
		"box-factory.in",
		"box-factory.out",
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
