#include <iomanip>
#include <iostream>
#include <map>

#define MAX_N 50

using namespace std;

long long comb[MAX_N + 1][MAX_N + 1];

void build_comb()
{
	for (int i = 0; i <= MAX_N; i++) {
		comb[i][0] = 1;
		for (int j = 1; j <= i; j++)
			comb[i][j] = comb[i - 1][j] + comb[i - 1][j - 1];
	}
}

void solve_problem()
{
	map<int, int> count;
	int n;

	cin >> n;
	for (int i = 0; i < n; i++) {
		int value;
		cin >> value;
		++count[value];
	}

	double answer = 0;
	int sum = 0;
	for (map<int, int>::reverse_iterator it = count.rbegin(); it != count.rend(); ++it) {
		double value = 0;
		sum += it->second;
		for (int i = 0; i <= n - sum; i++) {
			double ratio = (double) comb[n - i - 1][sum - 1] / comb[n][sum];
			value += (i + 1) * ratio;
		}
		answer += it->second * value;
	}
	cout << fixed << setprecision(2) << answer << endl;
}

int main()
{
	int num_tests;

	build_comb();

	cin >> num_tests;
	for (int i = 0; i < num_tests; i++)
		solve_problem();

	return 0;
}
