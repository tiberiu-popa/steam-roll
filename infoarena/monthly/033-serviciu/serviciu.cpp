#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>

#define MAX_N 100000

using namespace std;

int x[MAX_N];
int y[MAX_N];

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
		int left, right;
		if (fscanf(f, "%d %d", &left, &right) != 2)
			exit(EXIT_FAILURE);
		x[i] = left;
		y[i] = right;
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

	fprintf(f, "%d\n", result);
	fclose(f);
}

int solve_problem(int n)
{
	int result = 0;

	for (int i = 0; i < n; i++) {
		int cand = abs(x[i] - y[i]);
		if (cand > n)
			cand = 2 * n - cand;
		result = max(result, cand);
	}

	return result;
}

int main()
{
	int n;
	int result;

	read_input("serviciu.in", n);
	result = solve_problem(n);
	write_output("serviciu.out", result);

	return 0;
}
