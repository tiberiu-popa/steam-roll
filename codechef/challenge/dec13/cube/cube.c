#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_N 40
#define BUFFER_SIZE (MAX_N * MAX_N * MAX_N)

int grid[MAX_N][MAX_N][MAX_N];
int sum[MAX_N + 1][MAX_N + 1][MAX_N + 1];
char str[BUFFER_SIZE + 10];

char *trim_line(char *str)
{
	return strtok(str, "\r\n");
}

int partial_sum(int p, int q, int r, int x, int y, int z)
{
	int a = sum[p][y + 1][z + 1] + sum[x + 1][q][z + 1] + sum[x + 1][y + 1][r];
	int b = sum[x + 1][q][r] + sum[p][y + 1][r] + sum[p][q][z + 1];
	return sum[x + 1][y + 1][z + 1] - a + b - sum[p][q][r];
}

int count_common(int n, int m, int s)
{
	int cube_size = s * s * s;
	int num_cubes = 0;
	int i, j, k;
	for (i = 0; i + s <= n; i++)
		for (j = 0; j + s <= n; j++)
			for (k = 0; k + s <= n; k++) {
				int t = partial_sum(i, j, k, i + s - 1, j + s - 1, k + s - 1);
				if (100 * t >= m * cube_size)
					++num_cubes;
			}
	return num_cubes;
}

int solve_problem()
{
	int idx, result;
	int n, m;
	int diff;
	int i, j, k, l;

	if (fgets(str, sizeof(str), stdin) == NULL)
		return 1;
	trim_line(str);
	char *p = strtok(str, " ");
	n = atoi(p);
	p = strtok(NULL, " ");
	m = atoi(p);

	for (l = 0; l < 2; l++) {
		if (fgets(str, sizeof(str), stdin) == NULL)
			return 1;
		trim_line(str);
		for (i = 0, idx = 0; i < n; i++)
			for (j = 0; j < n; j++)
				for (k = 0; k < n; k++) {
					diff = str[idx++] - 'a';
					if (l == 0)
						grid[i][j][k] = diff;
					else
						grid[i][j][k] = (grid[i][j][k] == diff);
				}
	}

	for (i = 1; i <= n; i++) {
		for (j = 1; j <= n; j++)
			for (k = 0; k < n; k++)
				sum[i][j][k + 1] = sum[i][j][k] + grid[i - 1][j - 1][k];
		for (j = 1; j <= n; j++)
			for (k = 1; k <= n; k++)
				sum[i][j][k] += sum[i][j - 1][k];
		for (j = 1; j <= n; j++)
			for (k = 1; k <= n; k++)
				sum[i][j][k] += sum[i - 1][j][k];
	}

	idx = result = -1;
	for (i = n; i > 0; i--) {
		int cand = count_common(n, m, i);
		if (cand > 0) {
			result = cand;
			idx = i;
			break;
		}
	}
	if (idx >= 0)
		printf("%d %d\n", idx, result);
	else
		printf("%d\n", idx);

	return 0;
}

int main()
{
	int i, num_tests;

	if (fgets(str, sizeof(str), stdin) == NULL)
		return 1;
	num_tests = atoi(str);

	for (i = 0; i < num_tests; i++)
		solve_problem();

	return 0;
}
