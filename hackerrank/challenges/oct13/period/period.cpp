#include <algorithm>
#include <vector>
#include <cstdio>
#include <cstring>

#define MAX_PRIME 3163

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

int addmod(int x, int y, int p)
{
	long long sum = x;
	sum += y;
	if (sum >= p)
		sum -= p;
	return (int) sum;
}

int submod(int x, int y, int p)
{
	long long diff = x;
	diff -= y;
	if (diff < 0)
		diff += p;
	return (int) diff;
}

int mulmod(int x, int y, int p)
{
	long long prod = x;
	prod = (prod * y) % p;
	return (int) prod;
}

int sqrmod(int x, int p)
{
	return mulmod(x, x, p);
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

int invmod(int x, int p)
{
	int result[2][2] = { {1, 0}, {0, 1} };
	int base[2][2] = { {0, 1}, {1, 0}};

	int y = x % p;
	x = p;
	while (y > 0) {
		int q = x / y;
		int r = x % y;
		base[1][1] = -q;
		int tmp[2][2] = { {0, 0}, {0, 0} };
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 2; j++)
				for (int k = 0; k < 2; k++)
					tmp[i][j] += result[i][k] * base[k][j];
		memcpy(result, tmp, sizeof(result));
		x = y;
		y = r;
	}

	int inv = result[1][0];
	if (inv < 0)
		inv += p;
	return inv;
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

int compute_phi(int n)
{
	vector< pair<int, int> > factors;
	int q = n;

	compute_factors(n, factors);
	for (size_t i = 0; i < factors.size(); i++) {
		int p = factors[i].first;
		q /= p;
		q *= p - 1;
	}

	return q;
}

int compute_order(int x, int p, int phi)
{
	vector< pair<int, int> > factors;
	int order = 1;

	compute_factors(phi, factors);
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

pair<int, int> mulmod_sqrt5(pair<int, int> x, pair<int, int> y, int p)
{
	int t = mulmod(x.second, y.second, p);
	int u = addmod(mulmod(x.first, y.first, p), mulmod(5, t, p), p);
	int v = addmod(mulmod(x.first, y.second, p), mulmod(x.second, y.first, p), p);
	return make_pair(u, v);
}

pair<int, int> powmod_sqrt5(pair<int, int> x, int e, int p)
{
	pair<int, int> result(1, 0);
	pair<int, int> base(x);

	if (e > 0) {
		while (true) {
			if (e & 1) {
				result = mulmod_sqrt5(result, base, p);
			}
			e >>= 1;
			if (e <= 0)
				break;
			base = mulmod_sqrt5(base, base, p);
		}
	}

	return result;
}

int solve_problem()
{
	int a, b, m, q;

	a = read_unsigned_integer<int>();
	b = read_unsigned_integer<int>();
	m = read_unsigned_integer<int>();

	q = compute_phi(m);

	int asq = sqrmod(a, m);
	int bsq = sqrmod(b, m);
	int norm = submod(asq, mulmod(5, bsq, m), m);

	long long result = -1;
	if (gcd(norm, m) == 1) {
		int order = compute_order(norm, m, q);
		pair<int, int> base(a, b);
		base = powmod_sqrt5(base, order, m);
		pair<int, int> accum(base);
		result = 1;
		while (accum.first != 1 || accum.second != 0) {
			accum = mulmod_sqrt5(accum, base, m);
			++result;
		}
		result *= order;
	}
	printf("%lld\n", result);

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
