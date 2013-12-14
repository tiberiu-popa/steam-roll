#include <algorithm>
#include <vector>
#include <cstdio>

#define MAX_N 300
#define NUM_VALUES 10

using namespace std;

int grid[MAX_N][MAX_N];
int sum[NUM_VALUES][MAX_N + 1][MAX_N + 1];

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

char read_character()
{
	return skip_whitespace();
}

int solve_problem()
{
	int n, q;

	n = read_unsigned_integer<int>();

	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			grid[i][j] = read_unsigned_integer<int>() - 1;

	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++) {
			int val = grid[i][j];
			for (int k = 0; k < NUM_VALUES; k++)
				sum[k][i + 1][j + 1] = sum[k][i + 1][j] + (k == val);
		}

	for (int k = 0; k < NUM_VALUES; k++)
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++)
				sum[k][i + 1][j + 1] += sum[k][i][j + 1];

	q = read_unsigned_integer<int>();
	for (int i = 0; i < q; i++) {
		int x1, y1, x2, y2;
		x1 = read_unsigned_integer<int>() - 1;
		y1 = read_unsigned_integer<int>() - 1;
		x2 = read_unsigned_integer<int>() - 1;
		y2 = read_unsigned_integer<int>() - 1;
		int num_distinct = 0;
		for (int k = 0; k < NUM_VALUES; k++) {
			int d = sum[k][x2 + 1][y2 + 1] + sum[k][x1][y1]
				- (sum[k][x1][y2 + 1] + sum[k][x2 + 1][y1]);
			if (d > 0)
				++num_distinct;
		}
		printf("%d\n", num_distinct);
	}

	return 0;
}

int main()
{
	solve_problem();
	return 0;
}
