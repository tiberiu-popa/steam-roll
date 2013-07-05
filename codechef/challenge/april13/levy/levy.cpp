#include <algorithm>
#include <vector>
#include <cmath>
#include <cstdio>
#include <cstring>

#define MAX_N 10000

using namespace std;

int cache[MAX_N + 1];
int primes[MAX_N];
int num_primes = 0;

void build_primes(int n)
{
	if (num_primes == 0) {
		primes[num_primes++] = 2;
	}
	int r = (num_primes == 1);
	for (int i = primes[num_primes - 1] + r; i <= n; i += 2) {
		int is_prime = 1;
		for (int j = 0; j < num_primes; j++)
			if (i % primes[j] == 0) {
				is_prime = 0;
				break;
			}
		if (is_prime)
			primes[num_primes++] = i;
	}
}

void solve_problem()
{
	int v[MAX_N];
	int n;

	if (scanf("%d", &n) != 1)
		return;

	if (cache[n] != 0) {
		printf("%d\n", cache[n]);
	} else {
		build_primes(n);

		int result = 0;
		int j = num_primes - 1;
		for (int i = 0; i < num_primes; i++) {
			while (j > 0 && primes[i] + 2 * primes[j] > n)
				j--;
			if (primes[i] + 2 * primes[j] == n) {
				++result;
			}
		}
		printf("%d\n", result);
		cache[n] = result;
	}
}

int main()
{
	int num_tests;

	if (scanf("%d", &num_tests) != 1)
		return 1;
	for (int i = 0; i < num_tests; i++)
		solve_problem();

	return 0;
}
