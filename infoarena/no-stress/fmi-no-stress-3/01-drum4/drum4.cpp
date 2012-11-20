#include <algorithm>
#include <limits>
#include <stack>
#include <set>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>

using namespace std;

#define MAX_N 100000
#define MAX_M 100000

vector<int> fw_edges[MAX_N];
vector<int> bw_edges[MAX_N];
set<int> fw_condensed[MAX_N];
set<int> bw_condensed[MAX_N];
stack<int> finish;
int visited[MAX_N];
int scc[MAX_N];
int scc_root[MAX_N];

void read_input(const char *filename, int &n, int &m)
{
	FILE *f;

	f = fopen(filename, "rt");
	if (f == NULL) {
		fprintf(stderr, "Could not open file: %s\n", filename);
		exit(EXIT_FAILURE);
	}

	if (fscanf(f, "%d %d", &n, &m) != 2)
		exit(EXIT_FAILURE);

	for (int i = 0; i < m; i++) {
		int x, y;
		if (fscanf(f, "%d %d", &x, &y) != 2)
			exit(EXIT_FAILURE);
		--x;
		--y;
		fw_edges[x].push_back(y);
		bw_edges[y].push_back(x);
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

void dfs(int node)
{
	visited[node] = 1;
	for (vector<int>::iterator it = fw_edges[node].begin();
			it != fw_edges[node].end(); ++it) {
		int next = *it;
		if (!visited[next])
			dfs(next);
	}
	finish.push(node);
}

void full_dfs(int n)
{
	for (int i = 0; i < n; i++)
		if (!visited[i])
			dfs(i);
}

void reverse_dfs(int root, int node)
{
	visited[node] = 1;
	scc[node] = root;
	for (vector<int>::iterator it = bw_edges[node].begin();
			it != bw_edges[node].end(); ++it) {
		int next = *it;
		if (!visited[next])
			reverse_dfs(root, next);
	}
}

void full_reverse_dfs()
{
	while (!finish.empty()) {
		int node = finish.top();
		if (!visited[node]) {
			reverse_dfs(node, node);
			scc_root[node] = 1;
		}
		finish.pop();
	}
}

int solve_problem(int n, int m)
{
	size_t num_visited_bytes = (size_t) n;
	num_visited_bytes *= sizeof(*visited);
	(void) m;
	int num_sources = 0;
	int num_sinks = 0;
	int num_both = 0;

	full_dfs(n);

	memset(visited, 0, num_visited_bytes);
	full_reverse_dfs();

	for (int i = 0; i < n; i++) {
		const vector<int> &edges = fw_edges[i];
		int x = scc[i];
		for (vector<int>::const_iterator it = edges.begin(); it != edges.end(); ++it) {
			int y = scc[*it];
			if (x != y) {
				fw_condensed[x].insert(y);
				bw_condensed[y].insert(x);
			}
		}
	}

	for (int i = 0; i < n; i++)
		if (scc_root[i]) {
			bool is_source = false;
			bool is_sink = false;
			if (fw_condensed[i].empty())
				is_sink = true;
			if (bw_condensed[i].empty())
				is_source = true;
			if (is_source) {
				if (is_sink)
					num_both++;
				else
					num_sources++;
			} else if (is_sink) {
				num_sinks++;
			}
		}

	//printf("%d %d %d\n", num_sources, num_sinks, num_both);
	if (num_sources == 0 && num_sinks == 0) {
		return (num_both == 1) ? 0 : num_both;
	} else {
		int total_sources = num_sources + num_both;
		int total_sinks = num_sinks + num_both;
		if (total_sources > 1)
			return min(total_sources, total_sinks);
		else
			return total_sinks;
	}
}

int main()
{
	int n, m;
	int result;

	read_input("drum4.in", n, m);
	result = solve_problem(n, m);
	write_output("drum4.out", result);

	return 0;
}
