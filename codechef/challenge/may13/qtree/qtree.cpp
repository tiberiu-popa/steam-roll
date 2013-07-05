#include <algorithm>
#include <map>
#include <vector>
#include <cmath>
#include <cstdio>
#include <cstring>

#define MAX_N 100000
#define BUFFER_SIZE 1

using namespace std;

struct tree_node {

	int whole_sum[2];
	int best_sum[2];

	int left_sum[2];
	int right_sum[2];

	int size;

	bool flip;

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

map< pair<int, int>, int > edge_value;

vector<int> neighbours[MAX_N];
int value[MAX_N];
int cycle_value[MAX_N];
int level[MAX_N];
int subtree[MAX_N];
int parent[MAX_N];
bool visited[MAX_N];

vector<int> cycle;
size_t cycle_position[MAX_N];
bool in_cycle[MAX_N];
int cycle_start = -1, cycle_end = -1;

vector<int> chains[MAX_N];
int chain_index[MAX_N];
size_t chain_position[MAX_N];
int num_chains = 0;

vector<tree_node> tree[MAX_N];

char str[BUFFER_SIZE + 1];

char * node_to_str(const tree_node &obj)
{
	char str[200];

	sprintf(str, "[b(%d,%d) w(%d,%d) l(%d,%d) r(%d,%d) (%s)]",
		obj.best_sum[0], obj.best_sum[1], obj.whole_sum[0], obj.whole_sum[1],
		obj.left_sum[0], obj.left_sum[1], obj.right_sum[0], obj.right_sum[1],
		obj.flip ? "Y" : "N");
	return strdup(str);
}

void insert_node_in_cycle(int node)
{
	cycle_position[node] = cycle.size();
	cycle.push_back(node);
	in_cycle[node] = true;
}

void insert_node_in_chain(int chain, int node)
{
	chain_position[node] = chains[chain].size();
	chains[chain].push_back(node);
	chain_index[node] = chain;
}

void initial_dfs(int root)
{
	visited[root] = true;

	for (vector<int>::iterator it = neighbours[root].begin(); it != neighbours[root].end(); ++it) {
		int node = *it;
		if (visited[node]) {
			if (node != parent[root] && cycle_start < 0 && cycle_end < 0) {
				cycle_end = root;
				cycle_start = node;
			}
		} else {
			parent[node] = root;
			initial_dfs(node);
		}
	}
}

void fill_cycle()
{
	int x = cycle_end;
	while (x != cycle_start) {
		insert_node_in_cycle(x);
		x = parent[x];
	}
	insert_node_in_cycle(x);
}

void dfs(int root)
{
	int heavy_node = -1;
	bool leaf = true;

	visited[root] = true;
	subtree[root] = 1;

	for (vector<int>::iterator it = neighbours[root].begin(); it != neighbours[root].end(); ++it) {
		int node = *it;
		if (!visited[node] && !in_cycle[node]) {
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
		insert_node_in_chain(num_chains, root);
		++num_chains;
	} else {
		int heavy_chain = chain_index[heavy_node];
		insert_node_in_chain(heavy_chain, root);
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

void combine_nodes(tree_node &parent, const tree_node &left, const tree_node &right)
{
	if (left.size == 0 || right.size == 0) {
		parent = (left.size == 0) ? right : left;
		return;
	}

	for (int i = 0; i < 2; i++) {
		parent.whole_sum[i] = left.whole_sum[i] + right.whole_sum[i];

		parent.best_sum[i] = 0;

		int left_cand = left.best_sum[i];
		if (left_cand > parent.best_sum[i])
			parent.best_sum[i] = left_cand;

		int right_cand = right.best_sum[i];
		if (right_cand > parent.best_sum[i])
			parent.best_sum[i] = right_cand;

		int both_cand = left.right_sum[i] + right.left_sum[i];
		if (both_cand > parent.best_sum[i])
			parent.best_sum[i] = both_cand;

		parent.left_sum[i] = left.left_sum[i];
		if (right.left_sum[i] > 0) {
			int cand = left.whole_sum[i] + right.left_sum[i];
			if (cand > parent.left_sum[i])
				parent.left_sum[i] = cand;
		}

		parent.right_sum[i] = right.right_sum[i];
		if (left.right_sum[i] > 0) {
			int cand = right.whole_sum[i] + left.right_sum[i];
			if (cand > parent.right_sum[i])
				parent.right_sum[i] = cand;
		}

	}

	parent.size = left.size + right.size;
}

void build_interval_tree(vector<tree_node> &tree, const vector<int> &chains,
	const int *value)
{
	const int num_bits = get_upper_exp(chains.size());
	size_t tree_size = pow2(num_bits + 1) - 1;
	tree.resize(tree_size);

	size_t offset = pow2(num_bits) - 1;
	for (size_t i = 0; i < chains.size(); i++) {
		int elem = value[chains[i]];
		tree[offset + i].whole_sum[0] = elem;
		tree[offset + i].whole_sum[1] = -elem;
		if (elem >= 0) {
			tree[offset + i].best_sum[0] = elem;
			tree[offset + i].left_sum[0] = elem;
			tree[offset + i].right_sum[0] = elem;
		}
		if (elem <= 0) {
			tree[offset + i].best_sum[1] = -elem;
			tree[offset + i].left_sum[1] = -elem;
			tree[offset + i].right_sum[1] = -elem;
		}
		tree[offset + i].size = 1;
	}

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

void reflect_node(tree_node &node)
{
	for (int i = 0; i < 2; i++)
		swap(node.left_sum[i], node.right_sum[i]);
}

void flip_node(tree_node &node)
{
	swap(node.whole_sum[0], node.whole_sum[1]);
	swap(node.best_sum[0], node.best_sum[1]);

	swap(node.left_sum[0], node.left_sum[1]);
	swap(node.right_sum[0], node.right_sum[1]);
}

void propagate_flips(vector<tree_node> &tree, tree_node &node,
	int left_index, int right_index, bool has_children)
{
	if (node.flip) {
		flip_node(node);
		node.flip = false;
		if (has_children) {
			tree[(size_t) left_index].flip ^= 1;
			tree[(size_t) right_index].flip ^= 1;
		}
	}
}

tree_node update_tree(vector<tree_node> &tree, int index, int start, int end,
	int left, int right)
{
	tree_node &node = tree[(size_t) index];

	int left_index = (index << 1) + 1;
	int right_index = left_index + 1;

	propagate_flips(tree, node, left_index, right_index, start < end);

	if (right < start || end < left)
		return node;

	if (left <= start && end <= right) {
		flip_node(node);
		if (start < end) {
			tree[(size_t) left_index].flip ^= 1;
			tree[(size_t) right_index].flip ^= 1;
		}
	} else {
		int mid = start + ((end - start) >> 1);
		tree_node left_node = update_tree(tree, left_index, start, mid,
			left, right);
		tree_node right_node = update_tree(tree, right_index, mid + 1, end,
			left, right);
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

tree_node update_full_tree(vector<tree_node> &tree, int left, int right)
{
	return update_tree(tree, 0, 0, tree.size() / 2, left, right);
}

tree_node query_full_tree(vector<tree_node> &tree, int left, int right)
{
	return query_tree(tree, 0, 0, tree.size() / 2, left, right);
}

void update_side_note(int x, int chain)
{
	update_full_tree(tree[chain], chain_position[x], chains[chain].size() - 1);
}

void query_side_note(tree_node &side_node, int x, int chain)
{
	tree_node cand_node = query_full_tree(tree[chain], chain_position[x], chains[chain].size() - 1);
	side_node = side_node.combine(cand_node);
}

int solve_problem()
{
	int n, q;

	if (scanf("%d", &n) != 1)
		return 1;

	for (int i = 0; i < n; i++) {
		int x, y, elem;
		if (scanf("%d %d %d", &x, &y, &elem) != 3)
			return 1;
		--x;
		--y;
		neighbours[x].push_back(y);
		neighbours[y].push_back(x);
		edge_value[pair<int, int>(x, y)] = elem;
		edge_value[pair<int, int>(y, x)] = elem;
	}

	fill(visited, visited + n, false);
	parent[0] = -1;
	initial_dfs(0);
	fill_cycle();

	fill(visited, visited + n, false);
	for (int i = 0; i < n; i++)
		if (in_cycle[i]) {
			level[i] = 0;
			parent[i] = -1;
			dfs(i);
		}

	for (int i = 0; i < n; i++)
		if (parent[i] >= 0)
			value[i] = edge_value.at(pair<int, int>(i, parent[i]));

	for (size_t j = 0; j < cycle.size(); j++) {
		int crt_node = cycle[j];
		int next_node = cycle[(j + 1) % cycle.size()];
		cycle_value[crt_node] = edge_value.at(pair<int, int>(crt_node, next_node));
	}

	for (int k = 0; k < num_chains; k++)
		build_interval_tree(tree[k], chains[k], value);

	build_interval_tree(tree[num_chains], cycle, cycle_value);

	if (scanf("%d", &q) != 1)
		return 1;

	for (int i = 0; i < q; i++) {
		int x, y;
		if (scanf("%s %d %d", str, &x, &y) != 3)
			return 1;
		--x;
		--y;
		if (str[0] == 'f') {
			int px = -1, py = -1;
			while (x >= 0 && y >= 0) {
				int x_chain = chain_index[x];
				int y_chain = chain_index[y];
				px = chains[x_chain].back();
				py = chains[y_chain].back();

				if (px == py) {
					size_t left = chain_position[x];
					size_t right = chain_position[y];
					if (left < right) {
						update_full_tree(tree[y_chain], left, right - 1);
					} else if (left > right) {
						update_full_tree(tree[y_chain], right, left - 1);
					}
					break;
				} else {
					if (level[px] <= level[py]) {
						update_side_note(y, y_chain);
						y = parent[py];
					} else {
						update_side_note(x, x_chain);
						x = parent[px];
					}
				}
			}
			if (px != py) {
				while (x >= 0) {
					int x_chain = chain_index[x];
					px = chains[x_chain].back();
					update_side_note(x, x_chain);
					x = parent[px];
				}
				while (y >= 0) {
					int y_chain = chain_index[y];
					py = chains[y_chain].back();
					update_side_note(y, y_chain);
					y = parent[py];
				}
				size_t qx = cycle_position[px];
				size_t qy = cycle_position[py];
				if (qx < qy) {
					if (2 * (qy - qx) < cycle.size()) {
						update_full_tree(tree[num_chains], qx, qy - 1);
					} else {
						update_full_tree(tree[num_chains], qy, tree[num_chains].size() - 1);
						if (qx > 0)
							update_full_tree(tree[num_chains], 0, qx - 1);
					}
				} else {
					if (2 * (qx - qy) < cycle.size()) {
						update_full_tree(tree[num_chains], qy, qx - 1);
					} else {
						update_full_tree(tree[num_chains], qx, tree[num_chains].size() - 1);
						if (qy > 0)
							update_full_tree(tree[num_chains], 0, qy - 1);
					}
				}
			}
		} else {
			tree_node best_node;
			tree_node left_node;
			tree_node right_node;
			int px = -1, py = -1;
			while (x >= 0 && y >= 0) {
				int x_chain = chain_index[x];
				int y_chain = chain_index[y];
				px = chains[x_chain].back();
				py = chains[y_chain].back();

				if (px == py) {
					size_t left = chain_position[x];
					size_t right = chain_position[y];
					if (left < right) {
						tree_node cand_node = query_full_tree(tree[y_chain], left, right - 1);
						left_node = left_node.combine(cand_node);
					} else if (left > right) {
						tree_node cand_node = query_full_tree(tree[y_chain], right, left - 1);
						right_node = right_node.combine(cand_node);
					}
					break;
				} else {
					if (level[px] <= level[py]) {
						query_side_note(right_node, y, y_chain);
						y = parent[py];
					} else {
						query_side_note(left_node, x, x_chain);
						x = parent[px];
					}
				}
			}
			if (px != py) {
				while (x >= 0) {
					int x_chain = chain_index[x];
					px = chains[x_chain].back();
					query_side_note(left_node, x, x_chain);
					x = parent[px];
				}
				while (y >= 0) {
					int y_chain = chain_index[y];
					py = chains[y_chain].back();
					query_side_note(right_node, y, y_chain);
					y = parent[py];
				}
				size_t qx = cycle_position[px];
				size_t qy = cycle_position[py];
				tree_node cand_node;
				if (qx < qy) {
					if (2 * (qy - qx) < cycle.size()) {
						cand_node = query_full_tree(tree[num_chains], qx, qy - 1);
					} else {
						cand_node = query_full_tree(tree[num_chains], qy, tree[num_chains].size() - 1);
						if (qx > 0) {
							tree_node first_node = query_full_tree(tree[num_chains], 0, qx - 1);
							cand_node = cand_node.combine(first_node);
						}
						reflect_node(cand_node);
					}
					left_node = left_node.combine(cand_node);
				} else {
					if (2 * (qx - qy) < cycle.size()) {
						cand_node = query_full_tree(tree[num_chains], qy, qx - 1);
					} else {
						tree_node last_node = query_full_tree(tree[num_chains], qx, tree[num_chains].size() - 1);
						tree_node first_node;
						if (qy > 0)
							first_node = query_full_tree(tree[num_chains], 0, qy - 1);
						cand_node = last_node.combine(first_node);
						reflect_node(cand_node);
					}
					right_node = right_node.combine(cand_node);
				}
			}
			reflect_node(right_node);
			combine_nodes(best_node, left_node, right_node);
			printf("%d\n", best_node.best_sum[0]);
		}
	}

	return 0;
}

int main()
{
	int num_tests = 1;

	for (int i = 0; i < num_tests; i++) {
		int ret = solve_problem();
		if (ret != 0) {
			fprintf(stderr, "Read error at case #%d\n", i + 1);
			break;
		}
	}

	return 0;
}
