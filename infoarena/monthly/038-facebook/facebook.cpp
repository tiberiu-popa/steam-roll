#include <iostream>
#include <map>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>

using namespace std;

void read_input(const char *filename, vector<int> &v, int &n, int &k)
{
	FILE *f;

	f = fopen(filename, "rt");
	if (f == NULL) {
		fprintf(stderr, "Could not open file: %s\n", filename);
		exit(EXIT_FAILURE);
	}

	if (fscanf(f, "%d %d", &n, &k) != 2)
		exit(EXIT_FAILURE);
	for (int i = 0; i < n; i++) {
		int x;
		if (fscanf(f, "%d", &x) != 1)
			exit(EXIT_FAILURE);
		v.push_back(x);
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

int solve_problem(const vector<int> &v, int k)
{
	map<int, int> count;
	size_t i;

	for (i = 0; i < v.size(); i++) {
		int x = v[i];
		if (++count[x] == k)
			break;
	}

	if (i < v.size()) {
		int j = (int) i;
		++j;
		if (j <= k)
			return 0;
		else
			return j - k;
	} else {
		return -1;
	}
}

int main()
{
	vector<int> v;
	int n, k;
	int result;

	read_input("facebook.in", v, n, k);
	result = solve_problem(v, k);
	write_output("facebook.out", result);

	return 0;
}
