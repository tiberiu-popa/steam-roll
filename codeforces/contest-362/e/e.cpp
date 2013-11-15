#include <limits>
#include <vector>
#include <cstdio>
#include <cstring>

#define MAX_NODES 50

using namespace std;

struct edge {

	int out, capacity, flow, cost;
	pair<int, size_t> opposite;

	edge(int out, int capacity, int flow, int cost)
		: out(out), capacity(capacity), flow(flow), cost(cost), opposite(-1, 0) { }

};

vector<edge> neighbours[MAX_NODES];
pair<int, size_t> parent[MAX_NODES];
int queue[MAX_NODES];
int dist[MAX_NODES];
int path_capacity[MAX_NODES];

int bellman_ford(int n, int source, int sink)
{
	for (int u = 0; u < n; u++)
		parent[u] = make_pair(-1, 0);

	for (int i = 0; i < n; i++)
		dist[i] = numeric_limits<int>::max();
	dist[source] = 0;

	path_capacity[source] = numeric_limits<int>::max();
	path_capacity[sink] = 0;

	for (int k = 0; k < n - 1; k++)
		for (int u = 0; u < n; u++)
			if (dist[u] < numeric_limits<int>::max()) {
				size_t i = 0;
				for (vector<edge>::iterator it = neighbours[u].begin(); it != neighbours[u].end(); ++it, ++i) {
					int v = it->out;
					if (it->capacity > it->flow) {
						int relaxed_distance = dist[u] + it->cost;
						if (relaxed_distance < dist[v]) {
							dist[v] = relaxed_distance;
							parent[v] = make_pair(u, i);
							path_capacity[v] = it->capacity - it->flow;
						}
					}
				}
			}

	int v = sink;
	int saturate = path_capacity[v];
	while (v != source) {
		v = parent[v].first;
		if (path_capacity[v] < saturate)
			saturate = path_capacity[v];
	}

	return saturate;
}

int dijkstra(int n, int source, int sink)
{
	int k;
	int pos;

	for (int u = 0; u < n; u++)
		parent[u] = make_pair(-1, 0);

	for (int i = 0; i < n; i++)
		dist[i] = numeric_limits<int>::max();
	dist[source] = 0;

	for (int i = 0; i < n; i++)
		queue[i] = i;
	k = n;

	path_capacity[source] = numeric_limits<int>::max();
	path_capacity[sink] = 0;

	pos = -1;
	while (k > 0) {
		int min_val = numeric_limits<int>::max();
		for (int i = 0; i < k; i++)
			if (dist[queue[i]] < min_val) {
				min_val = dist[queue[i]];
				pos = i;
			}
		if (min_val == numeric_limits<int>::max())
			break;

		int u = queue[pos];
		--k;
		memmove(queue + pos, queue + pos + 1, ((size_t) (k - pos)) * sizeof(*queue));

		size_t i = 0;
		for (vector<edge>::iterator it = neighbours[u].begin(); it != neighbours[u].end(); ++it, ++i) {
			int v = it->out;
			if (it->capacity > it->flow) {
				int relaxed_distance = dist[u] + it->cost;
				if (relaxed_distance < dist[v]) {
					dist[v] = relaxed_distance;
					parent[v] = make_pair(u, i);
					int residual_capacity = it->capacity - it->flow;
					path_capacity[v] = min(path_capacity[u], residual_capacity);
				}
			}
		}
	}

	return path_capacity[sink];
}

void edmonds_karp(int n, int source, int sink, int cost_limit)
{
	int cumulated_cost = 0;
	int max_flow = 0;
	int min_cost = 0;

	int saturate = bellman_ford(n, source, sink);

	while (saturate != 0) {
		for (int i = 0; i < n; i++)
			if (dist[i] < numeric_limits<int>::max()) {
				for (vector<edge>::iterator it = neighbours[i].begin(); it != neighbours[i].end(); ++it) {
					int j = it->out;
					if (dist[j] < numeric_limits<int>::max())
						it->cost += dist[i] - dist[j];
				}
			}

		cumulated_cost += dist[sink];
		if (cumulated_cost > 0) {
			int max_saturate = (cost_limit - min_cost) / cumulated_cost;
			if (max_saturate == 0)
				break;
			if (saturate > max_saturate)
				saturate = max_saturate;
		}
		max_flow += saturate;
		min_cost += saturate * cumulated_cost;

		int v = sink;
		while (v != source) {
			pair<int, size_t> u = parent[v];
			edge &fw = neighbours[u.first][u.second];
			pair<int, size_t> w = fw.opposite;
			edge &bw = neighbours[w.first][w.second];
			fw.flow += saturate;
			bw.flow -= saturate;
			v = u.first;
		}

		saturate = dijkstra(n, source, sink);
	}

	printf("%d\n", max_flow);
}

int solve_problem()
{
	int n, k;

	if (scanf("%d %d", &n, &k) != 2)
		return 1;

	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++) {
			int x;
			if (scanf("%d", &x) != 1)
				return 1;
			if (x > 0) {
				size_t p0 = neighbours[i].size();
				neighbours[i].push_back(edge(j, x, 0, 0));
				size_t p1 = neighbours[i].size();
				neighbours[i].push_back(edge(j, numeric_limits<int>::max(), 0, 1));

				size_t p2 = neighbours[j].size();
				neighbours[j].push_back(edge(i, 0, 0, 0));
				size_t p3 = neighbours[j].size();
				neighbours[j].push_back(edge(i, 0, 0, -1));

				neighbours[i][p0].opposite = make_pair(j, p2);
				neighbours[i][p1].opposite = make_pair(j, p3);
				neighbours[j][p2].opposite = make_pair(i, p0);
				neighbours[j][p3].opposite = make_pair(i, p1);
			}
		}

	edmonds_karp(n, 0, n - 1, k);

	return 0;
}

int main()
{
	solve_problem();
	return 0;
}
