#include <algorithm>
#include <functional>
#include <cstdio>

#define MAX_N 100000

using namespace std;

int v[MAX_N];

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

int solve_problem()
{
	long long op_cost;
	int n;

	n = read_unsigned_integer<int>();
	for (int i = 0; i < n;) {
		int elem = read_signed_integer<int>();
		if (elem < 0) {
			v[i] = elem;
			++i;
		} else {
			--n;
		}
	}
	op_cost = read_unsigned_integer<long long>();

	sort(v, v + n, greater<int>());

	long long result = 0LL;
	int m = n - op_cost;
	if (m < 0) {
		for (int i = 0; i < n; i++)
			result -= v[i];
	} else {
		while (m + 1 < n && v[m + 1] == v[m])
			++m;

		if (m < n)
			result -= op_cost * v[m];
		for (int i = m + 1; i < n; i++)
			result += v[m] - v[i];
	}

	printf("%lld\n", result);

	return 0;
}

int main()
{
	solve_problem();

	return 0;
}
