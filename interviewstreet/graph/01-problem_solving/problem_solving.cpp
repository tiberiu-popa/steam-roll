#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

#define MAX_N 300
#define MAX_M (2 * (MAX_N + 1))

using namespace std;

vector<int> neighbour_lists[MAX_M];
int capacity[MAX_M][MAX_M];
int flow[MAX_M][MAX_M];
int parent[MAX_M];

bool bfs(int n, int source, int sink)
{
	queue<int> node_queue;

	fill(parent, parent + n, -1);
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

void write_output(int result)
{
	cout << result << endl;
}

int edmonds_karp(int n, int source, int sink)
{
	int max_flow;

	for (int i = 0; i < n; i++)
		fill(flow[i], flow[i] + n, 0);

	const vector<int> &sinkers = neighbour_lists[sink];
	max_flow = 0;
	for (;;) {
		bool path_exists = bfs(n, source, sink);
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

void solve_problem()
{
	int v[MAX_N];
	int source, sink;
	int n, k, m;

	cin >> n >> k;
	m = 2 * (n + 1);

	for (int i = 0; i < n; i++)
		cin >> v[i];

	for (int i = 0; i < m; i++) {
		neighbour_lists[i].clear();
		fill(capacity[i], capacity[i] + m, 0);
	}

	for (int i = 0; i < n; i++)
		for (int j = i + 1; j < n; j++)
			if (abs(v[j] - v[i]) >= k) {
				int q = j + n;
				neighbour_lists[i].push_back(q);
				neighbour_lists[q].push_back(i);
				capacity[i][q] = 1;
			}

	source = 2 * n;
	sink = 2 * n + 1;

	for (int i = 0; i < n; i++) {
		neighbour_lists[source].push_back(i);
		neighbour_lists[i].push_back(source);
		capacity[source][i] = 1;
	}

	for (int j = 0; j < n; j++) {
		int q = j + n;
		neighbour_lists[q].push_back(sink);
		neighbour_lists[sink].push_back(q);
		capacity[q][sink] = 1;
	}

	int max_flow = edmonds_karp(m, source, sink);
	write_output(n - max_flow);
}

int main()
{
	int num_tests;

	cin >> num_tests;
	for (int i = 0; i < num_tests; i++)
		solve_problem();

	return 0;
}
