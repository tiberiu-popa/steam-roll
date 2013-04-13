#include <algorithm>
#include <vector>
#include <cmath>
#include <cstdio>
#include <cstring>

#define MAX_N 100
#define MAX_M 100
#define MAX_HEIGHT 100

using namespace std;

int grid[MAX_N][MAX_M];

int solve_problem(int num_case)
{
	int n, m;
	vector< pair<int, int> > reverse_index[MAX_HEIGHT];
	int row[MAX_N];
	int col[MAX_N];

	if (scanf("%d %d", &n, &m) != 2)
		return 1;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			if (scanf("%d", &grid[i][j]) != 1)
				return 2;

	fill(row, row + n, 0);
	fill(col, col + m, 0);

	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++) {
			int x = grid[i][j];
			reverse_index[x - 1].push_back(pair<int, int>(i, j));
		}

	bool answer = true;
	for (int i = MAX_HEIGHT - 1; i >= 0; i--) {
		for (vector< pair<int, int> >::const_iterator it = reverse_index[i].begin(); it != reverse_index[i].end(); ++it) {
			if (row[it->first] && col[it->second]) {
				answer = false;
				break;
			}
		}
		if (!answer)
			break;
		for (vector< pair<int, int> >::const_iterator it = reverse_index[i].begin(); it != reverse_index[i].end(); ++it) {
			row[it->first] = 1;
			col[it->second] = 1;
		}
	}

	printf("Case #%d: %s\n", num_case, answer ? "YES" : "NO");

	return 0;
}

int main()
{
	int num_tests;

	if (freopen("lawnmower.in", "rt", stdin) == NULL) {
		fprintf(stderr, "Could not reopen stdin\n");
		return 1;
	}
	if (freopen("lawnmower.out", "wt", stdout) == NULL) {
		fprintf(stderr, "Could not reopen stdin\n");
		return 1;
	}

	if (scanf("%d", &num_tests) != 1)
		return 1;
	for (int i = 0; i < num_tests; i++)
		solve_problem(i + 1);

	return 0;
}
