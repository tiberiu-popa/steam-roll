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
	dp[0][0][0] = 1;
	for (int i = 0; i < MAX_NUM_DIGITS; i++)
		for (int j = 0; j <= 9 * i; j++)
			for (int k = 0; k <= 81 * i; k++)
				for (int t = 0; t <= 9; t++)
					dp[i + 1][j + t][k + t * t] += dp[i][j][k];
}

long long fixed_lucky(size_t num_digits, int s, int t)
{
	long long answer = 0;
	for (size_t i = 0; i <= 9 * num_digits; i++)
		if (primes.find(s + i) != primes.end()) {
			for (size_t j = 0; j <= 81 * num_digits; j++)
				if (primes.find(t + j) != primes.end()) {
					answer += dp[num_digits][i][j];
				}
		}
	return answer;
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
			answer += fixed_lucky(i, s + j, t + j * j);
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
