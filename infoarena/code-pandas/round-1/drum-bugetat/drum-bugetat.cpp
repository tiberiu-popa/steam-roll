#include <algorithm>
#include <map>
#include <set>
#include <vector>
#include <cstdio>
#include <cstring>

#define MAX_N 1000
#define MAX_M 10000
#define MAX_B 1000

using namespace std;

int city[MAX_N];
vector< pair<int, int> > neighbours[MAX_N];
int cost[MAX_N][MAX_B + 1];

void solve_problem()
{
	int n, m, b;
	int s, t;

	if (scanf("%d %d %d", &n, &m, &b) != 3)
		return;
	if (scanf("%d %d", &s, &t) != 2)
		return;
	--s;
	--t;

	for (int i = 0; i < n; i++)
		if (scanf("%d", &city[i]) != 1)
			return;

	for (int i = 0; i < m; i++) {
		int x, y, c;
		if (scanf("%d %d %d", &x, &y, &c) != 3)
			return;
		--x;
		--y;
		neighbours[x].push_back(pair<int, int>(y, c));
		neighbours[y].push_back(pair<int, int>(x, c));
	}

	for (int i = 0; i < n; i++)
		fill(cost[i], cost[i] + b + 1, 2 * n * MAX_B + 100);
	for (vector< pair<int, int> >::iterator it = neighbours[s].begin(); it != neighbours[s].end(); ++it) {
		for (int j = city[it->first]; j <= b; j++)
			cost[it->first][j] = it->second;
	}

	for (int j = 0; j <= b; j++) {
		for (int i = 0; i < n; i++) {
			if (j >= city[i]) {
				for (vector< pair<int, int> >::iterator it = neighbours[i].begin(); it != neighbours[i].end(); ++it) {
					int neigh = it->first;
					int weight = it->second;
					int cand = cost[neigh][j - city[i]] + weight;
					if (cand < cost[i][j])
						cost[i][j] = cand;
				}
			}
		}
	}

	int index = -1;
	int min_cost = 2 * n * MAX_B + 100;
	for (int j = 0; j <= b; j++)
		if (cost[t][j] < min_cost) {
			min_cost = cost[t][j];
			index = j;
		}
#if 0
	for (int i = 0; i < n; i++) {
		for (int j = 0; j <= b; j++)
			printf("%d ", cost[i][j]);
		printf("\n");
	}
	printf("\n");
#endif
	if (s != t) {
		if (index >= 0)
			printf("%d %d\n", min_cost, index);
		else
			printf("-1\n");
	} else {
		printf("0 0\n");
	}
}

int main()
{
	if (freopen("drum-bugetat.in", "rt", stdin) == NULL)
		return 1;
	if (freopen("drum-bugetat.out", "wt", stdout) == NULL)
		return 1;

	solve_problem();

	return 0;
}
