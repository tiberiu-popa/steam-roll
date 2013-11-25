#include <algorithm>
#include <limits>
#include <cstdio>

#define MAX_N 2000
#define MAX_M 2000
#define MAX_K 9

using namespace std;

int qtable[MAX_K][MAX_K];
int x_min[MAX_K];
int x_max[MAX_K];
int y_min[MAX_K];
int y_max[MAX_K];

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
	int n, m, k, s;

	n = read_unsigned_integer<int>();
	m = read_unsigned_integer<int>();
	k = read_unsigned_integer<int>();
	s = read_unsigned_integer<int>();

	fill(x_min, x_min + k, numeric_limits<int>::max());
	fill(x_max, x_max + k, numeric_limits<int>::min());
	fill(y_min, y_min + k, numeric_limits<int>::max());
	fill(y_max, y_max + k, numeric_limits<int>::min());

	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++) {
			int t = read_unsigned_integer<int>() - 1;
			int x = i + j;
			int y = i - j;
			if (x < x_min[t])
				x_min[t] = x;
			if (x > x_max[t])
				x_max[t] = x;
			if (y < y_min[t])
				y_min[t] = y;
			if (y > y_max[t])
				y_max[t] = y;
		}

	int prev_q = -1;
	for (int i = 0; i < s; i++) {
		int q = read_unsigned_integer<int>() - 1;
		if (i > 0)
			qtable[min(prev_q, q)][max(prev_q, q)] = 1;
		prev_q = q;
	}

	int result = 0;
	for (int i = 0; i < k; i++)
		for (int j = i; j < k; j++)
			if (qtable[i][j]) {
				int cand = abs(x_min[i] - x_max[j]);
				if (cand > result)
					result = cand;
				cand = abs(x_max[i] - x_min[j]);
				if (cand > result)
					result = cand;
				cand = abs(y_min[i] - y_max[j]);
				if (cand > result)
					result = cand;
				cand = abs(y_max[i] - y_min[j]);
				if (cand > result)
					result = cand;
			}

	printf("%d\n", result);

	return 0;
}

int main()
{
	solve_problem();
	return 0;
}
