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

template<typename T>
int get_lower_exp(T n)
{
	int e = 0;
	T k = (T) 1;
	while ((k << 1) <= n) {
		k <<= 1;
		++e;
	}
	return e;
}

int i_usqrt(int n)
{
	int e = get_lower_exp(n);
	int left = 1, right = 1 << (e / 2 + 1);
	while (left < right) {
		int mid = left + (right - left) / 2;
		int sqm = mid * mid;
		if (sqm == n)
			return mid;
		else if (sqm < n)
			left = mid + 1;
		else
			right = mid;
	}
	return left;
}

int solve_problem()
{
	int n;

	n = read_unsigned_integer<int>();

	long long x = 0;
	int root = i_usqrt(n);
	int invroot = n / root;
	int min_idx = min(root - 1, invroot);
	int max_idx = max(root - 1, invroot);

	int i = 2;
	for (; i <= min_idx; i++)
		x += n / i;
	x = 2 * x + n;
	for (; i <= max_idx; i++)
		x += n / i;
	if (root > 1)
		x += n - n / root * ((long long) (root - 1));

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
