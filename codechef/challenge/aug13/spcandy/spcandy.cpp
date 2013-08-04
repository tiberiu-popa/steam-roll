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

int solve_problem()
{
	long long n, k;

	n = read_unsigned_integer<long long>();
	k = read_unsigned_integer<long long>();

	if (k == 0) {
		printf("%lld %lld\n", 0LL, n);
	} else {
		printf("%lld %lld\n", n / k, n % k);
	}

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
