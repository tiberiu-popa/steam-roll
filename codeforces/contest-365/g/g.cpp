#include <algorithm>
#include <iostream>
#include <map>
#include <vector>
#include <cstdio>
#include <cstring>

#define BUFFER_SIZE 2500
#define MAX_N 2500
#define MAX_M 2500
#define MAX_K 6

using namespace std;

int grid[MAX_N][MAX_M];
int s[MAX_N + 1][MAX_M + 1];
char str[BUFFER_SIZE + 10];
long long result = 0;

char *trim_line(char *str)
{
	return strtok(str, "\r\n");
}

long long mul_ll(long long x, long long y)
{
	return x * y;
}

int count_ones(int lx, int ly, int rx, int ry)
{
	return s[rx + 1][ry + 1] - s[lx][ry + 1] - s[rx + 1][ly] + s[lx][ly];
}

void process_x_left(int lx, int ly, int rx, int ry, int k, int *left, bool initial)
{
	int end = rx;
	left[0] = end;
	for (int i = 1; i <= k + 1; i++) {
		int start = initial ? (lx - 1) : left[i];
		while (start < end) {
			int mid = start + (end - start + 1) / 2;
			int nr = count_ones(mid, ly, rx, ry);
			if (nr >= i)
				start = mid;
			else
				end = mid - 1;
		}
		left[i] = start;
	}
}

void process_x_right(int lx, int ly, int rx, int ry, int k, int *right, bool initial)
{
	int start = lx;
	right[0] = start;
	for (int i = 1; i <= k + 1; i++) {
		int end = initial ? (rx + 1) : right[i];
		while (start < end) {
			int mid = start + (end - start) / 2;
			int nr = count_ones(lx, ly, mid, ry);
			if (nr >= i)
				end = mid;
			else
				start = mid + 1;
		}
		right[i] = start;
	}
}

void process_y_left(int lx, int ly, int rx, int ry, int k, int *left, bool initial)
{
	int end = ry;
	left[0] = end;
	for (int i = 1; i <= k + 1; i++) {
		int start = initial ? (ly - 1) : left[i];
		while (start < end) {
			int mid = start + (end - start + 1) / 2;
			int nr = count_ones(lx, mid, rx, ry);
			if (nr >= i)
				start = mid;
			else
				end = mid - 1;
		}
		left[i] = start;
	}
}

void process_y_right(int lx, int ly, int rx, int ry, int k, int *right, bool initial)
{
	int start = ly;
	right[0] = start;
	for (int i = 1; i <= k + 1; i++) {
		int end = initial ? (ry + 1) : right[i];
		while (start < end) {
			int mid = start + (end - start) / 2;
			int nr = count_ones(lx, ly, rx, mid);
			if (nr >= i)
				end = mid;
			else
				start = mid + 1;
		}
		right[i] = start;
	}
}

void kdtree(int lx, int ly, int rx, int ry, int k)
{
	int left[MAX_K + 2];
	int right[MAX_K + 2];
	int dx = rx - lx;
	int dy = ry - ly;

	if (dx >= dy) {
		if (dy == 0) {
			map<int, int> cnt;
			cnt[0] = 1;
			for (int i = lx; i <= rx; i++) {
				int nr = count_ones(lx, ly, i, ry);
				if (nr >= k)
					result += cnt[nr - k];
				++cnt[nr];
			}
		} else {
			int mx = lx + (rx - lx) / 2;
			for (int i = ly; i <= ry; i++)
				for (int j = i; j <= ry; j++) {
					process_x_left(lx, i, mx, j, k, left, (i == j));
					process_x_right(mx + 1, i, rx, j, k, right, (i == j));
					for (int l = 0; l <= k; l++) {
						int a = left[l] - left[l + 1];
						int b = right[k - l + 1] - right[k - l];
						result += a * b;
					}
				}

			kdtree(lx, ly, mx, ry, k);
			kdtree(mx + 1, ly, rx, ry, k);
		}
	} else {
		if (dx == 0) {
			map<int, int> cnt;
			cnt[0] = 1;
			for (int i = ly; i <= ry; i++) {
				int nr = count_ones(lx, ly, rx, i);
				if (nr >= k)
					result += cnt[nr - k];
				++cnt[nr];
			}
		} else {
			int my = ly + (ry - ly) / 2;
			for (int i = lx; i <= rx; i++)
				for (int j = i; j <= rx; j++) {
					process_y_left(i, ly, j, my, k, left, (i == j));
					process_y_right(i, my + 1, j, ry, k, right, (i == j));
					for (int l = 0; l <= k; l++) {
						int a = left[l] - left[l + 1];
						int b = right[k - l + 1] - right[k - l];
						result += a * b;
					}
				}

			kdtree(lx, ly, rx, my, k);
			kdtree(lx, my + 1, rx, ry, k);
		}
	}
}

int solve_problem()
{
	int n, m, k;

	if (fgets(str, sizeof(str), stdin) == NULL)
		return 1;
	n = atoi(strtok(str, " "));
	m = atoi(strtok(NULL, " "));
	k = atoi(strtok(NULL, " "));

	for (int i = 0; i < n; i++) {
		if (fgets(str, sizeof(str), stdin) == NULL)
			return 1;
		for (int j = 0; j < m; j++)
			grid[i][j] = str[j] - '0';
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++)
			s[i + 1][j + 1] = s[i + 1][j] + grid[i][j];
		for (int j = 0; j < m; j++)
			s[i + 1][j + 1] += s[i][j + 1];
	}

	kdtree(0, 0, n - 1, m - 1, k);
	cout << result << endl;

	return 0;
}

int main()
{
	solve_problem();
	return 0;
}
