#include <algorithm>
#include <vector>
#include <cmath>
#include <cstdio>
#include <cstdlib>

using namespace std;

#define MAX_PRIME 31623
#define MAX_N 5000

vector<int> primes;
long long tests[MAX_N];
bool results[MAX_N];

void build_primes()
{
	primes.push_back(2);
	for (int i = 3; i <= MAX_PRIME; i += 2) {
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

bool is_prime(long long n)
{
	if (n <= 1)
		return false;
	bool is_prime = true;
	for (vector<int>::iterator it = primes.begin(); it != primes.end(); ++it) {
		long long p = *it;
		if (p * p > n)
			break;
		if (n % p == 0) {
			is_prime = false;
			break;
		}
	}
	return is_prime;
}

void read_input(const char *filename, int &n)
{
	FILE *f;

	f = fopen(filename, "rt");
	if (f == NULL) {
		fprintf(stderr, "Could not open file: %s\n", filename);
		exit(EXIT_FAILURE);
	}

	if (fscanf(f, "%d", &n) != 1)
		exit(EXIT_FAILURE);

	for (int i = 0; i < n; i++)
		if (fscanf(f, "%lld", &tests[i]) != 1)
			exit(EXIT_FAILURE);

	fclose(f);
}

void write_output(const char *filename, int n)
{
	FILE *f;

	f = fopen(filename, "wt");
	if (f == NULL) {
		fprintf(stderr, "Could not open file: %s\n", filename);
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < n; i++)
		fprintf(f, "%s\n", results[i] ? "DA" : "NU");
	fclose(f);
}

long long powbin(long long n, int e)
{
	long long result = 1, base = n;

	while (e > 0) {
		if (e & 1)
			 result *= base;
		e >>= 1;
		if (e > 0)
			base *= base;
	}

	return result;
}

void solve_problem(int n)
{
	for (int i = 0; i < n; i++) {
		long long x = tests[i];
		int k = 0;
		for (long long y = x; y > 0; y >>= 1)
			k++;
		bool done = false;
		for (int j = 2; j <= k && !done; j++) {
			double z = x;
			z = pow(z, 1. / j);
			long long r = (long long) z;
			//printf("x=%lld z=%lf\n", x, z);
			for (long long l = r - 1; l <= r + 1; l++) {
				if (powbin(l, j) == x) {
					if (is_prime(l)) {
						results[i] = true;
						done = true;
						break;
					}
				}
			}
		}
	}
}

int main()
{
	int n;

	build_primes();
	read_input("ubercool.in", n);
	solve_problem(n);
	write_output("ubercool.out", n);

	return 0;
}
