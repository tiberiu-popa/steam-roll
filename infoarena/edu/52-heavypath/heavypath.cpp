#include <algorithm>
#include <vector>
#include <cmath>
#include <cstdio>
#include <cstring>

#define MAX_N 100000

using namespace std;

vector<int> neighbours[MAX_N];
int value[MAX_N];
int level[MAX_N];
int subtree[MAX_N];
int parent[MAX_N];
bool visited[MAX_N];

vector<int> chains[MAX_N];
int chain_index[MAX_N];
size_t chain_position[MAX_N];
int num_chains = 0;

vector<int> tree[MAX_N];

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

	visited[root] = true;
	subtree[root] = 1;

	for (vector<int>::iterator it = neighbours[root].begin(); it != neighbours[root].end(); ++it) {
		int node = *it;
		if (!visited[node]) {
			leaf = false;
			level[node] = level[root] + 1;
			parent[node] = root;
			dfs(node);
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
	return get_msb(n - 1);
}

void build_interval_tree(vector<int> &tree, const vector<int> &chains)
{
	const int num_bits = get_upper_exp(chains.size());
	size_t tree_size = pow2(num_bits + 1) - 1;
	tree.resize(tree_size);

	size_t offset = pow2(num_bits) - 1;
	for (size_t i = 0; i < chains.size(); i++)
		tree[offset + i] = value[chains[i]];

	for (int k = num_bits - 1; k >= 0; k--) {
		offset = pow2(k) - 1;
		for (size_t i = 0; i <= offset; i++) {
			size_t index = offset + i;
			size_t left_index = 2 * index + 1;
			size_t right_index = left_index + 1;
			int &parent = tree[index];
			int &left = tree[left_index];
			int &right = tree[right_index];
			parent = max(left, right);
		}
	}
}

int update_tree(vector<int> &tree, int index, int start, int end,
	int left, int right, int elem)
{
	int &node = tree[(size_t) index];

	if (right < start || end < left)
		return node;

	int left_index = (index << 1) + 1;
	int right_index = left_index + 1;

	//propagate_flips(tree, node, left_index, right_index, start < end);

	if (left <= start && end <= right) {
		node = elem;
#if 0
		if (start < end) {
			if (axis) {
				tree[left_index].flip_x ^= 1;
				tree[right_index].flip_x ^= 1;
			} else {
				tree[left_index].flip_y ^= 1;
				tree[right_index].flip_y ^= 1;
			}
		}
#endif
	} else {
		int mid = start + ((end - start) >> 1);
		int left_max = update_tree(tree, left_index, start, mid,
			left, right, elem);
		int right_max = update_tree(tree, right_index, mid + 1, end,
			left, right, elem);
		node = max(left_max, right_max);
	}

	return node;
}

int query_tree(vector<int> &tree, int index, int start, int end,
	int left, int right)
{
	int max_elem = 0;

	if (right < start || end < left)
		return max_elem;

	int left_index = (index << 1) + 1;
	int right_index = left_index + 1;
	max_elem = tree[(size_t) index];

	//propagate_flips(tree, node, left_index, right_index, start < end);

	if (left <= start && end <= right)
		return max_elem;

	int mid = start + ((end - start) >> 1);
	int left_max = query_tree(tree, left_index, start, mid,
		left, right);
	int right_max = query_tree(tree, right_index, mid + 1, end,
		left, right);
	return max(left_max, right_max);
}

int update_full_tree(vector<int> &tree, int left, int right, int elem)
{
	return update_tree(tree, 0, 0, tree.size() / 2, left, right, elem);
}

int query_full_tree(vector<int> &tree, int left, int right)
{
	return query_tree(tree, 0, 0, tree.size() / 2, left, right);
}

int solve_problem()
{
	int n, m;

	if (scanf("%d %d", &n, &m) != 2)
		return 1;

	for (int i = 0; i < n; i++)
		if (scanf("%d", &value[i]) != 1)
			return 1;

	for (int i = 1; i < n; i++) {
		int x, y;
		if (scanf("%d %d", &x, &y) != 2)
			return 1;
		--x;
		--y;
		neighbours[x].push_back(y);
		neighbours[y].push_back(x);
	}

	level[0] = 0;
	parent[0] = -1;
	dfs(0);

	for (int k = 0; k < num_chains; k++) {
		build_interval_tree(tree[k], chains[k]);
	}

	for (int i = 0; i < m; i++) {
		int t, x, y;
		if (scanf("%d %d %d", &t, &x, &y) != 3)
			return 1;
		if (t == 0) {
			--x;
			value[x] = y;
			int x_chain = chain_index[x];
			size_t left = chain_position[x];
			update_full_tree(tree[x_chain], left, left, y);
		} else {
			--x;
			--y;
			int max_elem = 0;
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
					int cand = query_full_tree(tree[y_chain], left, right);
					max_elem = max(max_elem, cand);
					break;
				} else {
					if (level[px] > level[py]) {
						swap(x, y);
						swap(x_chain, y_chain);
						swap(px, py);
					}
					int cand = query_full_tree(tree[y_chain], chain_position[y], chains[y_chain].size() - 1);
					max_elem = max(max_elem, cand);
					y = parent[py];
				}
			}
			printf("%d\n", max_elem);
		}
	}

	return 0;
}

int main()
{
	const char *filenames[] = {
		"heavypath.in",
		"heavypath.out",
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
