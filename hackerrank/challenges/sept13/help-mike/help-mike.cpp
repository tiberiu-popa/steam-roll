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

long long ll_mul(long long x, long long y)
{
	return x * y;
}

int solve_problem()
{
	int n, k;

	n = read_unsigned_integer<int>();
	k = read_unsigned_integer<int>();

	long long result = 0;
	for (int i = 1; i <= k; i++) {
		int j = k - i;
		if (j == 0)
			j = k;
		int x = (n - i + k) / k;
		int y = (n - j + k) / k;
		if (i != j)
			result += ll_mul(x, y);
		else
			result += ll_mul(x, x - 1);
	}
	printf("%lld\n", result / 2);

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
