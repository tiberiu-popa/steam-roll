#include <algorithm>
#include <iostream>
#include <map>
#include <vector>
#include <cstdio>
#include <ctime>

#define MAX_N 1000000
#define MAX_PRIME 1000000
#define MAX_TIME 3

using namespace std;

vector<int> primes;
bool prime[MAX_PRIME + 1];
long long v[MAX_N];
clock_t cs;

template<typename T>
T gcd(T x, T y)
{
	if (x < 0)
		x = -x;
	if (y < 0)
		y = -y;
	if (x < y)
		swap(x, y);
	while (y > 0) {
		T r = x % y;
		x = y;
		y = r;
	}
	return x;
}

template<typename T>
T pow2(int k)
{
	T result = 1;
	return result << k;
}

template<typename T>
int get_msb(T n)
{
	int k = 0;
	for (; n > 0; n >>= 1)
		k++;
	return k;
}

template<typename T>
int get_lower_exp(T n)
{
	return get_msb(n >> 1);
}

template<typename T>
int get_upper_exp(T n)
{
	return get_msb(n - 1);
}

template<typename T>
T i_lsqrt(T n)
{
	int e = get_lower_exp(n);
	T left = 1, right = ((T) 1) << (e / 2 + 1);
	while (left < right) {
		T mid = left + (right - left + 1) / 2;
		T sqm = mid * mid;
		if (sqm == n)
			return mid;
		else if (sqm < n)
			left = mid;
		else
			right = mid - 1;
	}
	return left;
}

void build_primes(int limit)
{
	int sqlim = i_lsqrt(limit);

	primes.push_back(2);
	fill(prime + 2, prime + limit + 1, true);
	for (int j = 4; j <= limit; j += 2)
		prime[j] = false;

	for (int i = 3; i <= sqlim; i += 2)
		if (prime[i]) {
			primes.push_back(i);
			for (int j = i * i; j <= limit; j += i)
				prime[j] = false;
		}
	for (int i = (sqlim + 1) | 1; i <= limit; i += 2)
		if (prime[i])
			primes.push_back(i);
}

int better_rand(int n, int shift)
{
	long long r;
	do {
		long long p = rand();
		long long q = rand();
		r = (p * RAND_MAX + q) >> shift;
	} while (r >= n);
	return r;
}

void compute_factors(long long n, vector< pair<long long, int> > &factors)
{
	for (vector<int>::iterator it = primes.begin(); n > 1 && it != primes.end(); ++it) {
		int p = *it;
		int e = 0;
		while (n % p == 0) {
			++e;
			n /= p;
		}
		if (e > 0)
			factors.push_back(make_pair(p, e));
	}
	if (n > 1)
		factors.push_back(make_pair(n, 1));
}

template<class T>
void trim_unique(vector<T> &v)
{
	typename vector<T>::iterator it = unique(v.begin(), v.end());
	typename vector<T>::difference_type diff = it - v.begin();
	v.resize((typename vector<T>::size_type) diff);
}

template<typename T>
void fisher_yates(T *v, int n)
{
	int m = get_upper_exp(n);
	int max_lg = 2 * get_upper_exp<int>(RAND_MAX);

	for (int i = n - 1; i >= 1; i--) {
		int j = better_rand(i + 1, max_lg - m);
		if (i != j)
			swap(v[i], v[j]);
		if ((i & (i - 1)) == 0)
			--m;
	}
}

clock_t time_elapsed()
{
	return (clock() - cs) / CLOCKS_PER_SEC;
}

int solve_problem()
{
	long long max_val = 0;
	int n;

	cin >> n;

	for (int i = 0; i < n; i++)
		cin >> v[i];

	for (int i = 0; i < n; i++)
		if (v[i] > max_val)
			max_val = v[i];

	int limit = (int) i_lsqrt(max_val);
	build_primes(limit);

	fisher_yates(v, n);

	long long result = 1;
	for (int i = 0; i < n && time_elapsed() < MAX_TIME; i++) {
		vector< pair<long long, int> > factors;
		vector<long long> divisors;

		compute_factors(v[i], factors);
		divisors.push_back(1);
		for (size_t j = 0; j < factors.size(); j++) {
			size_t sz = divisors.size();
			long long x = factors[j].first;
			for (int l = 1; l <= factors[j].second; l++) {
				for (size_t k = 0; k < sz; k++)
					divisors.push_back(divisors[k] * x);
				x *= factors[j].first;
			}
		}
		sort(divisors.begin(), divisors.end());

		map<long long, int> cnt;
		for (int j = 0; j < n; j++) {
			long long g = gcd(v[i], v[j]);
			++cnt[g];
		}

		for (size_t j = 0; j < divisors.size(); j++) {
			long long a = divisors[j];
			for (size_t k = j + 1; k < divisors.size(); k++) {
				long long b = divisors[k];
				if (b % a == 0)
					cnt[a] += cnt[b];
			}
		}

		for (size_t j = divisors.size(); j > 0;) {
			--j;
			long long a = divisors[j];
			if (2 * cnt[a] >= n) {
				if (a > result)
					result = a;
				break;
			}
		}
	}

	cout << result << endl;

	return 0;
}

int main()
{
	cs = clock();
	srand(time(NULL));
	solve_problem();
	return 0;
}
