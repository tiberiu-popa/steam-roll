#include <algorithm>
#include <iostream>
#include <vector>
#include <cstdio>
#include <cstdlib>

#define MAX_N 2000
#define MAX_M (MAX_N * (MAX_N - 1) / 2)

using namespace std;

struct edge {

	int x, y;
	int cost;

	bool operator< (const struct edge &rhs) const
	{
		return cost < rhs.cost;
	}

};

struct set_entry {
	int parent, rank;
};

struct edge edges[MAX_M];
struct set_entry v[MAX_N];
int path[MAX_N];

int find(int x)
{
	int num_elems = 0;

	while (1) {
		int parent = v[x].parent;
		if (x == parent)
			break;
		path[num_elems++] = x;
		x = parent;
	}

	for (int i = 0; i < num_elems; i++)
		v[path[i]].parent = x;

	return x;
}

bool set_union(int x, int y)
{
	int p = find(x);
	int q = find(y);

	if (p == q)
		return false;

	set_entry &fst = v[p];
	set_entry &snd = v[q];

	if (fst.rank < snd.rank) {
		fst.parent = q;
	} else {
		snd.parent = p;
		if (fst.rank == snd.rank)
			fst.rank++;
	}

	return true;
}

void solve_problem()
{
	int n, m, k;

	if (scanf("%d %d %d", &n, &m, &k) != 3)
		exit(EXIT_FAILURE);

	if (k < 0)
		k = 0;
	else if (k > n)
		k = n;

	for (int i = 0; i < n; i++) {
		v[i].parent = i;
		v[i].rank = 0;
	}

	for (int i = 0; i < m; i++) {
		int x, y, cost;
		if (scanf("%d %d %d", &x, &y, &cost) != 3)
			exit(EXIT_FAILURE);
		--x;
		--y;
		edges[i].x = x;
		edges[i].y = y;
		edges[i].cost = cost;
	}

	sort(edges, edges + m);

	vector<edge> cool_edges;
	int num_trees = n;
	int total_cost = 0;
	for (int i = 0; i < m; i++) {
		const edge &elem = edges[i];
		if (set_union(elem.x, elem.y)) {
			--num_trees;
			cool_edges.push_back(elem);
			total_cost += elem.cost;
			if (num_trees == 1)
				break;
		}
	}

	if (num_trees > k) {
		cout << "Impossible!" << endl;
		return;
	}

	for (int i = num_trees; i < k; i++) {
		total_cost -= cool_edges.back().cost;
		cool_edges.pop_back();
	}

	cout << total_cost << endl;
}

int main()
{
	int num_tests;

	if (scanf("%d", &num_tests) != 1)
		exit(EXIT_FAILURE);
	for (int i = 0; i < num_tests; i++)
		solve_problem();

	return 0;
}
