#include <algorithm>
#include <map>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#define MAX_N 100000

using namespace std;

vector<int> edges[MAX_N];
int cost[MAX_N];
int parent[MAX_N];
bool visited[MAX_N];
bool inner[MAX_N];
int root = -1;
int cycle = -1;
int sums[MAX_N][2];

void read_input(const char *filename, int &n)
{
	FILE *f;

	f = fopen(filename, "rt");
	if (f == NULL) {
		fprintf(stderr, "Could not open file: %s\n", filename);
		exit(EXIT_FAILURE);
	}

	if (fscanf(f, "%d", &n) != 1)
		exit(EXIT_FAILURE);
	for (int i = 0; i < n; i++) {
		if (fscanf(f, "%d", &cost[i]) != 1)
			exit(EXIT_FAILURE);
	}

	for (int i = 0; i < n; i++) {
		int x, y;
		if (fscanf(f, "%d %d", &x, &y) != 2)
			exit(EXIT_FAILURE);
		--x;
		--y;
		edges[x].push_back(y);
		edges[y].push_back(x);
	}

	fclose(f);
}

void write_output(const char *filename, int result)
{
	FILE *f;

	f = fopen(filename, "wt");
	if (f == NULL) {
		fprintf(stderr, "Could not open file: %s\n", filename);
		exit(EXIT_FAILURE);
	}

	fprintf(f, "%d\n", result);
	fclose(f);
}

void trim_unique(vector<int> &v)
{
	vector<int>::iterator it = unique(v.begin(), v.end());
	vector<int>::difference_type diff = it - v.begin();
	v.resize((vector<int>::size_type) diff);
}

void dfs(int node)
{
	int prev = parent[node];
	visited[node] = 1;
	for (vector<int>::iterator it = edges[node].begin(); it != edges[node].end(); ++it) {
		int next = *it;
		if (visited[next]) {
			if (next != prev && root < 0) {
				root = next;
				cycle = node;
			}
		} else {
			parent[next] = node;
			dfs(next);
		}
	}
}

void compute_tree(int n)
{
	for (int i = 0; i < n; i++)
		edges[i].clear();
	for (int i = 0; i < n; i++) {
		int prev = parent[i];
		if (prev >= 0)
			edges[prev].push_back(i);
	}
}

void compute_inner(int node)
{
	while (node >= 0) {
		inner[node] = true;
		node = parent[node];
	}
}

void tree_dfs(int node)
{
	int u = 0;
	int v = cost[node];
	for (vector<int>::iterator it = edges[node].begin(); it != edges[node].end(); ++it) {
		int next = *it;
		tree_dfs(next);
		if (!inner[next]) {
			u += sums[next][1];
			v += sums[next][0];
		}
	}
	if (v < u)
		u = v;
	sums[node][0] = u;
	sums[node][1] = v;
}

int tree_cycle(int root)
{
	vector<int> nodes;
	int prev_pair[2] = { -1, -1 }, crt_pair[2] = { -1, -1 };
	int node = root;

	while (node >= 0) {
		nodes.push_back(node);
		node = parent[node];
	}

	bool is_first = true;
	for (size_t i = 0; i < nodes.size(); i++) {
		node = nodes[i];
		if (is_first) {
			crt_pair[0] = sums[node][1];
			crt_pair[1] = sums[node][1];
			is_first = false;
		} else {
			memcpy(prev_pair, crt_pair, sizeof(prev_pair));
			crt_pair[0] = prev_pair[1] + sums[node][0];
			crt_pair[1] = prev_pair[0] + sums[node][1];
			if (crt_pair[1] < crt_pair[0])
				crt_pair[0] = crt_pair[1];
		}
	}
	int forward = min(crt_pair[0], crt_pair[1]);

	is_first = true;
	for (size_t i = nodes.size(); i > 0;) {
		--i;
		node = nodes[i];
		if (is_first) {
			crt_pair[0] = sums[node][1];
			crt_pair[1] = sums[node][1];
			is_first = false;
		} else {
			memcpy(prev_pair, crt_pair, sizeof(prev_pair));
			crt_pair[0] = prev_pair[1] + sums[node][0];
			crt_pair[1] = prev_pair[0] + sums[node][1];
			if (crt_pair[1] < crt_pair[0])
				crt_pair[0] = crt_pair[1];
		}
	}
	int backward = min(crt_pair[0], crt_pair[1]);

	return min(forward, backward);
}

int solve_problem(int n)
{
	int result = 0;

	fill(parent, parent + n, -1);
	fill(visited, visited + n, false);
	dfs(0);

	fill(parent, parent + n, -1);
	fill(visited, visited + n, false);
	dfs(root);

	compute_tree(n);
	compute_inner(cycle);
	tree_dfs(root);
	result = tree_cycle(cycle);

	return result;
}

int main()
{
	int n;
	int result;

	read_input("mvc.in", n);
	result = solve_problem(n);
	write_output("mvc.out", result);

	return 0;
}
