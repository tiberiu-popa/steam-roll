#include <algorithm>
#include <complex>
#include <limits>
#include <map>
#include <queue>
#include <vector>
#include <cmath>
#include <cstdio>

#define MAX_N 1000
#define MAX_NODES (2 * MAX_N + 2)

using namespace std;

vector<int> neighbour_lists[MAX_NODES];
int capacity[MAX_NODES][MAX_NODES];
int flow[MAX_NODES][MAX_NODES];
int parent[MAX_NODES];
int x[MAX_N];
int y[MAX_N];

int skip_whitespace()
{
	int ch;
	while (true) {
		ch = getchar_unlocked();
		if (ch != ' ' && ch != '\n')
			break;
	}
	return ch;
}

template<typename T>
T read_unsigned_integer()
{
	T result = (T) 0;
	int ch = skip_whitespace();
	while (ch >= '0' && ch <= '9') {
		result = 10 * result + (ch - '0');
		ch = getchar_unlocked();
	}
	return result;
}

template<typename T>
T read_signed_integer()
{
	T result = (T) 0;
	bool flip = false;
	int ch = skip_whitespace();
	if (ch == '-') {
		flip = true;
		ch = skip_whitespace();
	}
	while (ch >= '0' && ch <= '9') {
		result = 10 * result + (ch - '0');
		ch = getchar_unlocked();
	}
	return flip ? -result : result;
}

template<class T>
void trim_unique(vector<T> &v)
{
	typename vector<T>::iterator it = unique(v.begin(), v.end());
	typename vector<T>::difference_type diff = it - v.begin();
	v.resize((typename vector<T>::size_type) diff);
}

bool bfs(size_t num_nodes, int source, int sink)
{
	queue<int> node_queue;

	fill(parent, parent + num_nodes, -1);
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

int edmonds_karp(size_t num_nodes, int source, int sink)
{
	int max_flow;

	for (size_t i = 0; i < num_nodes; i++)
		fill(flow[i], flow[i] + num_nodes, 0);

	const vector<int> &sinkers = neighbour_lists[sink];
	max_flow = 0;
	for (;;) {
		bool path_exists = bfs(num_nodes, source, sink);
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

int solve_problem()
{
	vector<int> vx, vy;
	map<int, size_t> rx, ry;
	int n;

	n = read_unsigned_integer<int>();

	for (int i = 0; i < n; i++) {
		x[i] = read_unsigned_integer<int>();
		y[i] = read_unsigned_integer<int>();
		vx.push_back(x[i]);
		vy.push_back(y[i]);
	}

	sort(vx.begin(), vx.end());
	trim_unique(vx);
	sort(vy.begin(), vy.end());
	trim_unique(vy);

	for (size_t i = 0; i < vx.size(); i++)
		rx[vx[i]] = i;
	for (size_t i = 0; i < vy.size(); i++)
		ry[vy[i]] = i;

	size_t num_nodes = vx.size() + vy.size() + 2;
	int source = (int) (num_nodes - 1);
	int sink = (int) (num_nodes - 2);

	for (size_t i = 0; i < num_nodes; i++)
		neighbour_lists[i].clear();

	for (size_t i = 0; i < num_nodes; i++)
		fill(capacity[i], capacity[i] + num_nodes, 0);

	for (size_t i = 0; i < vx.size(); i++) {
		neighbour_lists[source].push_back(i);
		neighbour_lists[i].push_back(source);
		capacity[source][i] = 1;
	}
	for (size_t i = 0; i < vy.size(); i++) {
		neighbour_lists[vx.size() + i].push_back(sink);
		neighbour_lists[sink].push_back(vx.size() + i);
		capacity[vx.size() + i][sink] = 1;
	}
	for (int i = 0; i < n; i++) {
		size_t r = rx[x[i]];
		size_t c = ry[y[i]];
		neighbour_lists[r].push_back(vx.size() + c);
		neighbour_lists[vx.size() + c].push_back(r);
		capacity[r][vx.size() + c] = 1;
	}

	int max_flow = edmonds_karp(num_nodes, source, sink);
	printf("%d\n", max_flow);

	return 0;
}

int main()
{
	int num_tests;

	num_tests = read_unsigned_integer<int>();
	for (int i = 0; i < num_tests; i++)
		solve_problem();

	return 0;
}
