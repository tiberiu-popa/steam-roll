#include <algorithm>
#include <limits>
#include <set>
#include <vector>
#include <cmath>
#include <cstdio>
#include <cstring>

#define MAX_N 1000
#define MAX_M 2000
#define MAX_P 500

using namespace std;

int u[MAX_M];
int v[MAX_M];
int a[MAX_M];
int b[MAX_M];
int route[MAX_P];

vector<int> graph[MAX_N];
int dist[MAX_N];
bool low[MAX_N];
bool high[MAX_N];
bool special[MAX_M];

void dijkstra(int n, const vector< pair<int, int> > &sources)
{
	set< pair<int, int> > nodes;
	fill(dist, dist + n, numeric_limits<int>::max());
	for (vector< pair<int, int> >::const_iterator it = sources.begin();
			it != sources.end(); ++it) {
		nodes.insert(*it);
		dist[it->second] = it->first;
	}

	while (!nodes.empty()) {
		pair<int, int> entry = *nodes.begin();
		int root = entry.second;
		nodes.erase(nodes.begin());
		for (size_t j = 0; j < graph[root].size(); j++) {
			int edge = graph[root][j];
			int node = v[edge];
			int cand = entry.first;
			if (special[edge]) {
				cand += a[edge];
			} else {
				cand += (low[root] ? a[edge] : b[edge]);
			}
			if (cand < dist[node] || (cand == dist[node] && low[root] && high[node])) {
				nodes.erase(make_pair(dist[node], node));
				nodes.insert(make_pair(cand, node));
				dist[node] = cand;
				low[node] = low[root];
				high[node] = high[root];
			}
		}
	}
}

int solve_problem(int num_case)
{
	int n, m, p;

	if (scanf("%d %d %d", &n, &m, &p) != 3)
		return 1;

	for (int i = 0; i < m; i++) {
		if (scanf("%d %d %d %d", &u[i], &v[i], &a[i], &b[i]) != 4)
			return 1;
		--u[i];
		--v[i];
	}

	for (int i = 0; i < p; i++) {
		if (scanf("%d", &route[i]) != 1)
			return 1;
		--route[i];
	}

	for (int i = 0; i < n; i++)
		graph[i].clear();

	for (int i = 0; i < m; i++)
		graph[u[i]].push_back(i);

	int start = u[route[0]];
	int end = v[route[p - 1]];

	int result = -1;
	int sum = 0;
	for (int i = 0; i < p - 1; i++) {
		int edge = route[i];
		sum += a[edge];
		fill(low, low + n, false);
		fill(high, high + n, false);
		fill(special, special + m, false);
		high[start] = true;
		low[v[edge]] = true;
		for (int j = 0; j <= i; j++)
			special[route[j]] = true;

		vector< pair<int, int> > sources;
		sources.push_back(make_pair(0, start));
		sources.push_back(make_pair(sum, v[edge]));
		dijkstra(n, sources);
		if (high[end]) {
			result = i;
			break;
		}
	}

	if (result >= 0)
		printf("Case #%d: %d\n", num_case, route[result] + 1);
	else
		printf("Case #%d: %s\n", num_case, "Looks Good To Me");

	return 0;
}

int main(int argc, char *argv[])
{
	const char *filenames[] = {
		"lost-yet.in",
		"lost-yet.out",
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
