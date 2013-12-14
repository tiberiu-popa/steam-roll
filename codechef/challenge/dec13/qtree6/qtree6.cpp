#include <algorithm>
#include <limits>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#define MAX_N 100000
#define MAX_LG 18
#define BLACK 0

using namespace std;

struct tree_node {

	bool valid;
	int left_black, right_black;
	int left_white, right_white;
	bool full_black, full_white;
	int below[2];

	tree_node() : valid(false), left_black(0), right_black(0), left_white(0), right_white(0),
		full_black(false), full_white(false)
	{
		memset(below, 0, sizeof(below));
	}

	tree_node(int color) : valid(true), left_black(!color), right_black(!color),
		left_white(color), right_white(color), full_black(!color), full_white(color)
	{
		memset(below, 0, sizeof(below));
	}

	void set_color(int color)
	{
		left_black = !color;
		right_black = !color;
		left_white = color;
		right_white = color;
		full_black = !color;
		full_white = color;
	}

};

vector<tree_node> tree[MAX_N];
int colors[MAX_N];

vector<int> neighbours[MAX_N];
int parent[MAX_N];
int subtree[MAX_N];
bool visited[MAX_N];

vector<int> chains[MAX_N];
int chain_index[MAX_N];
size_t chain_position[MAX_N];
int num_chains;

vector<int> chain_neighbours[MAX_N];
vector<int> sorted_chains;

int db[MAX_N], dw[MAX_N];

int skip_whitespace()
{
	int ch;
	while (true) {
		ch = getchar_unlocked();
		if (ch != ' ' && ch != '\n')
			break;
	}
	return ch;
}

template<typename T>
T read_unsigned_integer()
{
	T result = (T) 0;
	int ch = skip_whitespace();
	while (ch >= '0' && ch <= '9') {
		result = 10 * result + (ch - '0');
		ch = getchar_unlocked();
	}
	return result;
}

template<typename T>
T read_signed_integer()
{
	T result = (T) 0;
	bool flip = false;
	int ch = skip_whitespace();
	if (ch == '-') {
		flip = true;
		ch = skip_whitespace();
	}
	while (ch >= '0' && ch <= '9') {
		result = 10 * result + (ch - '0');
		ch = getchar_unlocked();
	}
	return flip ? -result : result;
}

int read_character()
{
	return skip_whitespace();
}

template<class T>
void trim_unique(vector<T> &v)
{
	typename vector<T>::iterator it = unique(v.begin(), v.end());
	typename vector<T>::difference_type diff = it - v.begin();
	v.resize((typename vector<T>::size_type) diff);
}

void insert_node_in_chain(int chain, int node)
{
	chain_position[node] = chains[chain].size();
	chains[chain].push_back(node);
	chain_index[node] = chain;
}

void tree_bfs(const vector<int> *neighbours, int root, vector<int> &nodes)
{
	nodes.push_back(root);
	for (size_t k = 0; k < nodes.size(); k++) {
		int root = nodes[k];
		for (size_t i = 0; i < neighbours[root].size(); i++) {
			int node = neighbours[root][i];
			nodes.push_back(node);
		}
	}
}

void tree_bfs_parent(const vector<int> *neighbours, int root, vector<int> &nodes)
{
	nodes.push_back(root);
	visited[root] = true;
	for (size_t k = 0; k < nodes.size(); k++) {
		int root = nodes[k];
		for (size_t i = 0; i < neighbours[root].size(); i++) {
			int node = neighbours[root][i];
			if (!visited[node]) {
				nodes.push_back(node);
				visited[node] = true;
			} else {
				parent[root] = node;
			}
		}
	}
}

void heavy_light(int root)
{
	vector<int> nodes;

	tree_bfs_parent(neighbours, root, nodes);
	for (size_t k = nodes.size(); k > 0;) {
		--k;
		int root = nodes[k];
		int heavy_node = -1;
		bool leaf = true;
		subtree[root] = 1;

		for (size_t i = 0; i < neighbours[root].size(); i++) {
			int node = neighbours[root][i];
			if (node != parent[root]) {
				leaf = false;
				subtree[root] += subtree[node];
				if (heavy_node == -1 || subtree[node] > subtree[heavy_node]) {
					heavy_node = node;
				}
			}
		}

		if (leaf) {
			chains[num_chains].clear();
			insert_node_in_chain(num_chains, root);
			++num_chains;
		} else {
			int heavy_chain = chain_index[heavy_node];
			insert_node_in_chain(heavy_chain, root);
		}
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

void combine_nodes(tree_node &parent, const tree_node &left, const tree_node &right)
{
	if (left.valid) {
		if (right.valid) {
			parent.valid = true;
			if (left.full_black) {
				parent.left_black = left.left_black + right.left_black;
				parent.full_black = right.full_black;
			} else {
				parent.left_black = left.left_black;
				parent.full_black = false;
			}
			if (left.full_white) {
				parent.left_white = left.left_white + right.left_white;
				parent.full_white = right.full_white;
			} else {
				parent.left_white = left.left_white;
				parent.full_white = false;
			}
			if (right.full_black) {
				parent.right_black = left.right_black + right.right_black;
			} else {
				parent.right_black = right.right_black;
			}
			if (right.full_white) {
				parent.right_white = left.right_white + right.right_white;
			} else {
				parent.right_white = right.right_white;
			}
		} else {
			parent = left;
		}
	} else {
		if (right.valid) {
			parent = right;
		}
	}
	for (int i = 0; i < 2; i++)
		parent.below[i] = left.below[i] + right.below[i];
}

void combine_full_nodes(tree_node &parent, const tree_node &left, const tree_node &right)
{
	combine_nodes(parent, left, right);
}

void build_interval_tree(vector<tree_node> &tree, const vector<int> &chains)
{
	const int num_bits = get_upper_exp(chains.size());
	size_t tree_size = pow2(num_bits + 1) - 1;
	tree.resize(tree_size);

	size_t offset = pow2(num_bits) - 1;
	for (size_t i = 0; i < chains.size(); i++)
		tree[offset + i] = tree_node(BLACK);
	for (size_t i = offset + chains.size(); i < tree_size; i++)
		tree[i] = tree_node();

	for (int k = num_bits - 1; k >= 0; k--) {
		offset = pow2(k) - 1;
		for (size_t i = 0; i <= offset; i++) {
			size_t index = offset + i;
			size_t left_index = 2 * index + 1;
			size_t right_index = left_index + 1;
			tree_node &parent = tree[index];
			tree_node &left = tree[left_index];
			tree_node &right = tree[right_index];
			combine_full_nodes(parent, left, right);
		}
	}
}

tree_node & update_tree_color(vector<tree_node> &tree, int index, int start, int end, int left)
{
	tree_node &node = tree[(size_t) index];

	int left_index = (index << 1) + 1;
	int right_index = left_index + 1;

	if (left < start || end < left)
		return node;

	if (left <= start && end <= left) {
		int color = node.full_white;
		node.set_color(color ^ 1);
	} else {
		int mid = start + ((end - start) >> 1);
		tree_node &left_node = update_tree_color(tree, left_index, start, mid, left);
		tree_node &right_node = update_tree_color(tree, right_index, mid + 1, end, left);
		combine_nodes(node, left_node, right_node);
	}

	return node;
}

tree_node update_tree_below(vector<tree_node> &tree, int index, int start, int end,
	int left, int black_value, int white_value)
{
	pair<size_t, size_t> nodes[MAX_LG];
	int num_nodes = 0;

	nodes[num_nodes++] = make_pair((size_t) index, numeric_limits<size_t>::max());

	while (left >= start && end >= left) {
		tree_node &node = tree[(size_t) index];

		int left_index = (index << 1) + 1;
		int right_index = left_index + 1;

		if (left <= start && end <= left) {
			node.below[0] += black_value;
			node.below[1] += white_value;
			break;
		} else {
			int mid = start + ((end - start) >> 1);
			if (left <= mid) {
				nodes[num_nodes++] = make_pair(left_index, right_index);
				index = left_index;
				end = mid;
			} else {
				nodes[num_nodes++] = make_pair(right_index, left_index);
				index = right_index;
				start = mid + 1;
			}
		}
	}

	for (int i = num_nodes - 1; i > 0; i--) {
		tree_node &node = tree[nodes[i - 1].first];
		size_t l = nodes[i].first;
		size_t r = nodes[i].second;
		if (l > r)
			swap(l, r);
		tree_node &left_node = tree[l];
		tree_node &right_node = tree[r];
		combine_nodes(node, left_node, right_node);
	}

	return tree[nodes[0].first];
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

	if (left <= start && end <= right)
		return parent_node;

	int mid = start + ((end - start) >> 1);
	tree_node left_node = query_tree(tree, left_index, start, mid,
		left, right);
	tree_node right_node = query_tree(tree, right_index, mid + 1, end,
		left, right);
	combine_full_nodes(best_node, left_node, right_node);
	return best_node;
}

int query_tree_below(vector<tree_node> &tree, int index, int start, int end,
	int left, int right, int t)
{
	int total_below = 0;

	if (right < start || end < left)
		return total_below;

	int left_index = (index << 1) + 1;
	int right_index = left_index + 1;
	tree_node &parent_node = tree[(size_t) index];

	if (left <= start && end <= right)
		return parent_node.below[t];

	int mid = start + ((end - start) >> 1);
	total_below += query_tree_below(tree, left_index, start, mid, left, right, t);
	total_below += query_tree_below(tree, right_index, mid + 1, end, left, right, t);
	return total_below;
}

tree_node & update_full_tree_color(vector<tree_node> &tree, int left)
{
	return update_tree_color(tree, 0, 0, (int) (tree.size() / 2), left);
}

tree_node update_full_tree_below(vector<tree_node> &tree, int left, int black_value, int white_value)
{
	return update_tree_below(tree, 0, 0, (int) (tree.size() / 2), left, black_value, white_value);
}

tree_node query_full_tree(vector<tree_node> &tree, int left, int right)
{
	return query_tree(tree, 0, 0, (int) (tree.size() / 2), left, right);
}

int query_full_tree_below(vector<tree_node> &tree, int left, int right, int t)
{
	return query_tree_below(tree, 0, 0, (int) (tree.size() / 2), left, right, t);
}

int query_full_tree_below_from(vector<tree_node> &tree, int left, int t)
{
	int right = (int) (tree.size() / 2);
	return query_tree_below(tree, 0, 0, right, left, right, t);
}

int solve_problem()
{
	int n, m;

	n = read_unsigned_integer<int>();

	num_chains = 0;

	for (int i = 0; i < n; i++)
		neighbours[i].clear();

	for (int i = 1; i < n; i++) {
		int x = read_unsigned_integer<int>() - 1;
		int y = read_unsigned_integer<int>() - 1;
		neighbours[x].push_back(y);
		neighbours[y].push_back(x);
	}

	fill(visited, visited + n, false);
	parent[0] = -1;
	heavy_light(0);

	for (int i = 0; i < num_chains; i++)
		chain_neighbours[i].clear();

	int root_chain = -1;
	for (int i = 0; i < num_chains; i++) {
		int head = chains[i].back();
		int p = parent[head];
		if (p >= 0)
			chain_neighbours[chain_index[p]].push_back(i);
		else
			root_chain = i;
	}

	sorted_chains.clear();
	tree_bfs(chain_neighbours, root_chain, sorted_chains);
	reverse(sorted_chains.begin(), sorted_chains.end());

	for (int k = 0; k < num_chains; k++)
		build_interval_tree(tree[k], chains[k]);

	for (size_t i = 0; i < sorted_chains.size(); i++) {
		int k = sorted_chains[i];
		int head = chains[k].back();
		int p = parent[head];
		if (p >= 0) {
			int chain = chain_index[p];
			int chpos = (int) chain_position[p];
			tree_node entry = tree[k].front();
			int diff = entry.right_black;
			int left = ((int) chains[k].size()) - diff;
			int below = query_full_tree_below_from(tree[k], left, BLACK);
			below += diff;
			db[head] = below;
			update_full_tree_below(tree[chain], chpos, below, BLACK);
		}
	}

	m = read_unsigned_integer<int>();

	for (int i = 0; i < m; i++) {
		int t = read_unsigned_integer<int>();
		int u = read_unsigned_integer<int>() - 1;
		int chain = chain_index[u];
		int chpos = (int) chain_position[u];
		if (t) {
			update_full_tree_color(tree[chain], chpos);
			colors[u] ^= 1;

			while (true) {
				int head = chains[chain].back();
				int diff = max(tree[chain].front().right_black, tree[chain].front().right_white);
				int left = ((int) chains[chain].size()) - diff;
				int p = parent[head];
				if (p < 0)
					break;
				int below = query_full_tree_below_from(tree[chain], left, colors[head]);
				below += diff;
				chain = chain_index[p];
				chpos = (int) chain_position[p];
				int color = colors[head];
				if (color) {
					update_full_tree_below(tree[chain], chpos, -db[head], below - dw[head]);
					db[head] = 0;
					dw[head] = below;
				} else {
					update_full_tree_below(tree[chain], chpos, below - db[head], -dw[head]);
					db[head] = below;
					dw[head] = 0;
				}
			}
		} else {
			int root = u;
			int color = colors[u];

			while (true) {
				int head = chains[chain].back();
				tree_node entry = query_full_tree(tree[chain], chpos, (int) (chains[chain].size() - 1));
				int diff = max(entry.left_black, entry.left_white);
				int idx = chpos + diff - 1;
				root = chains[chain][(size_t) idx];
				if (root != head)
					break;
				int p = parent[head];
				if (p < 0)
					break;
				if (color != colors[p])
					break;
				color = colors[p];
				chain = chain_index[p];
				chpos = (int) chain_position[p];
			}
			chain = chain_index[root];
			chpos = (int) chain_position[root];
			tree_node entry = query_full_tree(tree[chain], 0, chpos);
			int diff = max(entry.right_black, entry.right_white);
			int left = chpos - diff + 1;
			int below = query_full_tree_below(tree[chain], left, chpos, color);
			below += diff;
			printf("%d\n", below);
		}
	}

	for (int i = 0; i < num_chains; i++)
		chains[i].clear();

	return 0;
}

int main()
{
	solve_problem();
	return 0;
}
