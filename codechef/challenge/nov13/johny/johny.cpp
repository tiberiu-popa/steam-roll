#include <algorithm>
#include <cstdio>

#define MAX_N 100

using namespace std;

pair<int, int> a[MAX_N];

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
	for (int i = 0; i < n; i++)
		a[i] = make_pair(read_unsigned_integer<int>(), i + 1);
	k = read_unsigned_integer<int>();

	sort(a, a + n);

	int result = 0;
	for (int i = 0; i < n; i++)
		if (a[i].second == k) {
			result = i + 1;
			break;
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
