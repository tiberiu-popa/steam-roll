#include <algorithm>
#include <iostream>
#include <map>
#include <utility>
#include <vector>

#define MAX_N 1000

using namespace std;

pair<int, int> v[MAX_N];
map<int, int> dp[MAX_N + 1];

template<class T>
void print_vector(const vector<T> &v)
{
	for (typename vector<T>::const_iterator it = v.begin(); it != v.end(); ++it) {
		if (it != v.begin())
			cout << ' ';
		//cout << *it;
		cout << it->first << ',' << it->second;
	}
	cout << endl;
}

template<class T>
void print_vector(const char *title, const vector<T> &v)
{
	cout << title << ": ";
	print_vector(v);
}

template<class T, class S>
void print_map(const map<T, S> &v)
{
        for (typename map<T, S>::const_iterator it = v.begin(); it != v.end(); ++it) {
		if (it != v.begin())
			cout << ' ';
                cout << '(' << it->first << ", " << it->second << ')';
	}
	cout << endl;
}

void solve_problem()
{
	int n;

	cin >> n;
	for (int i = 0; i < n; i++)
		cin >> v[i].first >> v[i].second;

	sort(v, v + n);

	for (int j = 0; j <= n; j++)
		dp[j].clear();
	dp[0][0] = 0;
	dp[1][0] = v[0].second;
	for (int i = 1; i < n; i++) {
		for (int j = n; j >= 1; j--) {
			map<int, int> &crt = dp[j];
			const map<int, int> &prev = dp[j - 1];
			for (map<int, int>::const_iterator it = prev.begin(); it != prev.end(); ++it) {
				pair<int, int> elem = *it;
				if (elem.first < v[i].first) {
					int smaller = min(v[i].second, elem.second);
					int larger = max(v[i].second, elem.second);
					int former = crt[smaller];
					if (former == 0 || larger < former)
						crt[smaller] = larger;
				} else {
					break;
				}
			}
		}
	}

	for (int j = n; j >= 0; j--)
		if (!dp[j].empty()) {
			cout << j << endl;
			break;
		}
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

