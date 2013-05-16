#include <algorithm>
#include <functional>
#include <limits>
#include <queue>
#include <vector>
#include <cmath>
#include <cstdio>
#include <cstring>

#define MAX_N 1000

using namespace std;

vector<int> edges[MAX_N];
bool visited[MAX_N];

bool bfs(int source)
{
	queue<int> nodes;
	visited[source] = true;
	nodes.push(source);
	while (!nodes.empty()) {
		int node = nodes.front();
		nodes.pop();
		for (vector<int>::iterator it = edges[node].begin(); it != edges[node].end(); ++it) {
			int next_node = *it;
			if (!visited[next_node]) {
				visited[next_node] = true;
				nodes.push(next_node);
			} else {
				return true;
			}
		}
	}
	return false;
}

int solve_problem(int num_case)
{
	int n;

	if (scanf("%d", &n) != 1)
		return 1;

	for (int i = 0; i < n; i++)
		edges[i].clear();

	for (int i = 0; i < n; i++) {
		int m;
		if (scanf("%d", &m) != 1)
			return 1;
		for (int j = 0; j < m; j++) {
			int node;
			if (scanf("%d", &node) != 1)
				return 1;
			--node;
			edges[i].push_back(node);
		}
	}

	bool result = false;
	for (int i = 0; i < n; i++) {
		fill(visited, visited + n, false);
		result = bfs(i);
		if (result)
			break;
	}

	printf("Case #%d: %s\n", num_case, result ? "Yes" : "No");

	return 0;
}

int main(int argc, char *argv[])
{
	const char *filenames[] = {
		"diamond-inheritance.in",
		"diamond-inheritance.out",
	};
	int num_tests;

	for (int i = 1; i < argc && i <= 2; i++)
		filenames[i - 1] = argv[i];

	if (freopen(filenames[0], "rt", stdin) == NULL) {
		fprintf(stderr, "Could not reopen stdin\n");
		return 1;
	}
	if (freopen(filenames[1], "wt", stdout) == NULL) {
		fprintf(stderr, "Could not reopen stdout\n");
		return 1;
	}

	if (scanf("%d", &num_tests) != 1)
		return 1;
	for (int i = 0; i < num_tests; i++)
		solve_problem(i + 1);

	return 0;
}
