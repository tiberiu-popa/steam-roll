#include <algorithm>
#include <vector>
#include <cmath>
#include <cstdio>
#include <cstring>

#define MAX_N 100000

using namespace std;

struct tree_node {

	double elem;

	tree_node() : elem(1.0) { }

};

vector<int> neighbours[MAX_N];
vector<int> bridge_neighbours[MAX_N];
vector< pair<int, double> > bridge_prob_edges[MAX_N];

int level[MAX_N];
int lowlink[MAX_N];
int bridge_reduction[MAX_N];
int num_components = 0;

int subtree[MAX_N];
int parent[MAX_N];
bool visited[MAX_N];

vector<int> chains[MAX_N];
int chain_index[MAX_N];
size_t chain_position[MAX_N];
int num_chains = 0;

vector<tree_node> tree[MAX_N];

void dfs(int root)
{
	bool found_parent = false;
	visited[root] = true;
	lowlink[root] = level[root];

	for (vector<int>::iterator it = neighbours[root].begin(); it != neighbours[root].end(); ++it) {
		int node = *it;
		if (visited[node]) {
			if (parent[root] != node || found_parent) {
				if (level[node] < lowlink[root])
					lowlink[root] = level[node];
			} else {
				found_parent = true;
			}
		} else {
			level[node] = level[root] + 1;
			parent[node] = root;
			dfs(node);
			if (lowlink[node] < lowlink[root])
				lowlink[root] = lowlink[node];
		}
	}
}

void bridge_dfs(int root)
{
	visited[root] = true;

	if (parent[root] >= 0) {
		if (lowlink[root] == level[root]) {
			bridge_reduction[root] = ++num_components;
			int parent_component = bridge_reduction[parent[root]];
			bridge_neighbours[parent_component].push_back(num_components);
		} else {
			bridge_reduction[root] = bridge_reduction[parent[root]];
		}
	} else {
		bridge_reduction[root] = num_components;
	}

	for (vector<int>::iterator it = neighbours[root].begin(); it != neighbours[root].end(); ++it) {
		int node = *it;
		if (!visited[node]) {
			parent[node] = root;
			bridge_dfs(node);
		}
	}
}

void insert_node(int chain, int node)
{
	chain_position[node] = chains[chain].size();
	chains[chain].push_back(node);
	chain_index[node] = chain;
}

void heavy_dfs(int root)
{
	int heavy_node = -1;
	bool leaf = true;

	visited[root] = true;
	subtree[root] = 1;

	for (vector<int>::iterator it = bridge_neighbours[root].begin(); it != bridge_neighbours[root].end(); ++it) {
		int node = *it;
		if (!visited[node]) {
			leaf = false;
			level[node] = level[root] + 1;
			parent[node] = root;
			heavy_dfs(node);
			subtree[root] += subtree[node];
			if (heavy_node == -1 || subtree[node] > subtree[heavy_node]) {
				heavy_node = node;
			}
		}
	}

	if (leaf) {
		insert_node(num_chains, root);
		++num_chains;
	} else {
		int heavy_chain = chain_index[heavy_node];
		insert_node(heavy_chain, root);
	}
}

size_t pow2(int k)
{
	size_t result = 1;
	return result << k;
}

int get_msb(size_t n)
{
	int k = 0;
	for (; n > 0; n >>= 1)
		k++;
	return k;
}

int get_upper_exp(size_t n)
{
	int k = get_msb(n);
	size_t result = pow2(k - 1);
	if (result == n)
		return k - 1;
	else
		return k;
}

void build_interval_tree(vector<tree_node> &tree, size_t num_elems)
{
	const int num_bits = get_upper_exp(num_elems);
	size_t tree_size = pow2(num_bits + 1) - 1;
	tree.clear();
	tree.resize(tree_size);
}

void flip_node(vector<tree_node> &tree, size_t index, double value)
{
	tree[index].elem *= value;
}

void propagate_flips(vector<tree_node> &tree, tree_node &node,
	int left_index, int right_index, bool has_children)
{
	if (node.elem != 1.0) {
		if (has_children) {
			flip_node(tree, (size_t) left_index, node.elem);
			flip_node(tree, (size_t) right_index, node.elem);
			node.elem = 1.0;
		}
	}
}

void update_tree(vector<tree_node> &tree, int index, int start, int end,
	int left, int right, double value)
{
	tree_node &node = tree[(size_t) index];

	if (right < start || end < left)
		return;

	int left_index = (index << 1) + 1;
	int right_index = left_index + 1;

	propagate_flips(tree, node, left_index, right_index, start < end);

	if (left <= start && end <= right) {
		node.elem *= value;
	} else {
		int mid = start + ((end - start) >> 1);
		update_tree(tree, left_index, start, mid, left, right, value);
		update_tree(tree, right_index, mid + 1, end, left, right, value);
	}
}

double query_tree(vector<tree_node> &tree, int index, int start, int end,
	int left, int right)
{
	double product = 1.0;

	if (right < start || end < left)
		return product;

	int left_index = (index << 1) + 1;
	int right_index = left_index + 1;
	tree_node &node = tree[(size_t) index];
	product = node.elem;

	propagate_flips(tree, node, left_index, right_index, start < end);

	if (left <= start && end <= right)
		return product;

	int mid = start + ((end - start) >> 1);
	double left_product = query_tree(tree, left_index, start, mid,
		left, right);
	double right_product = query_tree(tree, right_index, mid + 1, end,
		left, right);
	return left_product * right_product;
}

void update_full_tree(vector<tree_node> &tree, int left, int right, double value)
{
	update_tree(tree, 0, 0, tree.size() / 2, left, right, value);
}

double query_full_tree(vector<tree_node> &tree, int left, int right)
{
	return query_tree(tree, 0, 0, tree.size() / 2, left, right);
}

int solve_problem()
{
	int n, m, e;

	if (scanf("%d %d", &n, &m) != 2)
		return 1;

	for (int i = 0; i < m; i++) {
		int x, y;
		if (scanf("%d %d", &x, &y) != 2)
			return 1;
		--x;
		--y;
		if (x != y) {
			neighbours[x].push_back(y);
			neighbours[y].push_back(x);
		}
	}

	level[0] = 0;
	parent[0] = -1;
	fill(visited, visited + n, false);
	dfs(0);

	parent[0] = -1;
	fill(visited, visited + n, false);
	bridge_dfs(0);
	++num_components;

	level[0] = 0;
	parent[0] = -1;
	fill(visited, visited + num_components, false);
	heavy_dfs(0);

	for (int k = 0; k < num_chains; k++)
		build_interval_tree(tree[k], chains[k].size());

	if (scanf("%d", &e) != 1)
		return 1;

	for (int i = 0; i < e; i++) {
		int x, y;
		double p;
		if (scanf("%d %d %lf", &x, &y, &p) != 3)
			return 1;
		--x;
		--y;
		x = bridge_reduction[x];
		y = bridge_reduction[y];
		if (x != y) {
			bridge_prob_edges[x].push_back(make_pair(y, 1 - p));
			while (y >= 0) {
				int x_chain = chain_index[x];
				int y_chain = chain_index[y];
				int px = chains[x_chain].back();
				int py = chains[y_chain].back();

				if (px == py) {
					size_t left = chain_position[x];
					size_t right = chain_position[y];
					if (left != right) {
						if (left > right)
							swap(left, right);
						update_full_tree(tree[y_chain], left, right - 1, 1 - p);
					}
					break;
				} else {
					if (level[px] > level[py]) {
						swap(x, y);
						swap(x_chain, y_chain);
						swap(px, py);
					}
					update_full_tree(tree[y_chain], chain_position[y], chains[y_chain].size() - 1, 1 - p);
					y = parent[py];
				}
			}
		}
	}

	double result = 0.0;
	for (int i = 1; i < num_components; i++) {
		int chain = chain_index[i];
		size_t pos = chain_position[i];
		result += query_full_tree(tree[chain], pos, pos);
	}
	printf("%lf\n", result);

	return 0;
}

int main()
{
	const char *filenames[] = {
		"critice2.in",
		"critice2.out",
	};

	if (freopen(filenames[0], "rt", stdin) == NULL) {
		fprintf(stderr, "Could not reopen stdin\n");
		return 1;
	}
	if (freopen(filenames[1], "wt", stdout) == NULL) {
		fprintf(stderr, "Could not reopen stdout\n");
		return 1;
	}

	solve_problem();

	return 0;
}
