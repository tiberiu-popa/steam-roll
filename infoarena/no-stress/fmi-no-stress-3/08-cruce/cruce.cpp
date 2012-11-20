#include <algorithm>
#include <deque>
#include <cstdio>
#include <cstdlib>

using namespace std;

#define MAX_N 100
#define MAX_M 100

char grid[MAX_N][MAX_M + 1];
int hor[MAX_N][MAX_M];
int vert[MAX_N][MAX_M];

void read_input(const char *filename, int &n, int &m)
{
	FILE *f;

	f = fopen(filename, "rt");
	if (f == NULL) {
		fprintf(stderr, "Could not open file: %s\n", filename);
		exit(EXIT_FAILURE);
	}

	if (fscanf(f, "%d %d", &n, &m) != 2)
		exit(EXIT_FAILURE);

	for (int i = 0; i < n; i++)
		if (fscanf(f, "%s", grid[i]) != 1)
			exit(EXIT_FAILURE);

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

	fprintf(f, "%d\n", result);
	fclose(f);
}

int solve_problem(int n, int m)
{
	int result = 0;

	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++) {
			int k;
			for (k = 1; ; k++) {
				int left = j - k;
				int right = j + k;
				if (left < 0 || right >= m)
					break;
				if (grid[i][left] != grid[i][right])
					break;
			}
			--k;
			hor[i][j] = k;
		}

	for (int j = 0; j < m; j++)
		for (int i = 0; i < n; i++) {
			int k;
			for (k = 1; ; k++) {
				int left = i - k;
				int right = i + k;
				if (left < 0 || right >= n)
					break;
				if (grid[left][j] != grid[right][j])
					break;
			}
			--k;
			vert[i][j] = k;
		}

	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			result += hor[i][j] * vert[i][j];

	return result;
}

int main()
{
	int n, m;
	int result;

	read_input("cruce.in", n, m);
	result = solve_problem(n, m);
	write_output("cruce.out", result);

	return 0;
}
