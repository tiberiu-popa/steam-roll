#include <algorithm>
#include <vector>
#include <cstdio>
#include <cstdlib>

using namespace std;

#define MAX_N 10000

int v_girl, v_boy[2];

void read_input(const char *filename, int &d)
{
	FILE *f;
	int g, h;

	f = fopen(filename, "rt");
	if (f == NULL) {
		fprintf(stderr, "Could not open file: %s\n", filename);
		exit(EXIT_FAILURE);
	}

	if (fscanf(f, "%d", &d) != 1)
		exit(EXIT_FAILURE);

	if (fscanf(f, "%d %d %d", &v_boy[0], &g, &h) != 3)
		exit(EXIT_FAILURE);

	if (fscanf(f, "%d %d %d", &v_boy[1], &g, &h) != 3)
		exit(EXIT_FAILURE);

	if (fscanf(f, "%d", &v_girl) != 1)
		exit(EXIT_FAILURE);

	fclose(f);
}

void write_output(const char *filename, double result)
{
	FILE *f;

	f = fopen(filename, "wt");
	if (f == NULL) {
		fprintf(stderr, "Could not open file: %s\n", filename);
		exit(EXIT_FAILURE);
	}

	fprintf(f, "%.6lf\n", result);
	fclose(f);
}

double solve_problem(int d)
{
	double meeting_time = (double) d / (v_boy[0] + v_boy[1]);
	return meeting_time * v_girl;
}

int main()
{
	int d;
	double result;

	read_input("balulbobocilor.in", d);
	result = solve_problem(d);
	write_output("balulbobocilor.out", result);

	return 0;
}
