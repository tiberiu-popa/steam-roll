#include <algorithm>
#include <iostream>
#include <cstdio>

#define MAX_N 1000

using namespace std;

pair<int, int> v[MAX_N];

int quadrant(pair<int, int> p)
{
	if (p.first > 0 && p.second >= 0)
		return 0;
	else if (p.first <= 0 && p.second > 0)
		return 1;
	else if (p.first < 0 && p.second <= 0)
		return 2;
	else
		return 3;
}

long long mul_ll(long long x, long long y)
{
	return x * y;
}

struct comparator {

	bool operator() (pair<int, int> lhs, pair<int, int> rhs) const
	{
		int p = quadrant(lhs);
		int q = quadrant(rhs);
		int swp;

		if (p != q)
			return p < q;
		switch (p) {
		case 1:
			swp = -lhs.first;
			lhs.first = lhs.second;
			lhs.second = swp;
			swp = -rhs.first;
			rhs.first = rhs.second;
			rhs.second = swp;
			break;

		case 2:
			lhs.first = -lhs.first;
			lhs.second = -lhs.second;
			rhs.first = -rhs.first;
			rhs.second = -rhs.second;
			break;

		case 3:
			swp = lhs.first;
			lhs.first = -lhs.second;
			lhs.second = swp;
			swp = rhs.first;
			rhs.first = -rhs.second;
			rhs.second = swp;
			break;
		}

		return mul_ll(lhs.second, rhs.first) < mul_ll(rhs.second, lhs.first);
	}

};

#ifdef _WIN32
int getchar_unlocked()
{
	return getchar();
}
#endif

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

int solve_problem()
{
	int n;

	n = read_unsigned_integer<int>();

	for (int i = 0; i < n; i++)
		v[i].first = read_signed_integer<int>();

	for (int i = 0; i < n; i++)
		v[i].second = read_signed_integer<int>();

	pair<int, int> z;
	for (int i = 0; i < n;)
		if (v[i] == z) {
			--n;
			swap(v[i], v[n]);
		} else {
			++i;
		}

	sort(v, v + n, comparator());

	long long result = 0;
	for (int i = 0; i < n; i++) {
		int tx = 0, ty = 0;
		for (int j = 0, k = i; j < n; j++, k = (k + 1) % n) {
			tx += v[k].first;
			ty += v[k].second;
			long long cand = mul_ll(tx, tx) + mul_ll(ty, ty);
			if (cand > result)
				result = cand;
		}
	}
	cout << result << endl;

	return 0;
}

int main()
{
	int num_tests;

	num_tests = read_unsigned_integer<int>();
	for (int i = 0; i < num_tests; i++)
		solve_problem();

	return 0;
}
