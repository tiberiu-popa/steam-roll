#include <algorithm>
#include <iostream>
#include <map>
#include <cstring>

#define MAX_N 40
#define MAX_SUM (MAX_N * (MAX_N - 1) / 2)
#define MOD_PRIME 1000000007

using namespace std;

int comb[MAX_N + 1][MAX_N + 1];
int dp[MAX_N][MAX_SUM + 1][MAX_N + 1];

int add_modulo(int x, int y)
{
	int sum = x + y;
	if (sum >= MOD_PRIME)
		sum -= MOD_PRIME;
	return sum;
}

int mul_modulo(int x, int y)
{
	long long prod = x;
	prod *= y;
	return prod % MOD_PRIME;
}

void build_comb()
{
	for (int i = 0; i <= MAX_N; i++) {
		comb[i][0] = 1;
		for (int j = 1; j <= i; j++)
			comb[i][j] = add_modulo(comb[i - 1][j], comb[i - 1][j - 1]);
	}
}

template<class T>
void clear_array(T *v, int n)
{
	memset(v, 0, n * sizeof(*v));
}

void solve_problem()
{
	map<int, int> v;
	int n;

	cin >> n;
	for (int i = 0; i < n; i++) {
		int x;
		cin >> x;
		if (x >= 0)
			v[x]++;
	}

	clear_array(dp[0][0], n + 1);

	for (int k = v[0]; k <= n; k++)
		if (0 >= k * (k - 1) / 2)
			dp[0][0][k] = 1;
		else
			break;

	int max_sum = n * (n - 1) / 2;
	int extra = v[0];
	for (int i = 1; i < n; i++) {
		extra += v[i];
		for (int j = 0; j <= max_sum; j++)
			for (int k = 0; k <= n; k++) {
				int value = 0;
				for (int l = v[i]; l <= k; l++) {
					int reduced_sum = j - l * i;
					int reduced_index = k - l;
					int limit = reduced_index * (reduced_index - 1) / 2;
					if (reduced_sum >= limit) {
						int term = dp[i - 1][reduced_sum][reduced_index];
						term = mul_modulo(term, comb[k - extra][l - v[i]]);
						value = add_modulo(value, term);
					} else {
						break;
					}
				}
				dp[i][j][k] = value;
			}
	}
	cout << dp[n - 1][max_sum][n] << endl;
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
