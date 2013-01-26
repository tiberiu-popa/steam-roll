#include <cstdio>
#include <cstdlib>

using namespace std;

void read_input(const char *filename, int &n, int &k)
{
	FILE *f;

	f = fopen(filename, "rt");
	if (f == NULL) {
		fprintf(stderr, "Could not open file: %s\n", filename);
		exit(EXIT_FAILURE);
	}

	if (fscanf(f, "%d %d", &n, &k) != 2)
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

int solve_problem(int n, int k)
{
	int result;

	if (k % 2 == 0) {
		int r = n % k;
		if (r > k / 2 - 1)
			r = k / 2 - 1;
		result = (n / k) * (k / 2 - 1) + r;
		if (n >= k)
			result++;
		if (n >= k / 2)
			result++;
	} else {
		int r = n % k;
		if (r > k / 2)
			r = k / 2;
		result = (n / k) * (k / 2) + r;
		if (n >= k)
			result++;
	}

	return result;
}

int main()
{
	int n, k;
	int result;

	read_input("subset2.in", n, k);
	result = solve_problem(n, k);
	write_output("subset2.out", result);

	return 0;
}
