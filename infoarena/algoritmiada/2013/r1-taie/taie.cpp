#include <algorithm>
#include <vector>
#include <cstdio>
#include <cstdlib>

#define MAX_N 1000
#define EPSILON 1e-5

using namespace std;

double points[2 * MAX_N][2];

void read_input(const char *filename, int &n)
{
	FILE *f;

	f = fopen(filename, "rt");
	if (f == NULL) {
		fprintf(stderr, "Could not open file: %s\n", filename);
		exit(EXIT_FAILURE);
	}

	if (fscanf(f, "%d", &n) != 1)
		exit(EXIT_FAILURE);
	for (int i = 0; i < n; i++) {
		if (fscanf(f, "%lf %lf %lf %lf", &points[2 * i][0], &points[2 * i][1],
				&points[2 * i + 1][0], &points[2 * i + 1][1]) != 4) {
			exit(EXIT_FAILURE);
		}
	}

	fclose(f);
}

void write_output(const char *filename, int u, int v)
{
	FILE *f;

	f = fopen(filename, "wt");
	if (f == NULL) {
		fprintf(stderr, "Could not open file: %s\n", filename);
		exit(EXIT_FAILURE);
	}

	if (u < 0 || v < 0) {
		fprintf(f, "-1\n");
	} else {
		fprintf(f, "%lf %lf %lf %lf\n", points[u][0], points[u][1], points[v][0], points[v][1]);
	}
	fclose(f);
}

pair<int, int> solve_problem(int n)
{
	pair<int, int> result(-1, -1);

	int k = 0;
	for (int i = 0; i < 2 * n; i++)
		for (int j = i + 1; j < 2 * n; j++) {
			bool valid = true;
			for (int iter = 0; iter < n; iter++, k = (k + 1) % n) {
				if (i == 2 * k || i == 2 * k + 1 || j == 2 * k || j == 2 * k + 1)
					continue;
				double coef[2][3] = {
					{
						points[2 * k + 1][0] - points[2 * k][0],
						points[i][0] - points[j][0],
						points[i][0] - points[2 * k][0],
					},
					{
						points[2 * k + 1][1] - points[2 * k][1],
						points[i][1] - points[j][1],
						points[i][1] - points[2 * k][1],
					},
				};
				double det1 = coef[0][0] * coef[1][1] - coef[1][0] * coef[0][1];
				double det2 = coef[0][2] * coef[1][1] - coef[1][2] * coef[0][1];
				double t = det2 / det1;
				if (t < EPSILON || t > 1.0f + EPSILON) {
					valid = false;
					break;
				}
			}
			if (valid) {
				result.first = i;
				result.second = j;
			}
		}

	return result;
}

int main()
{
	int n;
	pair<int, int> result;

	read_input("taie.in", n);
	result = solve_problem(n);
	write_output("taie.out", result.first, result.second);

	return 0;
}
