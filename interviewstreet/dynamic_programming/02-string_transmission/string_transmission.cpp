#include <iostream>
#include <map>
#include <string>
#include <vector>

#define MAX_N 1000
#define MOD_PRIME 1000000007

using namespace std;

vector<int> primes;
int comb[MAX_N + 1][MAX_N + 1];
int dp[MAX_N + 1][MAX_N + 1];

template<class T>
void print_vector(const vector<T> &v)
{
	for (typename vector<T>::const_iterator it = v.begin(); it != v.end(); ++it) {
		if (it != v.begin())
			cout << ' ';
		cout << *it;
	}
	cout << endl;
}

template<class T>
void print_vector(const char *title, const vector<T> &v)
{
	cout << title << ": ";
	print_vector(v);
}

void add_to_mod(int *lhs, int rhs)
{
	int value = *lhs + rhs;
	if (value >= MOD_PRIME)
		value -= MOD_PRIME;
	*lhs = value;
}

void subtract_from_mod(int *lhs, int rhs)
{
	int value = *lhs - rhs;
	if (value < 0)
		value += MOD_PRIME;
	*lhs = value;
}

void mul_by_mod(int *lhs, int rhs)
{
	long long value = *lhs;
	value *= rhs;
	*lhs = value % MOD_PRIME;
}

void build_primes()
{
	primes.push_back(2);
	for (int i = 3; i <= MAX_N; i += 2) {
		bool is_prime = true;
		for (vector<int>::iterator it = primes.begin(); it != primes.end(); ++it) {
			int p = *it;
			if (p * p > i)
				break;
			if (i % p == 0) {
				is_prime = false;
				break;
			}
		}
		if (is_prime)
			primes.push_back(i);
	}
	//print_vector("primes", primes);
}

void build_comb()
{
	for (int i = 0; i <= MAX_N; i++)
		comb[i][0] = comb[i][i] = 1;
	for (int i = 1; i <= MAX_N; i++)
		for (int j = 1; j < i; j++) {
			comb[i][j] = comb[i - 1][j - 1];
			add_to_mod(&comb[i][j], comb[i - 1][j]);
		}
	/*
	for (int i = 0; i <= 10; i++) {
		for (int j = 0; j <= i; j++)
			cout << comb[i][j] << ' ';
		cout << endl;
	}
	*/
}

int solve_period(const vector<bool> &v, int p, int k)
{
	for (int i = 0; i <= p; i++)
		for (int j = 0; j <= k; j++)
			dp[i][j] = 0;
	dp[0][0] = 1;
	for (int i = 1; i <= p; i++) {
		int s = 0, t = 0;
		for (size_t j = (size_t) i - 1; j < v.size(); j += p)
			if (v[j])
				t++;
			else
				s++;
		for (int j = s; j <= k; j++)
			add_to_mod(&dp[i][j], dp[i - 1][j - s]);
		for (int j = t; j <= k; j++)
			add_to_mod(&dp[i][j], dp[i - 1][j - t]);
	}
	int sum = 0;
	for (int j = 0; j <= k; j++)
		add_to_mod(&sum, dp[p][j]);
	return sum;
}

void fill_prime_divs(vector<int> &pdivs, int n)
{
	int m = n;

	pdivs.clear();
	for (vector<int>::iterator it = primes.begin(); it != primes.end(); ++it) {
		int p = *it;
		if (p * p > m)
			break;

		bool is_divisor = false;
		while (m % p == 0) {
			m /= p;
			is_divisor = true;
		}

		if (is_divisor)
			pdivs.push_back(p);
	}
	if (m > 1)
		pdivs.push_back(m);
}

void solve_problem()
{
	vector<bool> v;
	string str;
	int n, k;

	cin >> n >> k >> str;
	for (size_t i = 0; i < str.size(); i++)
		v.push_back(str[i] == '1');

	vector<int> pdivs;
	fill_prime_divs(pdivs, n);

	int last = 0;
	int b = (1 << pdivs.size()) - 1;
	for (int i = b; i > 0; i = (i - 1) & b) {
		int m = n;
		int parity = 0;
		for (size_t j = 0; j < pdivs.size(); j++)
			if (i & (1 << j)) {
				m /= pdivs[j];
				parity ^= 1;
			}
		int result = solve_period(v, m, k);
		if (parity)
			add_to_mod(&last, result);
		else
			subtract_from_mod(&last, result);
	}

	int answer = 0;
	for (int i = 0; i <= k; i++)
		add_to_mod(&answer, comb[n][i]);
	subtract_from_mod(&answer, last);
	cout << answer << endl;
}

int main()
{
	int t;

	build_primes();
	build_comb();

	cin >> t;
	for (int i = 0; i < t; i++)
		solve_problem();

	return 0;
}
