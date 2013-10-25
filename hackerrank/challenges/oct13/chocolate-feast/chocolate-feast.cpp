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

int solve_problem()
{
	int n, c, m;

	n = read_unsigned_integer<int>();
	c = read_unsigned_integer<int>();
	m = read_unsigned_integer<int>();

	int num_wrappers = n / c;
	int result = num_wrappers;
	while (num_wrappers >= m) {
		int num_chocolates = num_wrappers / m;
		result += num_chocolates;
		num_wrappers -= m * num_chocolates;
		num_wrappers += num_chocolates;
	}
	printf("%d\n", result);

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
