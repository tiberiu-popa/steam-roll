#include <algorithm>
#include <limits>
#include <cstdio>
#include <cstdlib>

#define MAX_N 101
#define MAX_M MAX_N

using namespace std;

int matrix[2 * MAX_N + 2][2 * MAX_N + 2];
bool stale[2 * MAX_N + 2];

void gauss_jordan(int n, int m, int row, int col)
{
	int pivot = matrix[row][col];
	if (pivot < 0) {
		for (int j = 1; j <= n + 1; j++)
			matrix[row][j] *= pivot;
		pivot = matrix[row][col];
	}
	for (int i = 0; i <= m + 1; i++)
		if (i != row) {
			int factor = pivot * matrix[i][col];
			for (int k = 1; k <= n + 1; k++)
				matrix[i][k] -= factor * matrix[row][k];
		}
}

void search_solution(int n, int m, int target)
{
	for (;;) {
		int col, row;
		for (col = 1; col <= 2 * n; col++)
			if (!stale[col] && matrix[target][col] > 0)
				break;
		if (col > 2 * n)
			break;
		int min_ratio = numeric_limits<int>::max();
		int min_row = 0;
		for (row = 1; row <= m + n; row++)
			if (matrix[row][col] > 0) {
				int ratio = matrix[row][2 * n + 1] * matrix[row][col];
				if (ratio < min_ratio) {
					min_ratio = ratio;
					min_row = row;
				}
			}
		if (min_row == 0) {
			fprintf(stderr, "An irrecoverable error has occurred\n");
			exit(EXIT_FAILURE);
		}
		gauss_jordan(2 * n, m + n, min_row, col);
	}
}

void solve_problem()
{
	int n, m;

	if (scanf("%d %d", &n, &m) != 2)
		exit(EXIT_FAILURE);

	for (int i = 1; i <= m; i++) {
		int x, y, z;
		if (scanf("%d %d %d", &x, &y, &z) != 3)
			exit(EXIT_FAILURE);
		for (int j = x; j <= y; j++)
			matrix[i][j] = 1;
		matrix[i][2 * n + 1] = z;
	}
	matrix[0][0] = 1;
	matrix[m + n + 1][0] = 1;
	for (int j = 1; j <= n; j++) {
		matrix[0][j] = -1;
		matrix[m + n + 1][j] = 1;
	}
	for (int i = 1; i <= n; i++) {
		matrix[m + i][i] = 1;
		matrix[m + i][n + i] = 1;
		matrix[m + i][2 * n + 1] = 1;
	}

	int row = 1;
	for (int j = 1; j <= 2 * n; j++) {
		int idx;
		for (idx = row; idx <= m + n; idx++)
			if (matrix[idx][j] != 0)
				break;
		if (idx <= m + n) {
			if (idx > row) {
				for (int k = 1; k <= 2 * n + 1; k++)
					swap(matrix[row][k], matrix[idx][k]);
			}
			gauss_jordan(2 * n, m + n, row, j);
			row++;
		} else {
			bool is_zero = true;
			for (int i = 1; i < row && i <= m + n; i++)
				if (matrix[i][j] != 0) {
					is_zero = false;
					break;
				}
			if (is_zero)
				stale[j] = true;
		}
	}

	search_solution(n, m, 0);
	int min_value = matrix[0][2 * n + 1];

	search_solution(n, m, m + n + 1);
	int max_value = -matrix[m + n + 1][2 * n + 1];

	printf("%d %d\n", min_value, max_value);
}

int main()
{
	solve_problem();
	return 0;
}
