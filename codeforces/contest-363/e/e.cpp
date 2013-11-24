#include <algorithm>
#include <iostream>
#include <cstdio>

#define MAX_N 500
#define MAX_M 500
#define MAX_R ((MAX_N - 1) / 2)

using namespace std;

int grid[MAX_N][MAX_M];
int srow[MAX_N][MAX_M];
int scircle[MAX_N][MAX_M];
int rmaxcircle[MAX_N][MAX_M + 1];
int cmaxcircle[MAX_N][MAX_M + 1];
int width[MAX_R + 1];
int separation[2 * MAX_R + 1];

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

int partial_row_sum(int row, int left, int right)
{
	return srow[row][right + 1] - srow[row][left];
}

long long mul_ll(long long x, long long y)
{
	return x * y;
}

int solve_problem()
{
	int n, m, r;

	n = read_unsigned_integer<int>();
	m = read_unsigned_integer<int>();
	r = read_unsigned_integer<int>();

	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			grid[i][j] = read_unsigned_integer<int>();

	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			srow[i][j + 1] = srow[i][j] + grid[i][j];

	width[0] = r;
	for (int i = 1; i <= r; i++) {
		int w = width[i - 1];
		while (i * i + w * w > r * r)
			--w;
		width[i] = w;
	}

	for (int i = 0; i <= 2 * r; i++) {
		int limit = min(i / 2, r);
		for (int j = max(0, i - r); j <= limit; j++) {
			int cand = width[j] + width[i - j] + 1;
			if (cand > separation[i])
				separation[i] = cand;
		}
	}

	for (int i = r; i < n - r; i++)
		for (int j = r; j < m - r; j++)
			for (int k = 0; k <= r; k++) {
				scircle[i][j] += partial_row_sum(i + k, j - width[k], j + width[k]);
				if (k > 0)
					scircle[i][j] += partial_row_sum(i - k, j - width[k], j + width[k]);
			}

	for (int i = r; i < n - r; i++)
		for (int j = m - r - 1; j >= r; j--)
			if (scircle[i][j] <= rmaxcircle[i][j + 1]) {
				rmaxcircle[i][j] = rmaxcircle[i][j + 1];
				cmaxcircle[i][j] = cmaxcircle[i][j + 1];
				if (rmaxcircle[i][j] == scircle[i][j])
					++cmaxcircle[i][j];
			} else {
				rmaxcircle[i][j] = scircle[i][j];
				cmaxcircle[i][j] = 1;
			}

	int result = 0;
	long long mult = 0;
	for (int i = r; i < n - r; i++)
		for (int j = i + 2 * r + 1; j < n - r; j++) {
			int cand = rmaxcircle[i][r] + rmaxcircle[j][r];
			if (cand > result) {
				result = cand;
				mult = mul_ll(cmaxcircle[i][r], cmaxcircle[j][r]);
			} else if (cand == result) {
				mult += mul_ll(cmaxcircle[i][r], cmaxcircle[j][r]);
			}
		}

	for (int i = r; i < n - r; i++)
		for (int j = r; j < m - r; j++) {
			int limit = min(i + 2 * r + 1, n - r);
			for (int k = max(i - 2 * r, r); k < limit; k++) {
				int delta = k - i;
				if (delta < 0)
					delta = -delta;
				int l = j + separation[delta];
				if (l < m - r) {
					int cand = scircle[i][j] + rmaxcircle[k][l];
					if (cand > result) {
						result = cand;
						mult = cmaxcircle[k][l];
					} else if (cand == result) {
						mult += cmaxcircle[k][l];
					}
				}
			}
		}

	cout << result << ' ' << mult << endl;

	return 0;
}

int main()
{
	solve_problem();
	return 0;
}
