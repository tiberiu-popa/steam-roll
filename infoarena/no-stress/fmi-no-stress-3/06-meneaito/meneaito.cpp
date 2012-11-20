#include <algorithm>
#include <set>
#include <cstdio>
#include <cstdlib>

using namespace std;

#define MAX_N 200000
#define MAX_T 200000

int a[MAX_N];
int b[MAX_N];
bool bad[MAX_T + 1];
set<int> rem[MAX_N + 1];

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

	for (int i = 2; i < n; i++)
		if (fscanf(f, "%d", &a[i]) != 1)
			exit(EXIT_FAILURE);

	for (int i = 2; i < n; i++)
		if (fscanf(f, "%d", &b[i]) != 1)
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

int solve_problem(int n)
{
	for (int i = 2; i < n; i++)
		if (a[i] <= i && b[i] >= i) {
			int dist = b[i] - a[i];
			if (i == a[i] || i == b[i]) {
				int q = i - a[i];
				rem[dist].insert(q);
			} else {
				int q = i - a[i];
				int r = 2 * dist - q;
				rem[dist].insert(q);
				rem[dist].insert(r);
			}
		}

	for (int i = 1; i <= n; i++) {
		for (set<int>::iterator it = rem[i].begin(); it != rem[i].end(); ++it) {
			for (int x = *it; x <= MAX_T; x += 2 * i)
				bad[x] = true;
		}
	}

	int k;
	for (k = 1; k <= MAX_T; k++) {
		if (!bad[k])
			break;
	}
	if (k <= MAX_T)
		return k;
	else
		return -1;
}

int main()
{
	int n;
	int result;

	read_input("meneaito.in", n);
	result = solve_problem(n);
	write_output("meneaito.out", result);

	return 0;
}
