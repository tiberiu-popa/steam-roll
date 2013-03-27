#include <algorithm>
#include <set>
#include <cstdio>

#define MAX_N 700
#define MAX_M 700

using namespace std;

int grid[MAX_N][MAX_M];
int right[MAX_N][MAX_M];
int origin[MAX_M][MAX_N];

void solve_problem()
{
	char str[MAX_M + 1];
	int n, m;

	scanf("%d %d", &n, &m);
	for (int i = 0; i < n; i++) {
		scanf("%s", str);
		for (int j = 0; j < m; j++)
			grid[i][j] = (str[j] != '0');
	}

	for (int i = 0; i < n; i++)
		right[i][0] = (grid[i][0] != 0);
	for (int j = 1; j < m; j++)
		for (int i = 0; i < n; i++) {
			if (grid[i][j] == 0)
				right[i][j] = 0;
			else
				right[i][j] = 1 + right[i][j - 1];
		}

	int max_area = 0;

	for (int i = 0; i < m; i++)
		fill(origin[i], origin[i] + n, -1);

	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			if (grid[i][j] != 0) {
				int k = 0;
				for (k = 0; k < right[i][j]; k++) {
					int area;
					if (origin[j][j - k] < 0) {
						origin[j][j - k] = i;
						area = k + 1;
					} else {
						area = (k + 1) * (i - origin[j][j - k] + 1);
					}
					if (area > max_area)
						max_area = area;
				}
				for (; k <= j; k++)
					if (grid[i][j - k] == 0)
						origin[j][j - k] = -1;
			} else {
				fill(origin[j], origin[j] + j + 1, -1);
			}

	printf("%d\n", max_area);
}

int main()
{
	if (freopen("rama.in", "rt", stdin) == NULL)
		return 1;
	if (freopen("rama.out", "wt", stdout) == NULL)
		return 1;
	solve_problem();
	return 0;
}
