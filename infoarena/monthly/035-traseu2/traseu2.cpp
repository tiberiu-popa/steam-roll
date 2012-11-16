#include <algorithm>
#include <limits>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#define MAX_N 70
#define MAX_M 70

using namespace std;

int grid[MAX_N][MAX_M];
int x[MAX_N * MAX_M];
int y[MAX_N * MAX_M];

void read_input(const char *filename, int &n, int &m)
{
	FILE *f;
	char line[11];

	f = fopen(filename, "rt");
	if (f == NULL) {
		fprintf(stderr, "Could not open file: %s\n", filename);
		exit(EXIT_FAILURE);
	}

	if (fscanf(f, "%d %d", &n, &m) != 2)
		exit(EXIT_FAILURE);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++) {
			if (fscanf(f, "%s", line) != 1)
				exit(EXIT_FAILURE);
			if (strcmp(line, "#") == 0)
				grid[i][j] = -1;
			else
				grid[i][j] = atoi(line);
		}
	fclose(f);
}

void write_output(const char *filename, int result)
{
	FILE *f;

	f = fopen(filename, "wt");
	if (f == NULL) {
		fprintf(stderr, "Could not open file: %s\n", filename);
		exit(EXIT_FAILURE);
	}

	if (result >= 0) {
		fprintf(f, "Exista solutie!\n");
		fprintf(f, "%d\n", result);
		for (int i = 0; i < result; i++)
			fprintf(f, "%d %d %d\n", x[i] + 1, y[i] + 1, grid[x[i]][y[i]]);
	} else {
		fprintf(f, "Nu exista solutie!\n");
	}
	fclose(f);
}

int solve_problem(int n, int m)
{
	int num_valid = 0;
	int min_ipos = -1, min_jpos = -1;
	int min_elem = numeric_limits<int>::max();

	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			if (grid[i][j] >= 0) {
				num_valid++;
				if (grid[i][j] < min_elem) {
					min_elem = grid[i][j];
					min_ipos = i;
					min_jpos = j;
				}
			}

	int u = min_ipos;
	int v = min_jpos;
	x[0] = u;
	y[0] = v;
	for (int k = 1; k < num_valid; k++) {
		int best_value = numeric_limits<int>::max();
		int bu = -1, bv = -1;
		for (int i = -1; i <= 1; i++) {
			int nu = u + i;
			if (nu >= 0 && nu < n) {
				for (int j = -1; j <= 1; j++)
					if (i != 0 || j != 0) {
						int nv = v + j;
						if (nv >= 0 && nv < m) {
							if (grid[nu][nv] >= grid[u][v] && grid[nu][nv] < best_value) {
								best_value = grid[nu][nv];
								bu = nu;
								bv = nv;
							}
						}
					}
			}
		}
		if (bu == -1 || bv == -1)
			return -1;
		x[k] = u = bu;
		y[k] = v = bv;
	}

	return num_valid;
}

int main()
{
	int n, m;
	int result;

	read_input("traseu2.in", n, m);
	result = solve_problem(n, m);
	write_output("traseu2.out", result);

	return 0;
}
