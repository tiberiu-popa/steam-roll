#include <algorithm>
#include <complex>
#include <limits>
#include <vector>
#include <cmath>
#include <cstdio>

#define MAX_PRIME 31622

using namespace std;

vector<int> primes;

int skip_whitespace()
{
	int ch;
	while (true) {
		ch = getchar_unlocked();
		if (ch != ' ' && ch != '\n')
			break;
	}
	return ch;
}

template<typename T>
T read_unsigned_integer()
{
	T result = (T) 0;
	int ch = skip_whitespace();
	while (ch >= '0' && ch <= '9') {
		result = 10 * result + (ch - '0');
		ch = getchar_unlocked();
	}
	return result;
}

template<typename T>
T read_signed_integer()
{
	T result = (T) 0;
	bool flip = false;
	int ch = skip_whitespace();
	if (ch == '-') {
		flip = true;
		ch = skip_whitespace();
	}
	while (ch >= '0' && ch <= '9') {
		result = 10 * result + (ch - '0');
		ch = getchar_unlocked();
	}
	return flip ? -result : result;
}

void build_primes(int limit)
{
	primes.push_back(2);
	for (int i = 3; i <= limit; i += 2) {
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
}

int overlucky(int n)
{
	while (n > 0) {
		int d = n % 10;
		n /= 10;
		if (d == 4 || d == 7)
			return 1;
	}
	return 0;
}

int solve_problem()
{
	vector<int> factors, exponents;
	int n;

	n = read_unsigned_integer<int>();
	for (vector<int>::iterator it = primes.begin(); it != primes.end(); ++it) {
		int p = *it;
		int e = 0;
		while (n % p == 0) {
			++e;
			n /= p;
		}
		if (e > 0) {
			factors.push_back(p);
			exponents.push_back(e);
		}
	}
	if (n > 1) {
		factors.push_back(n);
		exponents.push_back(1);
	}

	int result = 0;

	if (exponents.empty()) {
		printf("%d\n", result);
		return 0;
	}

	vector<int> s;
	s.push_back(-1);
	size_t k = 0;
	while (!s.empty()) {
		if (s.back() < exponents[k]) {
			s.back()++;
			if (k + 1 == exponents.size()) {
				int m = 1;
				for (size_t i = 0; i < s.size(); i++)
					for (int j = 0; j < s[i]; j++)
						m *= factors[i];
				result += overlucky(m);
			} else {
				++k;
				s.push_back(-1);
			}
		} else {
			s.pop_back();
			--k;
		}
	}
	printf("%d\n", result);

	return 0;
}

int main()
{
	int num_tests;

	build_primes(MAX_PRIME);

	num_tests = read_unsigned_integer<int>();
	for (int i = 0; i < num_tests; i++)
		solve_problem();

	return 0;
}
