#include <algorithm>
#include <limits>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

#define MAX_PRIME 44721

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

bool is_quadratic_residue(int a, int p)
{
	return a == 0 || powmod(a, (p - 1) / 2, p) == 1;
}

int find_non_residue(int p)
{
	while (true) {
		int x = 1 + rand() % (p - 1);
		if (!is_quadratic_residue(x, p))
			return x;
	}
	return -1;
}

int tonelli_shanks(int n, int p)
{
	if (n == 0)
		return 0;

	if (p % 3 == 4) {
		return powmod(n, (p + 1) / 4, p);
	} else {
		int q = p - 1, s = 0;
		while (q % 2 == 0) {
			s++;
			q >>= 1;
		}
		int z = find_non_residue(p);
		int c = powmod(z, q, p);
		int r = powmod(n, (q + 1) / 2, p);
		int t = powmod(n, q, p);
		int m = s;
		while (true) {
			if (t == 1)
				return r;
			int i = 0;
			for (int x = t; x != 1; x = sqrmod(x, p))
				i++;
			int b = powmod(c, powmod(2, m - i - 1, p - 1), p);
			r = mulmod(r, b, p);
			c = sqrmod(b, p);
			t = mulmod(t, c, p);
			m = i;
		}
		return -1;
	}
}

void update_xab(int& x, int& a, int& b, int alpha, int beta, int p, int n)
{
	switch (x % 3) {
	case 0:
		x = mulmod(x, x, p);
		a = mulmod(a, 2, n);
		b = mulmod(b, 2, n);
		break;

	case 1:
		x = mulmod(x, alpha, p);
		a = addmod(a, 1, n);
		break;

	case 2:
		x = mulmod(x, beta, p);
		b = addmod(b, 1, n);
		break;
	}
}

int pollard_rho(int alpha, int beta, int p, int n, int parity)
{
	int x = 1, a = 0, b = 0;
	int X = x, A = a, B = b;
	for (int i = 1; i < p - 1; ++i) {
		update_xab(x, a, b, alpha, beta, p, n);
		update_xab(X, A, B, alpha, beta, p, n);
		update_xab(X, A, B, alpha, beta, p, n);
		if (x == X)
			break;
	}
	int m = n;
	int left = submod(a, A, n);
	int right = submod(B, b, n);
	int g = gcd(m, right);
	m /= g;
	right /= g;
	left /= g;
	int invright = invmod(right, m);
	int result = mulmod(left, invright, m);
	while (result < p && (powmod(alpha, result, p) != beta || result % 2 != parity))
		result += m;
	if (result < p)
		return result;
	else
		return -1;
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

int compute_order(int x, int p)
{
	vector< pair<int, int> > factors;
	int order = 1;

	compute_factors(p - 1, factors);
	for (size_t i = 0; i < factors.size(); i++) {
		int e = factors[i].second;
		int j = 0;
		for (int q = 1; j <= e; j++) {
			if (j > 0)
				q *= factors[i].first;
			if (powmod(x, (p - 1) / q, p) != 1)
				break;
		}
		order *= ipow(factors[i].first, e - (j - 1));
	}

	return order;
}

int solve_problem()
{
	int delta[2];
	int c, p;

	c = read_unsigned_integer<int>();
	p = read_unsigned_integer<int>();

	int sq5 = tonelli_shanks(5, p);
	int inv2 = (p + 1) / 2;
	int base = mulmod(sq5, c, p);

	int golden = mulmod(addmod(1, sq5, p), inv2, p);
	int order = compute_order(golden, p);

	int result = p;
	int alpha = mulmod(5, sqrmod(c, p), p);
	for (int i = 0; i < 2; i++) {
		int beta = addmod(alpha, i ? (p - 4) : 4, p);
		if (is_quadratic_residue(beta, p)) {
			int gamma = tonelli_shanks(beta, p);
			delta[0] = mulmod(addmod(base, gamma, p), inv2, p);
			delta[1] = mulmod(submod(base, gamma, p), inv2, p);
			for (int j = 0; j < 2; j++) {
				int f = pollard_rho(golden, delta[j], p, order, i);
				if (f >= 0 && f < result)
					result = f;
			}
		}
	}

	printf("%d\n", (result == p) ? -1 : result);

	return 0;
}

int main()
{
	int num_tests;

	srand(time(NULL));
	build_primes(MAX_PRIME);

	num_tests = read_unsigned_integer<int>();
	for (int i = 0; i < num_tests; i++)
		solve_problem();

	return 0;
}
