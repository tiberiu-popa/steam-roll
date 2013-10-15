#include <algorithm>
#include <cstdio>

using namespace std;

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

int solve_problem()
{
	int n;

	n = read_unsigned_integer<int>();

	long long x = 0;
	for (int i = n, j = 1; i > 0;) {
		int k = n / i;
		x += (k - j + 1) * ((long long) i);
		j = k + 1;
		i = n / j;
	}
	long long y = n;
	y *= n;
	long long g = gcd(x, y);
	printf("%lld/%lld\n", x / g, y / g);

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
