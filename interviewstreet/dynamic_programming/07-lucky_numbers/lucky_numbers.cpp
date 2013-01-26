#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

#define MAX_NUM_DIGITS 19
#define MAX_SUM_1 (9 * MAX_NUM_DIGITS)
#define MAX_SUM_2 (81 * MAX_NUM_DIGITS)
#define MAX_PRIME MAX_SUM_2

using namespace std;

set<int> primes;
long long dp[MAX_NUM_DIGITS + 1][MAX_SUM_1 + 1][MAX_SUM_2 + 1];

void build_primes()
{
	primes.insert(2);
	for (int i = 3; i <= MAX_PRIME; i += 2) {
		bool is_prime = true;
		for (set<int>::iterator it = primes.begin(); it != primes.end(); ++it) {
			int p = *it;
			if (p * p > i)
				break;
			if (i % p == 0) {
				is_prime = false;
				break;
			}
		}
		if (is_prime)
			primes.insert(i);
	}
}

void run_dp()
{
	for (set<int>::iterator i = primes.begin(); i != primes.end(); ++i) {
		if (*i > MAX_SUM_1)
			break;
		for (set<int>::iterator j = primes.begin(); j != primes.end(); ++j) {
			if (*j > MAX_SUM_2)
				break;
			dp[0][*i][*j] = 1;
		}
	}
	for (int i = 0; i < MAX_NUM_DIGITS; i++) {
		int num_left = MAX_NUM_DIGITS - 1 - i;
		for (int j = 0; j <= 9 * num_left; j++)
			for (int k = 0; k <= 81 * num_left; k++)
				for (int t = 0; t <= 9; t++)
					dp[i + 1][j][k] += dp[i][j + t][k + t * t];
	}
}

long long lucky(long long n)
{
	vector<int> digits;

	while (n > 0) {
		digits.push_back(n % 10);
		n /= 10;
	}

	long long answer = 0;
	int s = 0, t = 0;
	for (size_t i = digits.size(); i > 0;) {
		i--;
		for (int j = 0; j < digits[i]; j++)
			answer += dp[i][s + j][t + j * j];
		s += digits[i];
		t += digits[i] * digits[i];
	}

	return answer;
}

void solve_problem()
{
	long long a, b;

	cin >> a >> b;
	cout << lucky(b + 1) - lucky(a) << endl;
}

int main()
{
	int num_tests;

	build_primes();
	run_dp();

	cin >> num_tests;
	for (int i = 0; i < num_tests; i++)
		solve_problem();

	return 0;
}
