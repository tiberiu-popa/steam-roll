#include <algorithm>
#include <bitset>
#include <stack>
#include <cstdio>
#include <cstdlib>

using namespace std;

#define MAX_N 1234
#define MAX_M 1234

bitset<MAX_M> grid1[MAX_N];
bitset<MAX_M> grid2[MAX_N];
int left_ones[MAX_N][MAX_M];
int up_ones[MAX_N][MAX_M];
int up_twos[MAX_N][MAX_M];

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
		for (int j = 0; j < m; j++) {
			int elem;
			if (fscanf(f, "%d", &elem) != 1)
				exit(EXIT_FAILURE);
			size_t idx = (size_t) j;
			if (elem == 1)
				grid1[i].set(idx);
			else
				grid2[i].set(idx);
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

void update_result(int n, int m, int left, int right, int bottom, int height, int &result)
{
	int top = bottom + 1 - height;
	if (top > 0 && bottom + 1 < n && left > 0 && right + 1 < m) {
		--top;
		++bottom;
		--left;
		++right;
		if (up_ones[bottom][right] >= bottom - top + 1 &&
				left_ones[bottom][right] >= right - left + 1) {
			if (up_ones[bottom][left] >= bottom - top + 1 &&
					left_ones[top][right] >= right - left + 1) {
				int candidate = (right - left + 1) * (bottom - top + 1);
				result = max(result, candidate);
			}
		}
	}
}

int solve_problem(int n, int m)
{
	stack< pair<int, int> > nodes;
	int result = 0;

	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++) {
			size_t idx = (size_t) j;
			if (grid1[i][idx]) {
				int left_value = (j > 0) ? left_ones[i][j - 1] : 0;
				left_ones[i][j] = left_value + 1;
				int up_value = (i > 0) ? up_ones[i - 1][j] : 0;
				up_ones[i][j] = up_value + 1;
				result = 1;
			} else if (grid2[i][idx]) {
				int up_value = (i > 0) ? up_twos[i - 1][j] : 0;
				up_twos[i][j] = up_value + 1;
			}
		}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			int height = up_twos[i][j];
			if (nodes.empty()) {
				if (height > 0)
					nodes.push(pair<int, int>(j, height));
			} else {
				pair<int, int> peak = nodes.top();
				update_result(n, m, peak.first, j - 1, i, peak.second, result);
				if (height > peak.second) {
					nodes.push(pair<int, int>(j, height));
				} else if (height < peak.second) {
					for (;;) {
						nodes.pop();
						if (nodes.empty())
							break;
						pair<int, int> next = nodes.top();
						if (height > next.second)
							break;
						peak = next;
						update_result(n, m, peak.first, j - 1, i, peak.second, result);
					}
					if (height > 0)
						nodes.push(pair<int, int>(peak.first, height));
				}
			}
		}
		while (!nodes.empty()) {
			pair<int, int> peak = nodes.top();
			nodes.pop();
			update_result(n, m, peak.first, m - 1, i, peak.second, result);
		}
	}

	for (int i = 1; i < n; i++)
		for (int j = 0; j < m; j++) {
			int len = min(left_ones[i][j], left_ones[i - 1][j]);
			result = max(result, 2 * len);
		}

	for (int i = 0; i < n; i++)
		for (int j = 1; j < m; j++) {
			int len = min(up_ones[i][j], up_ones[i][j - 1]);
			result = max(result, 2 * len);
		}

	return result;
}

int main()
{
	int n, m, result;

	read_input("ternar.in", n, m);
	result = solve_problem(n, m);
	write_output("ternar.out", result);

	return 0;
}
