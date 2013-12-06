#include <algorithm>
#include <limits>
#include <map>
#include <set>
#include <vector>
#include <cstdio>

#define MAX_N 10000
#define MAX_M 100000
#define MAX_PRIME 31622

using namespace std;

vector<int> primes;
bool prime[MAX_PRIME + 1];
int a[MAX_N];
int b[MAX_M];

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

int ipow(int x, int e)
{
	int result = 1;
	int base = x;

	if (e > 0) {
		while (true) {
			if (e & 1) {
				result *= base;
			}
			e >>= 1;
			if (e <= 0)
				break;
			base *= base;
		}
	}

	return result;
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

int powmod(int x, int e, int p)
{
	long long result = 1;
	long long base = x;

	if (e > 0) {
		while (true) {
			if (e & 1) {
				result = (result * base) % p;
			}
			e >>= 1;
			if (e <= 0)
				break;
			base = (base * base) % p;
		}
	}

	return (int) result;
}

void compute_factors(int n, vector< pair<int, int> > &factors)
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

int compute_order(int x, int p, int phi, const vector< pair<int, int> > &factors)
{
	int order = 1;

	for (size_t i = 0; i < factors.size(); i++) {
		int e = factors[i].second;
		int j = 0;
		for (int q = 1; j <= e; j++) {
			if (j > 0)
				q *= factors[i].first;
			if (powmod(x, phi / q, p) != 1)
				break;
		}
		order *= ipow(factors[i].first, e - (j - 1));
	}

	return order;
}

template<class T>
void trim_unique(vector<T> &v)
{
	typename vector<T>::iterator it = unique(v.begin(), v.end());
	typename vector<T>::difference_type diff = it - v.begin();
	v.resize((typename vector<T>::size_type) diff);
}

int solve_problem()
{
	vector< pair<int, int> > factors;
	int n, m, p;

	if (scanf("%d %d %d", &n, &m, &p) != 3)
		return 1;

	for (int i = 0; i < n; i++)
		if (scanf("%d", &a[i]) != 1)
			return 1;

	for (int i = 0; i < m; i++)
		if (scanf("%d", &b[i]) != 1)
			return 1;

	int g = p - 1;
	for (int i = 0; i < m; i++) {
		b[i] %= p - 1;
		g = gcd(g, b[i]);
	}

	compute_factors(p - 1, factors);

	vector<int> divisors;
	map<int, size_t> revdiv;
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
	for (size_t i = 0; i < divisors.size(); i++)
		revdiv[divisors[i]] = i;

	vector<int> v;
	for (int i = 0; i < n; i++) {
		int order = compute_order(a[i], p, p - 1, factors);
		order = order / gcd(g, order);
		v.push_back((p - 1) / order);
	}
	sort(v.begin(), v.end());
	trim_unique(v);

	vector<int> coef(divisors.size());
	for (size_t i = 0; i < v.size(); i++) {
		int d = v[i];
		vector<int>::iterator it = lower_bound(divisors.begin(), divisors.end(), v[i]);
		for (; it != divisors.end(); ++it) {
			int nd = *it;
			if (nd % d == 0) {
				size_t idx = revdiv[nd];
				++coef[idx];
			}
		}
	}

	int result = 0;
	for (size_t i = 0; i < divisors.size(); i++)
		if (coef[i]) {
			factors.clear();
			compute_factors((p - 1) / divisors[i], factors);
			int term = 1;
			for (size_t j = 0; j < factors.size(); j++) {
				term *= ipow(factors[j].first, factors[j].second - 1);
				term *= factors[j].first - 1;
			}
			result += term;
		}
	printf("%d\n", result);

	return 0;
}

int main()
{
	build_primes(MAX_PRIME);
	solve_problem();
	return 0;
}
