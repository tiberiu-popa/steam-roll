#include <algorithm>
#include <limits>
#include <queue>
#include <vector>
#include <cstdio>

using namespace std;

bool bfs(const vector< vector<int> > &neighbour_lists,
		const vector< vector<int> > &capacity, vector< vector<int> > &flow,
		vector<int> &parent, int source, int sink)
{
	queue<int> node_queue;

	fill(parent.begin(), parent.end(), -1);
	parent[source] = -2;

	node_queue.push(source);

	while (!node_queue.empty()) {
		int u = node_queue.front();
		if (u == sink)
			return true;
		node_queue.pop();

		const vector<int> &n_list = neighbour_lists[u];
		for (vector<int>::const_iterator it = n_list.begin(); it != n_list.end(); ++it) {
			int v = *it;
			if (capacity[u][v] > flow[u][v] && parent[v] == -1) {
				parent[v] = u;
				node_queue.push(v);
			}
		}
	}

	return false;
}

void write_output(const char *filename, int max_flow)
{
	FILE *f;

	f = fopen(filename, "wt");
	if (f == NULL)
		return;

	fprintf(f, "%d\n", max_flow);
	fclose(f);
}

int edmonds_karp(const vector< vector<int> > &neighbour_lists,
		const vector< vector<int> > &capacity,
		int source, int sink) {
	vector< vector<int> > flow;
	vector<int> parent(neighbour_lists.size());
	int max_flow;

	for (size_t i = 0; i < neighbour_lists.size(); i++)
		flow.push_back(vector<int>(neighbour_lists.size()));

	const vector<int> &sinkers = neighbour_lists[sink];
	max_flow = 0;
	for (;;) {
		bool path_exists = bfs(neighbour_lists, capacity, flow,
			parent, source, sink);
		if (!path_exists)
			break;

		for (vector<int>::const_iterator it = sinkers.begin(); it != sinkers.end(); ++it) {
			int node = *it;
			int saturate = capacity[node][sink] - flow[node][sink];
			if (parent[node] == -1 || saturate == 0)
				continue;
			for (int v = node, u = parent[v]; v != source; v = u, u = parent[v])
				saturate = min(saturate, capacity[u][v] - flow[u][v]);
			if (saturate > 0) {
				parent[sink] = node;
				for (int v = sink, u = parent[v]; v != source; v = u, u = parent[v]) {
					flow[u][v] += saturate;
					flow[v][u] -= saturate;
				}
				max_flow += saturate;
			}
		}
	}

	return max_flow;
}

int main() {
	FILE *f;
	int source, sink;
	int n, m;
	int u, v;

	f = fopen("maxflow.in", "rt");
	if (f == NULL) {
		fprintf(stderr, "Fiserul retea.in n-a putut fi deschis.\n");
		exit(-1);
	}

	if (fscanf(f, "%d", &n) != 1)
		exit(-1);
	if (fscanf(f, "%d", &m) != 1)
		exit(-1);

	vector< vector<int> > neighbour_lists(n);
	vector< vector<int> > capacity;
	for (int i = 0; i < n; i++)
		capacity.push_back(vector<int>(neighbour_lists.size()));

	for (int i = 0; i < m; i++) {
		if (fscanf(f, "%d", &u) != 1)
			exit(-1);
		if (fscanf(f, "%d", &v) != 1)
			exit(-1);
		--u;
		--v;
		neighbour_lists[u].push_back(v);
		neighbour_lists[v].push_back(u);
		if (fscanf(f, "%d", &capacity[u][v]) != 1)
			exit(-1);
	}

	fclose(f);

	source = 0;
	sink = n - 1;

	int max_flow = edmonds_karp(neighbour_lists, capacity, source, sink);
	write_output("maxflow.out", max_flow);

	return 0;
}
