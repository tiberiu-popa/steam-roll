#include <algorithm>
#include <limits>
#include <vector>
#include <cmath>
#include <cstdio>
#include <cstring>

#define MAX_N 6666
#define MAX_D 5
#define MOD_NUMBER 747474747

using namespace std;

struct edge {

	long long weight;
	int left, right;

	edge() : weight(0LL), left(-1), right(-1) { }

	edge(long long weight, int left, int right) :
		weight(weight), left(left), right(right) { }

	bool operator<(const edge &other) const
	{
		return weight > other.weight;
	}

};

int coord[MAX_N][MAX_D];
edge edges[MAX_N * (MAX_N - 1) / 2];
int parent[MAX_N];
int rank[MAX_N];
int path[MAX_N];

int find_set(int n)
{
	int num_paths = 0;
	while (parent[n] != n) {
		path[num_paths++] = n;
		n = parent[n];
	}
	for (int i = 0; i < num_paths; i++)
		parent[path[i]] = n;
	return n;
}

int ds_union(int x, int y)
{
	int u = find_set(x);
	int v = find_set(y);

	if (u == v)
		return 0;

	if (rank[u] < rank[v]) {
		parent[u] = parent[v];
	} else if (rank[u] > rank[v]) {
		parent[v] = parent[u];
	} else {
		parent[v] = parent[u];
		rank[u]++;
	}

	return 1;
}

void solve_problem()
{
	int n, d;

	if (scanf("%d %d", &n, &d) != 2)
		return;

	for (int i = 0; i < n; i++) {
		parent[i] = i;
		rank[i] = 0;
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < d; j++)
			if (scanf("%d", &coord[i][j]) != 1)
				return;
	}

	int m = 0;
	for (int i = 0; i < n; i++)
		for (int j = i + 1; j < n; j++) {
			long long w = 0;
			for (int k = 0; k < d; k++) {
				long long diff = coord[i][k] - coord[j][k];
				w += diff * diff;
			}
			edges[m++] = edge(w, i, j);
		}

	long long result = 1;
	sort(edges, edges + m);
	for (int i = 0; i < m; i++) {
		int r = ds_union(edges[i].left, edges[i].right);
		if (r) {
			//printf("> %d %d :: %lld\n", edges[i].left, edges[i].right, edges[i].weight);
			if (edges[i].weight == 0)
				break;
			result *= edges[i].weight % MOD_NUMBER;
			result %= MOD_NUMBER;
		}
	}
	printf("%lld\n", result);
}

int main()
{
	int num_tests;

	if (scanf("%d", &num_tests) != 1)
		return 1;
	for (int i = 0; i < num_tests; i++)
		solve_problem();

	return 0;
}
