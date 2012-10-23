#include <algorithm>
#include <limits>
#include <queue>
#include <vector>
#include <cstdio>

using namespace std;

int BFS(const vector< vector<int> > &neighbour_lists,
		const vector< vector<int> > &capacity, vector< vector<int> > &flow,
		vector<int> &parent, int source, int sink)
{
	vector<int> path_capacity(neighbour_lists.size());
	queue<int> node_queue;
	unsigned long source_idx = (unsigned long) source;
	int residual_capacity;

	fill(parent.begin(), parent.end(), -1);
	parent[source_idx] = -2;

	path_capacity[source_idx] = numeric_limits<int>::max();
	node_queue.push(source);

	while (!node_queue.empty()) {
		int u = node_queue.front();
		unsigned long u_idx = (unsigned long) u;
		node_queue.pop();

		const vector<int> &n_list = neighbour_lists[u_idx];
		for (vector<int>::const_iterator it = n_list.begin(); it != n_list.end(); ++it) {
			int v = *it;
			unsigned long v_idx = (unsigned long) v;

			if (capacity[u_idx][v_idx] > flow[u_idx][v_idx] && parent[v_idx] == -1) {
				parent[v_idx] = u;
				residual_capacity = capacity[u_idx][v_idx] - flow[u_idx][v_idx];
				path_capacity[v_idx] = min(path_capacity[u_idx], residual_capacity);
				if (v == sink) {
					return path_capacity[v_idx];
				} else {
					node_queue.push(v);
				}
			}
		}
	}
	
	return 0;
}

void write_output(const char *filename, int max_flow)
{
	FILE *f;

	f = fopen("maxflow.out", "wt");
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
	int max_flow, saturate;

	for (size_t i = 0; i < neighbour_lists.size(); i++)
		flow.push_back(vector<int>(neighbour_lists.size()));

	max_flow = 0;
	for (;;) {
		saturate = BFS(neighbour_lists, capacity, flow, parent, source, sink);
		if (saturate == 0)
			break;

		max_flow += saturate;
		int v = sink;
		unsigned long v_idx = (unsigned long) v;
		while (v != source) {
			int u = parent[v_idx];
			unsigned long u_idx = (unsigned long) u;
			flow[u_idx][v_idx] += saturate;
			flow[v_idx][u_idx] -= saturate;
			v = u;
			v_idx = u_idx;
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

	// Ies de tot
	return 0;
}

