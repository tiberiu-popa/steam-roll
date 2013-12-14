#include <algorithm>
#include <vector>
#include <cstdio>

#define MAX_N 30000
#define MAX_PRIME 173 // sqrt(N)
#define MAX_NUM_DIVISORS 96

using namespace std;

vector<int> primes;
bool prime[MAX_PRIME + 1];
vector<int> p[MAX_NUM_DIVISORS];
vector<int> q;

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

char read_character()
{
	return skip_whitespace();
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

int is_prime(int n)
{
	if (n <= MAX_PRIME) {
		return prime[MAX_PRIME];
	} else {
		for (size_t i = 0; i < primes.size(); i++) {
			int p = primes[i];
			if (p * p > n)
				break;
			if (n % p == 0)
				return 0;
		}
		return 1;
	}
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

template<typename T>
void trim_poly(vector<T> &v)
{
	size_t i;
	for (i = v.size(); i > 0 && v[--i] == 0;) { }
	v.resize(i + 1);
}

template<typename T>
void addpoly(const vector<T> &u, const vector<T> &v, vector<T> &s)
{
	size_t n = min(u.size(), v.size());
	for (size_t i = 0; i < n; i++)
		s.push_back(u[i] + v[i]);
	for (size_t i = n; i < u.size(); i++)
		s.push_back(u[i]);
	for (size_t i = n; i < v.size(); i++)
		s.push_back(v[i]);
	trim_poly(s);
}

template<typename T>
void add_to_poly(const vector<T> &u, vector<T> &s)
{
	size_t n = min(u.size(), s.size());
	for (size_t i = 0; i < n; i++)
		s[i] += u[i];
	for (size_t i = n; i < u.size(); i++)
		s.push_back(u[i]);
	trim_poly(s);
}

template<typename T>
void subpoly(const vector<T> &u, const vector<T> &v, vector<T> &s)
{
	size_t n = min(u.size(), v.size());
	for (size_t i = 0; i < n; i++)
		s.push_back(u[i] - v[i]);
	for (size_t i = n; i < u.size(); i++)
		s.push_back(u[i]);
	for (size_t i = n; i < v.size(); i++)
		s.push_back(-v[i]);
	trim_poly(s);
}

// monic polynomials only
template<typename T>
void divpoly(const vector<T> &u, const vector<T> &v, vector<T> &q, vector<T> &r)
{
	size_t n = u.size() - 1;
	size_t m = v.size() - 1;
	r = u;
	if (n >= m) {
		q.resize(n - m + 1);
		for (size_t k = n - m + 1; k > 0;) {
			--k;
			q[k] = r[m + k] / v[m];
			for (size_t j = m + k; j > k;) {
				--j;
				r[j] -= q[k] * v[j - k];
			}
		}
		r.resize(m);
		trim_poly(r);
		if (r.empty())
			r.push_back(0);
	} else {
		q.push_back(0);
	}
}

int solve_problem()
{
	vector< pair<int, int> > factors;
	vector<int> divisors;
	vector<int> v[2];
	vector<int> r, s, t;
	int n;

	n = read_unsigned_integer<int>();

	compute_factors(n, factors);
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

	p[0].clear();
	p[0].push_back(-1);
	p[0].push_back(1);
	for (size_t i = 1; i < divisors.size(); i++) {
		size_t j;
		int m = divisors[i];
		int next = 0;

		v[next].resize((size_t) m);
		fill(v[next].begin(), v[next].end(), 1);
		for (j = i; j > 0;) {
			--j;
			if (divisors[i] % divisors[j] == 0)
				break;
		}
		if (j > 0) {
			size_t k = j;
			int e = divisors[j];
			s.resize((size_t) e);
			fill(s.begin(), s.end(), 1);
			int prev = next ^ 1;
			v[prev].clear();
			r.clear();
			divpoly(v[next], s, v[prev], r);
			next = prev;

			while (j > 1) {
				--j;
				if (divisors[i] % divisors[j] == 0 && divisors[k] % divisors[j] != 0) {
					prev = next ^ 1;
					v[prev].clear();
					r.clear();
					divpoly(v[next], p[j], v[prev], r);
					next = prev;
				}
			}
		}
		p[i] = v[next];
	}

	q.clear();
	for (int i = 0; i < n; i++) {
		int x = read_signed_integer<int>();
		q.push_back(x);
	}

	bool found = false;
	for (size_t i = 0; i < divisors.size(); i++) {
		s.clear();
		t.clear();
		divpoly(q, p[i], s, t);
		if (t.size() == 1 && t.back() == 0) {
			found = true;
			break;
		}
	}
	printf("%s\n", found ? "YES" : "NO");

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
