#include <algorithm>
#include <queue>
#include <set>
#include <vector>
#include <cstdio>
#include <cstdlib>

using namespace std;

#define MAX_N 100000

struct set_entry {
	int parent, rank;
};

struct set_entry disjoint[MAX_N];
int spath[MAX_N];
set<int> graph[MAX_N];
int counts[MAX_N];
int visited[MAX_N];

int set_find(int x)
{
	int num_elems = 0;

	while (1) {
		int parent = disjoint[x].parent;
		if (x == parent)
			break;
		spath[num_elems++] = x;
		x = parent;
	}

	for (int i = 0; i < num_elems; i++)
		disjoint[spath[i]].parent = x;

	return x;
}

bool set_union(int x, int y)
{
	int p = set_find(x);
	int q = set_find(y);

	if (p == q)
		return false;

	set_entry &fst = disjoint[p];
	set_entry &snd = disjoint[q];

	if (fst.rank < snd.rank) {
		fst.parent = q;
	} else {
		snd.parent = p;
		if (fst.rank == snd.rank)
			fst.rank++;
	}

	return true;
}

void read_input(const char *filename, int &n)
{
	FILE *f;
	vector< pair<int, int> > edges;
	int m;

	f = fopen(filename, "rt");
	if (f == NULL) {
		fprintf(stderr, "Could not open file: %s\n", filename);
		exit(EXIT_FAILURE);
	}

	if (fscanf(f, "%d %d", &n, &m) != 2)
		exit(EXIT_FAILURE);

	for (int i = 0; i < n; i++) {
		disjoint[i].parent = i;
		disjoint[i].rank = 0;
	}

	for (int i = 0; i < m; i++) {
		int t, x, y;
		if (fscanf(f, "%d %d %d", &t, &x, &y) != 3)
			exit(EXIT_FAILURE);
		--x;
		--y;
		if (t == 0)
			set_union(x, y);
		else
			edges.push_back(pair<int, int>(x, y));
	}

	for (int i = 0; i < n; i++) {
		int parent = set_find(i);
		++counts[parent];
	}

	for (vector< pair<int, int> >::iterator it = edges.begin(); it != edges.end(); ++it) {
		int x = set_find(it->first);
		int y = set_find(it->second);
		graph[x].insert(y);
		graph[y].insert(x);
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

int bfs(int node)
{
	queue< pair<int, int> > nodes;
	int sum[2] = { 0, 0 };

	nodes.push(pair<int, int>(node, 0));
	visited[node] = 1;
	while (!nodes.empty()) {
		pair<int, int> elem = nodes.front();
		int current = elem.first;
		sum[elem.second & 1] += counts[current];
		nodes.pop();
		for (set<int>::iterator it = graph[current].begin(); it != graph[current].end(); ++it) {
			int next = *it;
			if (!visited[next]) {
				visited[next] = 1;
				nodes.push(pair<int, int>(next, elem.second + 1));
			}
		}
	}

	return max(sum[0], sum[1]);
}

int solve_problem(int n)
{
	int sum = 0;

	for (int i = 0; i < n; i++)
		if (counts[i] > 0) {
			if (!visited[i])
				sum += bfs(i);
		}

	return sum;
}

int main()
{
	int n, result;

	read_input("petrecere2.in", n);
	result = solve_problem(n);
	write_output("petrecere2.out", result);

	return 0;
}
