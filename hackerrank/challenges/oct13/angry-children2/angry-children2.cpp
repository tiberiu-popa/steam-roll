#include <algorithm>
#include <limits>
#include <cstdio>

#define MAX_N 100000

using namespace std;

int v[MAX_N];
long long sum[MAX_N + 1];

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
	int n, k;

	n = read_unsigned_integer<int>();
	k = read_unsigned_integer<int>();
	for (int i = 0; i < n; i++)
		v[i] = read_unsigned_integer<int>();

	sort(v, v + n);

	sum[0] = 0;
	for (int i = 0; i < n; i++)
		sum[i + 1] = sum[i] + v[i];

	long long crt = 0;
	for (int i = 0; i < k; i++)
		crt += (-k + 1 + 2 * i) * ((long long) v[i]);
	long long result = crt;
	for (int i = 1; i + k < n; i++) {
		long long cand = crt + (k - 1) * ((long long) (v[i - 1] + v[i + k - 1]));
		cand -= 2 * (sum[i + k - 1] - sum[i]);
		if (cand < result)
			result = cand;
		crt = cand;
	}
	printf("%lld\n", result);

	return 0;
}

int main()
{
	solve_problem();
	return 0;
}
