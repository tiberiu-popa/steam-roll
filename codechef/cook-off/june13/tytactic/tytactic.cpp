#include <algorithm>
#include <functional>
#include <vector>
#include <cmath>
#include <cstdio>
#include <cstring>

#define MAX_N 100000

using namespace std;

struct tree_node {

	int value;
	int flip_value;

	tree_node()
	{
		memset(this, 0, sizeof(*this));
	}

	tree_node combine(const tree_node& other) const
	{
		tree_node join_node;
		combine_nodes(join_node, *this, other);
		return join_node;
	}

	friend void combine_nodes(tree_node &parent, const tree_node &left, const tree_node &right);

};

vector<int> neighbours[MAX_N];
int v[MAX_N];
int value[MAX_N];
int level[MAX_N];
int subtree[MAX_N];
int parent[MAX_N];
bool visited[MAX_N];

vector<int> chains[MAX_N];
int chain_index[MAX_N];
size_t chain_position[MAX_N];
int num_chains = 0;

vector<tree_node> tree[MAX_N];

void insert_node(int chain, int node)
{
	chain_position[node] = chains[chain].size();
	chains[chain].push_back(node);
	chain_index[node] = chain;
}

void dfs(int root)
{
	int heavy_node = -1;
	bool leaf = true;

	value[root] = v[root];
	visited[root] = true;
	subtree[root] = 1;

	for (vector<int>::iterator it = neighbours[root].begin(); it != neighbours[root].end(); ++it) {
		int node = *it;
		if (!visited[node]) {
			leaf = false;
			level[node] = level[root] + 1;
			parent[node] = root;
			dfs(node);
			value[root] += value[node];
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

void combine_nodes(tree_node &parent, const tree_node &left, const tree_node &right)
{
	parent.value = max(left.value, right.value);
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
	return get_msb(n - 1);
}

void build_interval_tree(vector<tree_node> &tree, const vector<int> &chains)
{
	const int num_bits = get_upper_exp(chains.size());
	size_t tree_size = pow2(num_bits + 1) - 1;
	tree.resize(tree_size);

	size_t offset = pow2(num_bits) - 1;
	for (size_t i = 0; i < chains.size(); i++)
		tree[offset + i].value = value[chains[i]];

	for (int k = num_bits - 1; k >= 0; k--) {
		offset = pow2(k) - 1;
		for (size_t i = 0; i <= offset; i++) {
			size_t index = offset + i;
			size_t left_index = 2 * index + 1;
			size_t right_index = left_index + 1;
			tree_node &parent = tree[index];
			tree_node &left = tree[left_index];
			tree_node &right = tree[right_index];
			combine_nodes(parent, left, right);
		}
	}
}

void flip_node(tree_node &node)
{
	node.value += node.flip_value;
}

void propagate_flips(vector<tree_node> &tree, tree_node &node,
	int left_index, int right_index, bool has_children)
{
	if (node.flip_value != 0) {
		flip_node(node);
		if (has_children) {
			tree[(size_t) left_index].flip_value += node.flip_value;
			tree[(size_t) right_index].flip_value += node.flip_value;
		}
		node.flip_value = 0;
	}
}

tree_node update_tree(vector<tree_node> &tree, int index, int start, int end,
	int left, int right, int update_value)
{
	tree_node &node = tree[(size_t) index];

	int left_index = (index << 1) + 1;
	int right_index = left_index + 1;

	propagate_flips(tree, node, left_index, right_index, start < end);

	if (right < start || end < left)
		return node;

	if (left <= start && end <= right) {
		node.flip_value += update_value;
		propagate_flips(tree, node, left_index, right_index, start < end);
	} else {
		int mid = start + ((end - start) >> 1);
		tree_node left_node = update_tree(tree, left_index, start, mid,
			left, right, update_value);
		tree_node right_node = update_tree(tree, right_index, mid + 1, end,
			left, right, update_value);
		combine_nodes(node, left_node, right_node);
	}

	return node;
}

tree_node query_tree(vector<tree_node> &tree, int index, int start, int end,
	int left, int right)
{
	tree_node best_node;

	if (right < start || end < left)
		return best_node;

	int left_index = (index << 1) + 1;
	int right_index = left_index + 1;
	tree_node &parent_node = tree[(size_t) index];

	propagate_flips(tree, parent_node, left_index, right_index, start < end);

	if (left <= start && end <= right)
		return parent_node;

	int mid = start + ((end - start) >> 1);
	tree_node left_node = query_tree(tree, left_index, start, mid,
		left, right);
	tree_node right_node = query_tree(tree, right_index, mid + 1, end,
		left, right);
	combine_nodes(best_node, left_node, right_node);
	return best_node;
}

tree_node update_full_tree(vector<tree_node> &tree, int left, int right, int update_value)
{
	return update_tree(tree, 0, 0, (int) (tree.size() / 2), left, right, update_value);
}

tree_node query_full_tree(vector<tree_node> &tree, int left, int right)
{
	return query_tree(tree, 0, 0, (int) (tree.size() / 2), left, right);
}

int query_value(int node)
{
	int node_chain = chain_index[node];
	size_t left = chain_position[node];
	tree_node result = query_full_tree(tree[node_chain], (int) left, (int) left);
	return result.value;
}

int solve_problem()
{
	int n, m;

	if (scanf("%d %d", &n, &m) != 2)
		return 1;

	for (int i = 0; i < n; i++)
		if (scanf("%d", &v[i]) != 1)
			return 1;

	for (int i = 1; i < n; i++) {
		int s, t;
		if (scanf("%d %d", &s, &t) != 2)
			return 1;
		--s;
		--t;
		neighbours[s].push_back(t);
		neighbours[t].push_back(s);
	}

	level[0] = 0;
	parent[0] = -1;
	dfs(0);
#if 0
	for (int i = 0; i < n; i++) {
		printf("(%d):", i);
		for (size_t j = 0; j < neighbours[i].size(); j++)
			printf(" %d", neighbours[i][j]);
		printf("\n");
	}

	printf("value:");
	for (int i = 0; i < n; i++)
		printf(" %d", value[i]);
	printf("\n");

	for (int i = 0; i < num_chains; i++) {
		printf("chain(%d):", i);
		for (size_t j = 0; j < chains[i].size(); j++)
			printf(" %d", chains[i][j]);
		printf("\n");
	}
#endif
	for (int k = 0; k < num_chains; k++) {
		build_interval_tree(tree[k], chains[k]);
	}

	for (int i = 0; i < m; i++) {
		char query_type;
		do {
			if (scanf("%c", &query_type) != 1)
				return 1;
		} while(query_type != 'Q' && query_type != 'U');
		if (query_type == 'Q') {
			int x;
			if (scanf("%d", &x) != 1)
				return 1;
			--x;

			printf("%d\n", query_value(x));
		} else {
			int x, y = 0, new_value;
			if (scanf("%d %d", &x, &new_value) != 2)
				return 1;
			--x;
			int diff_value = new_value - v[x];
			v[x] = new_value;

			while (y >= 0) {
				int x_chain = chain_index[x];
				int y_chain = chain_index[y];
				int px = chains[x_chain].back();
				int py = chains[y_chain].back();

				if (px == py) {
					size_t left = chain_position[x];
					size_t right = chain_position[y];
					if (left > right)
						swap(left, right);
					update_full_tree(tree[y_chain], (int) left, (int) right, diff_value);
					break;
				} else {
					if (level[px] > level[py]) {
						swap(x, y);
						swap(x_chain, y_chain);
						swap(px, py);
					}
					update_full_tree(tree[y_chain], (int) chain_position[y], (int) chains[y_chain].size() - 1, diff_value);
					y = parent[py];
				}
			}
		}
	}

	return 0;
}

int main()
{
	solve_problem();
	return 0;
}
