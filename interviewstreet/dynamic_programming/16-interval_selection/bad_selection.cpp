#include <algorithm>
#include <iostream>
#include <map>
#include <utility>
#include <vector>

#define MAX_N 1000

using namespace std;

pair<int, int> v[MAX_N];
int dp[MAX_N][MAX_N];

void print_dp_matrix(size_t size)
{
	for (size_t i = 0; i < size; i++) {
		for (size_t j = 0; j < size; j++) {
			if (j > 0)
				cout << ' ';
			if (dp[i][j] >= 0)
				cout << dp[i][j];
			else
				cout << ' ';
		}
		cout << endl;
	}
}

void solve_problem()
{
	vector<int> endpoints;
	map<int, size_t> reverse_ends;
	int n;

	cin >> n;
	endpoints.push_back(0);
	for (int i = 0; i < n; i++) {
		int right;
		cin >> v[i].first >> right;
		v[i].second = right;
		endpoints.push_back(right);
	}

	sort(v, v + n);
	sort(endpoints.begin(), endpoints.end());
	vector<int>::iterator uq_it = unique(endpoints.begin(), endpoints.end());
	endpoints.resize(uq_it - endpoints.begin());

	for (size_t i = 0; i < endpoints.size(); i++)
		reverse_ends[endpoints[i]] = i;

	for (size_t i = 0; i < endpoints.size(); i++)
		for (size_t j = 0; j < endpoints.size(); j++)
			dp[i][j] = -1;

	dp[0][0] = 0;
	for (int i = 0; i < n; i++) {
		size_t right = reverse_ends[v[i].second];
		vector<int>::iterator l_it = lower_bound(endpoints.begin(),
			endpoints.end(), v[i].first);
		size_t pos = l_it - endpoints.begin();
		vector<int> u;
		for (size_t j = 0; j < endpoints.size(); j++) {
			int max_path = -1;
			for (size_t k = 0; k < pos && k <= j; k++) {
				if (dp[k][j] >= 0)
					max_path = max(max_path, dp[k][j]);
			}
			u.push_back(1 + max_path);
		}
		for (size_t j = 0; j < u.size(); j++)
			if (u[j] > 0) {
				size_t p = min(right, j);
				size_t q = max(right, j);
				dp[p][q] = max(dp[p][q], u[j]);
			}
	}

	int answer = 0;
	for (size_t i = 0; i < endpoints.size(); i++) {
		int row_max = *max_element(dp[i], dp[i] + endpoints.size());
		answer = max(row_max, answer);
	}
	cout << answer << endl;
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
