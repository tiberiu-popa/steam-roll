#include <algorithm>
#include <queue>
#include <set>
#include <vector>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>

#define MAX_N 100000
#define MAX_M 100000

using namespace std;

vector<int> neighbours[MAX_N];
int parent[MAX_N];
int level[MAX_N];
int jumper[MAX_N][18];

int upper_2_exp(int n)
{
	int k = 0;
	while (n > 0) {
		n >>= 1;
		++k;
	}
	return k;
}

void bfs(int n)
{
	queue<int> nodes;
	int source = 0;

	nodes.push(source);
	fill(parent, parent + n, - 1);
	level[source] = 0;
	while (!nodes.empty()) {
		int node = nodes.front();
		nodes.pop();
		for (vector<int>::iterator it = neighbours[node].begin(); it != neighbours[node].end(); ++it) {
			int next_node = *it;
			if (next_node != source && parent[next_node] == -1) {
				parent[next_node] = node;
				level[next_node] = level[node] + 1;
				nodes.push(next_node);
			}
		}
	}
}

int query(int x, int y)
{
	if (level[x] < level[y])
		swap(x, y);

	int num_level_jumps = upper_2_exp(level[x]);
	for (int i = num_level_jumps - 1; i >= 0; i--) {
		int up_level = level[x] - (1 << i);
		if (up_level >= level[y])
			x = jumper[x][i];
	}

	if (x == y)
		return x;

	for (int i = num_level_jumps - 1; i >= 0; i--)
		if (jumper[x][i] != jumper[y][i]) {
			x = jumper[x][i];
			y = jumper[y][i];
		}

	return parent[x];
}

int quad_query(int x, int y, int z, int t)
{
	int first_side = -1, first_level = -1;
	int second_side = -1, second_level = -1;

	int first_lca = query(x, y);
	int second_lca = query(z, t);
	int a = query(x, z);
	int b = query(y, z);
	if (level[a] >= level[b]) {
		if (level[a] >= level[first_lca] && level[a] >= level[second_lca]) {
			first_side = 0;
			first_level = level[a];
		}
	} else {
		if (level[b] >= level[first_lca] && level[b] >= level[second_lca]) {
			first_side = 1;
			first_level = level[b];
		}
	}

	int c = query(x, t);
	int d = query(y, t);
	if (level[c] >= level[d]) {
		if (level[c] >= level[first_lca] && level[c] >= level[second_lca]) {
			second_side = 0;
			second_level = level[c];
		}
	} else {
		if (level[d] >= level[first_lca] && level[d] >= level[second_lca]) {
			second_side = 1;
			second_level = level[d];
		}
	}

	//printf("> (%d %d) (%d %d) (%d %d)\n", first_side, first_level, second_side, second_level, first_lca, second_lca);

	if (first_side >= 0) {
		if (second_side >= 0) {
			if (first_side == second_side)
				return abs(first_level - second_level) + 1;
			else
				return (first_level - level[first_lca]) + (second_level - level[first_lca]) + 1;
		} else {
			return (first_level - level[first_lca]) + 1;
		}
	} else {
		if (second_side >= 0) {
			return (second_level - level[first_lca]) + 1;
		} else {
			return 0;
		}
	}
}

int brute_force(int x, int y, int z, int t)
{
	int first_lca = query(x, y);
	int second_lca = query(z, t);

	set<int> u, v;

	while (x != first_lca) {
		u.insert(x);
		x = parent[x];
	}
	while (y != first_lca) {
		u.insert(y);
		y = parent[y];
	}
	u.insert(first_lca);

	while (z != second_lca) {
		v.insert(z);
		z = parent[z];
	}
	while (t != second_lca) {
		v.insert(t);
		t = parent[t];
	}
	v.insert(second_lca);

	int result = 0;
	for (set<int>::iterator it = u.begin(); it != u.end(); ++it)
		if (v.find(*it) != v.end())
			result++;
	return result;
}

void solve_problem()
{
	int n, m;

	if (scanf("%d %d", &n, &m) != 2)
		return;

	for (int i = 1; i < n; i++) {
		int x, y;
		if (scanf("%d %d", &x, &y) != 2)
			return;
		--x;
		--y;
		neighbours[x].push_back(y);
		neighbours[y].push_back(x);
	}

	bfs(n);

	int num_jumps = upper_2_exp(n - 1);
	for (int i = 0; i < n; i++)
		jumper[i][0] = parent[i];
	for (int j = 1; j < num_jumps; j++)
		for (int i = 0; i < n; i++) {
			int prev_node = jumper[i][j - 1];
			if (prev_node >= 0)
				jumper[i][j] = jumper[prev_node][j - 1];
			else
				jumper[i][j] = prev_node;
		}

#if 0
	srand(time(NULL));
	for (int i = 0; i < (1 << 21); i++) {
		int x, y, z, t;
		x = rand() % n;
		y = rand() % n;
		z = rand() % n;
		t = rand() % n;
		int r1 = quad_query(x, y, z, t);
		int r2 = brute_force(x, y, z, t);
		if (r1 != r2) {
			printf("> %d %d %d %d\n", x, y, z, t);
		}
	}
#endif

	for (int i = 0; i < m; i++) {
		int x, y, z, t;
		if (scanf("%d %d %d %d", &x, &y, &z, &t) != 4)
			return;
		--x;
		--y;
		--z;
		--t;

		int result = quad_query(x, y, z, t);
		printf("%d\n", result);
	}
}

int main()
{
	if (freopen("drumarb.in", "rt", stdin) == NULL)
		return 1;
	if (freopen("drumarb.out", "wt", stdout) == NULL)
		return 1;

	solve_problem();

	return 0;
}
