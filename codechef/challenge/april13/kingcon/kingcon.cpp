#include <algorithm>
#include <limits>
#include <vector>
#include <cmath>
#include <cstdio>
#include <cstring>

#define MAX_N 3000

using namespace std;

vector<int> neighbours[MAX_N];
int visited[MAX_N];
int parent[MAX_N];
int lowlink[MAX_N];
int indices[MAX_N];
int current_index;
int result;

void dfs(int root)
{
	int num_children = 0;
	int childlink = numeric_limits<int>::min();
	visited[root] = 1;
	indices[root] = current_index++;
	lowlink[root] = indices[root];
	for (vector<int>::iterator it = neighbours[root].begin(); it != neighbours[root].end(); ++it) {
		int node = *it;
		if (node != parent[root]) {
			if (!visited[node]) {
				++num_children;
				parent[node] = root;
				dfs(node);
				if (lowlink[node] < lowlink[root])
					lowlink[root] = lowlink[node];
				if (lowlink[node] > childlink)
					childlink = lowlink[node];
			} else {
				if (indices[node] < lowlink[root])
					lowlink[root] = indices[node];
			}
		}
	}
	if (parent[root] < 0) {
		if (num_children > 1)
			++result;
	} else {
		if (indices[root] <= childlink) {
			++result;
		}
	}
}

void solve_problem()
{
	int n, m, k;

	if (scanf("%d %d %d", &n, &m, &k) != 3)
		return;

	for (int i = 0; i < n; i++)
		neighbours[i].clear();
	for (int i = 0; i < m; i++) {
		int x, y;
		if (scanf("%d %d", &x, &y) != 2)
			return;
		neighbours[x].push_back(y);
		neighbours[y].push_back(x);
	}

	fill(visited, visited + n, 0);
	fill(parent, parent + n, -1);
	current_index = 0;
	result = 0;
	dfs(0);
#if 0
	for (int i = 0; i < n; i++)
		printf("%d ", indices[i]);
	printf("\n");

	for (int i = 0; i < n; i++)
		printf("%d ", lowlink[i]);
	printf("\n");
#endif
	printf("%d\n", result * k);
}

int main()
{
	int num_tests;

	if (scanf("%d", &num_tests) != 1)
		return 1;
	for (int i = 0; i < num_tests; i++)
		solve_problem();

	return 0;
}
