#include <algorithm>
#include <functional>
#include <vector>
#include <cmath>
#include <cstdio>
#include <cstring>

#define MAX_N 60
#define MAX_VALUE 10000
#define pow2ll(n) (1LL << (n))

using namespace std;

vector<int> primes;
int v[MAX_N];

void build_primes()
{
	primes.push_back(2);
	for (int i = 3; i <= MAX_VALUE; i += 2) {
		bool is_prime = true;
		for (size_t j = 0; j < primes.size(); j++) {
			if (primes[j] * primes[j] > i)
				break;
			if (i % primes[j] == 0) {
				is_prime = false;
				break;
			}
		}
		if (is_prime)
			primes.push_back(i);
	}
}

int count_distinct_prime_factors(int n)
{
	int count = 0;
	for (size_t i = 0; i < primes.size(); i++) {
		if (n % primes[i] == 0) {
			++count;
			n /= primes[i];
			if (n % primes[i] == 0)
				return -1;
			if (n == 1)
				break;
		}
	}
	return count;
}

int solve_problem()
{
	int n;

	if (scanf("%d", &n) != 1)
		return 1;

	for (int i = 0; i < n; i++)
		if (scanf("%d", &v[i]) != 1)
			return 1;

	long long result = 0;
	for (int i = 2; i <= MAX_VALUE; i++) {
		int num_factors = count_distinct_prime_factors(i);
		if (num_factors > 0) {
			int count = 0;
			for (int j = 0; j < n; j++)
				if (v[j] % i == 0)
					++count;
			if (count > 0) {
				long long delta = count;
				if (count > 1)
					delta += (pow2ll(count) - count - 1);
				if (num_factors & 1)
					result += delta;
				else
					result -= delta;
			}
		}
	}

	printf("%lld\n", pow2ll(n) - 1 - result);

	return 0;
}

int main()
{
	int num_tests;

	build_primes();

	if (scanf("%d", &num_tests) != 1)
		return 1;
	for (int i = 0; i < num_tests; i++)
		solve_problem();

	return 0;
}
