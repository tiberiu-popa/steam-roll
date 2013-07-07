#include <algorithm>
#include <vector>
#include <cstdio>
#include <cstring>

#define MAX_N 100000
#define MAX_K 5

using namespace std;

vector<int> primes;
vector<int> table[MAX_K];
int factors[MAX_N + 1];

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

void build_primes()
{
	primes.push_back(2);
	for (int i = 3; i <= MAX_N; i += 2) {
		bool is_prime = true;
		for (vector<int>::const_iterator it = primes.begin(); it != primes.end(); ++it) {
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

int count_distinct_factors(int n)
{
	int m = n;
	int num_factors = 0;
	for (vector<int>::const_iterator it = primes.begin(); it != primes.end() && n > 1; ++it) {
		int p = *it;
		if (n % p == 0) {
			++num_factors;
			do {
				n /= p;
			} while (n % p == 0);
			num_factors += factors[n];
			factors[m] = num_factors;
			break;
		}
	}
	return num_factors;
}

void build_table()
{
	for (int i = 2; i <= MAX_N; i++) {
		int num_factors = count_distinct_factors(i);
		--num_factors;
		if (num_factors < MAX_K)
			table[num_factors].push_back(i);
	}
}

int find_position(const vector<int> &v, int elem)
{
	vector<int>::const_iterator it = upper_bound(v.begin(), v.end(), elem);
	return (int) (it - v.begin());
}

int solve_problem()
{
	int a, b, k;

	a = read_unsigned_integer<int>();
	b = read_unsigned_integer<int>();
	k = read_unsigned_integer<int>();

	--k;
	printf("%d\n", find_position(table[k], b) - find_position(table[k], a - 1));

	return 0;
}

int main()
{
	int num_tests;

	num_tests = read_unsigned_integer<int>();

	build_primes();
	build_table();
	for (int i = 0; i < num_tests; i++)
		solve_problem();

	return 0;
}
