#include <algorithm>
#include <vector>
#include <cstdio>
#include <cstdlib>

#define MAX_N 100000

using namespace std;

void read_input(const char *filename, vector< pair<int, int> > &v)
{
	FILE *f;
	int n;

	f = fopen(filename, "rt");
	if (f == NULL) {
		fprintf(stderr, "Could not open file: %s\n", filename);
		exit(EXIT_FAILURE);
	}

	if (fscanf(f, "%d", &n) != 1)
		exit(EXIT_FAILURE);
	for (int i = 0; i < n; i++) {
		int x;
		if (fscanf(f, "%d", &x) != 1)
			exit(EXIT_FAILURE);
		v.push_back(pair<int, int>(x, i));
	}
	fclose(f);
}

void write_output(const char *filename, long long result)
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

long long solve_problem(vector< pair<int, int> > &v)
{
	vector<bool> visited(v.size());
	long long result = 0;

	sort(v.begin(), v.end());

	int min_value = v.front().first;
	for (size_t i = 0; i < v.size(); i++)
		if (!visited[i]) {
			visited[i] = true;
			int min_cycle_value = v[i].first;
			int len_cycle = 0;
			for (size_t next = v[i].second; next != i; next = v[next].second) {
				visited[next] = true;
				int next_value = v[next].first;
				if (next_value < min_cycle_value)
					min_cycle_value = next_value;
				len_cycle++;
			}
			long long left = min_cycle_value;
			left *= len_cycle;
			long long right = min_value;
			right *= len_cycle + 2;
			result += min(left, right);
		}

	return result;
}

int main()
{
	vector< pair<int, int> > v;
	long long result;

	read_input("intersort.in", v);
	result = solve_problem(v);
	write_output("intersort.out", result);

	return 0;
}
