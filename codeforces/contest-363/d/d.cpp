#include <algorithm>
#include <functional>
#include <cstdio>

#define MAX_N 100000

using namespace std;

int b[MAX_N];
int p[MAX_N];

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

int solve_problem()
{
	int n, m, a;

	n = read_unsigned_integer<int>();
	m = read_unsigned_integer<int>();
	a = read_unsigned_integer<int>();

	for (int i = 0; i < n; i++)
		b[i] = read_unsigned_integer<int>();

	sort(b, b + n, greater<int>());

	for (int i = 0; i < m; i++)
		p[i] = read_unsigned_integer<int>();

	sort(p, p + m);

	int left = 0;
	int right = min(n, m);
	while (left < right) {
		int mid = left + (right - left + 1) / 2;
		long long s = 0;
		for (int i = 0; i < mid; i++)
			s += max(p[i] - b[mid - 1 - i], 0);
		if (s > a)
			right = mid - 1;
		else
			left = mid;
	}

	int own = 0;
	for (int i = 0; i < left; i++) {
		own += min(p[i], b[left - 1 - i]);
		a -= max(p[i] - b[left - 1 - i], 0);
	}
	printf("%d %d\n", left, max(own - a, 0));

	return 0;
}

int main()
{
	solve_problem();
	return 0;
}
