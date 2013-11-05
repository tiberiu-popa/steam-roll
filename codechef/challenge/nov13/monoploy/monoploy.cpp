#include <algorithm>
#include <limits>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#define MAX_N 100000
#define MAX_LG 18
#define BUFFER_SIZE 100

using namespace std;

struct tree_node {

	long long surface;
	long long below;
	long long max_below;
	long long delta_below;
	int surface_count;
	bool flip;

	tree_node()
	{
		memset(this, 0, sizeof(*this));
	}

	tree_node(int surface) : surface(surface), below(0), max_below(0), delta_below(0),
		surface_count(surface > 0), flip(false) { }

	long long sum() const
	{
		return surface + below + max_below - delta_below;
	}

};

vector<tree_node> tree[MAX_N];

char str[BUFFER_SIZE + 10];

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

int head_connection[MAX_N];

char *trim_line(char *str)
{
	return strtok(str, "\r\n");
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
	int k = get_msb(n);
	size_t result = pow2(k - 1);
	if (result == n)
		return k - 1;
	else
		return k;
}

void combine_nodes(tree_node &parent, const tree_node &left, const tree_node &right)
{
	parent.surface = left.surface + right.surface;
	parent.surface_count = left.surface_count + right.surface_count;
	parent.below = left.below + right.below;
	parent.delta_below = left.delta_below + right.delta_below;
}

void combine_full_nodes(tree_node &parent, const tree_node &left, const tree_node &right)
{
	combine_nodes(parent, left, right);
	parent.max_below = left.max_below + right.max_below;
}

void build_interval_tree(vector<tree_node> &tree, const vector<int> &chains,
	const int *value)
{
	const int num_bits = get_upper_exp(chains.size());
	size_t tree_size = pow2(num_bits + 1) - 1;
	tree.resize(tree_size);

	size_t offset = pow2(num_bits) - 1;
	for (size_t i = 0; i < chains.size(); i++) {
		int node = chains[i];
		int elem = (i + 1 < chains.size()) ? value[node] : 0;
		tree[offset + i] = tree_node(elem);
	}
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
			parent.flip = false;
		}
	}
}

void flip_node(tree_node &node)
{
	node.surface = 0;
	node.surface_count = 0;
	node.delta_below = 0;
}

void propagate_flips(vector<tree_node> &tree, tree_node &node,
	int left_index, int right_index, bool has_children)
{
	if (node.flip) {
		flip_node(node);
		node.flip = false;
		if (has_children) {
			tree[(size_t) left_index].flip = true;
			tree[(size_t) right_index].flip = true;
		}
	}
}

void touch_node(vector<tree_node> &tree, int index, bool has_children)
{
	tree_node &node = tree[(size_t) index];

	int left_index = (index << 1) + 1;
	int right_index = left_index + 1;

	propagate_flips(tree, node, left_index, right_index, has_children);
}

tree_node & update_tree_surface(vector<tree_node> &tree, int index, int start, int end,
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
			tree[(size_t) left_index].flip = true;
			tree[(size_t) right_index].flip = true;
		}
	} else {
		int mid = start + ((end - start) >> 1);
		tree_node &left_node = update_tree_surface(tree, left_index, start, mid,
			left, right);
		tree_node &right_node = update_tree_surface(tree, right_index, mid + 1, end,
			left, right);
		combine_nodes(node, left_node, right_node);
	}

	return node;
}

tree_node & update_tree_single_surface(vector<tree_node> &tree, int index, int start, int end,
	int left, int value)
{
	tree_node &node = tree[(size_t) index];

	int left_index = (index << 1) + 1;
	int right_index = left_index + 1;

	propagate_flips(tree, node, left_index, right_index, start < end);

	if (left < start || end < left)
		return node;

	if (left <= start && end <= left) {
		node.surface = value;
		node.surface_count = (value > 0);
	} else {
		int mid = start + ((end - start) >> 1);
		tree_node &left_node = update_tree_single_surface(tree, left_index, start, mid,
			left, value);
		tree_node &right_node = update_tree_single_surface(tree, right_index, mid + 1, end,
			left, value);
		combine_nodes(node, left_node, right_node);
	}

	return node;
}

tree_node update_tree_max_below(vector<tree_node> &tree, int index, int start, int end,
	int left, long long value, long long max_value)
{
	pair<size_t, size_t> nodes[MAX_LG];
	int num_nodes = 0;

	nodes[num_nodes++] = make_pair((size_t) index, numeric_limits<size_t>::max());

	while (left >= start && end >= left) {
		tree_node &node = tree[(size_t) index];

		int left_index = (index << 1) + 1;
		int right_index = left_index + 1;

		propagate_flips(tree, node, left_index, right_index, start < end);

		if (left <= start && end <= left) {
			node.below += value;
			node.max_below += max_value;
			break;
		} else {
			int mid = start + ((end - start) >> 1);
			if (left <= mid) {
				nodes[num_nodes++] = make_pair(left_index, right_index);
				touch_node(tree, right_index, mid + 1 < end);
				index = left_index;
				end = mid;
			} else {
				nodes[num_nodes++] = make_pair(right_index, left_index);
				touch_node(tree, left_index, start < mid);
				index = right_index;
				start = mid + 1;
			}
		}
	}

	for (int i = num_nodes - 1; i > 0; i--) {
		tree_node &node = tree[nodes[i - 1].first];
		tree_node &left_node = tree[nodes[i].first];
		tree_node &right_node = tree[nodes[i].second];
		combine_full_nodes(node, left_node, right_node);
	}

	return tree[nodes[0].first];
}

tree_node update_tree_below(vector<tree_node> &tree, int index, int start, int end,
	int left, long long value)
{
	pair<size_t, size_t> nodes[MAX_LG];
	int num_nodes = 0;

	nodes[num_nodes++] = make_pair((size_t) index, numeric_limits<size_t>::max());

	while (left >= start && end >= left) {
		tree_node &node = tree[(size_t) index];

		int left_index = (index << 1) + 1;
		int right_index = left_index + 1;

		propagate_flips(tree, node, left_index, right_index, start < end);

		if (left <= start && end <= left) {
			node.below += value;
			break;
		} else {
			int mid = start + ((end - start) >> 1);
			if (left <= mid) {
				nodes[num_nodes++] = make_pair(left_index, right_index);
				touch_node(tree, right_index, mid + 1 < end);
				index = left_index;
				end = mid;
			} else {
				nodes[num_nodes++] = make_pair(right_index, left_index);
				touch_node(tree, left_index, start < mid);
				index = right_index;
				start = mid + 1;
			}
		}
	}

	for (int i = num_nodes - 1; i > 0; i--) {
		tree_node &node = tree[nodes[i - 1].first];
		tree_node &left_node = tree[nodes[i].first];
		tree_node &right_node = tree[nodes[i].second];
		combine_nodes(node, left_node, right_node);
	}

	return tree[nodes[0].first];
}

tree_node &update_tree_delta(vector<tree_node> &tree, int index, int start, int end,
	int left, int delta)
{
	pair<size_t, size_t> nodes[MAX_LG];
	int num_nodes = 0;

	nodes[num_nodes++] = make_pair((size_t) index, numeric_limits<size_t>::max());

	while (left >= start && end >= left) {
		tree_node &node = tree[(size_t) index];

		int left_index = (index << 1) + 1;
		int right_index = left_index + 1;

		propagate_flips(tree, node, left_index, right_index, start < end);

		if (left <= start && end <= left) {
			node.delta_below += delta;
			break;
		} else {
			int mid = start + ((end - start) >> 1);
			if (left <= mid) {
				nodes[num_nodes++] = make_pair(left_index, right_index);
				touch_node(tree, right_index, mid + 1 < end);
				index = left_index;
				end = mid;
			} else {
				nodes[num_nodes++] = make_pair(right_index, left_index);
				touch_node(tree, left_index, start < mid);
				index = right_index;
				start = mid + 1;
			}
		}
	}

	for (int i = num_nodes - 1; i > 0; i--) {
		tree_node &node = tree[nodes[i - 1].first];
		tree_node &left_node = tree[nodes[i].first];
		tree_node &right_node = tree[nodes[i].second];
		combine_nodes(node, left_node, right_node);
	}

	return tree[nodes[0].first];
}

tree_node query_tree_single(vector<tree_node> &tree, int index, int start, int end,
	int left)
{
	tree_node best_node;

	while (left >= start && end >= left) {
		int left_index = (index << 1) + 1;
		int right_index = left_index + 1;
		tree_node &parent_node = tree[(size_t) index];

		propagate_flips(tree, parent_node, left_index, right_index, start < end);

		if (left <= start && end <= left)
			return parent_node;

		int mid = start + ((end - start) >> 1);
		if (left <= mid) {
			index = left_index;
			end = mid;
		} else {
			index = right_index;
			start = mid + 1;
		}
	}

	return best_node;
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
	combine_full_nodes(best_node, left_node, right_node);
	return best_node;
}

tree_node & update_full_tree_surface_from(vector<tree_node> &tree, int left)
{
	int right = (int) (tree.size() / 2);
	return update_tree_surface(tree, 0, 0, right, left, right);
}

tree_node & update_full_tree_surface(vector<tree_node> &tree, int left, int right)
{
	return update_tree_surface(tree, 0, 0, (int) (tree.size() / 2), left, right);
}

tree_node & update_full_tree_single_surface(vector<tree_node> &tree, int left, int value)
{
	return update_tree_single_surface(tree, 0, 0, (int) (tree.size() / 2), left, value);
}

tree_node update_full_tree_max_below(vector<tree_node> &tree, int left, long long value, long long max_value)
{
	return update_tree_max_below(tree, 0, 0, (int) (tree.size() / 2), left, value, max_value);
}

tree_node update_full_tree_below(vector<tree_node> &tree, int left, long long value)
{
	return update_tree_below(tree, 0, 0, (int) (tree.size() / 2), left, value);
}

tree_node & update_full_tree_delta(vector<tree_node> &tree, int left, int delta)
{
	return update_tree_delta(tree, 0, 0, (int) (tree.size() / 2), left, delta);
}

tree_node query_full_tree_single(vector<tree_node> &tree, int left)
{
	return query_tree_single(tree, 0, 0, (int) (tree.size() / 2), left);
}

tree_node query_full_tree_from(vector<tree_node> &tree, int left)
{
	int right = (int) (tree.size() / 2);
	return query_tree(tree, 0, 0, right, left, right);
}

tree_node query_full_tree(vector<tree_node> &tree, int left, int right)
{
	return query_tree(tree, 0, 0, (int) (tree.size() / 2), left, right);
}

int solve_problem()
{
	int n, m;

	if (fgets(str, sizeof(str), stdin) == NULL)
		return 1;
	n = atoi(str);

	num_chains = 0;

	for (int i = 0; i < n; i++)
		neighbours[i].clear();

	for (int i = 1; i < n; i++) {
		if (fgets(str, sizeof(str), stdin) == NULL)
			return 1;
		trim_line(str);
		char *p = strtok(str, " ");
		int x = atoi(p);
		p = strtok(NULL, " ");
		int y = atoi(p);
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
		build_interval_tree(tree[k], chains[k], subtree);

	fill(head_connection, head_connection + n, -1);
	for (size_t i = 0; i < sorted_chains.size(); i++) {
		int k = sorted_chains[i];
		int head = chains[k].back();
		int p = parent[head];
		if (p >= 0) {
			int chain = chain_index[p];
			int chpos = (int) chain_position[p];
			tree_node entry = tree[k].front();
			long long sum = entry.surface + entry.below + entry.max_below;
			update_full_tree_max_below(tree[chain], chpos, sum, subtree[head]);
		}
	}

	if (fgets(str, sizeof(str), stdin) == NULL)
		return 1;
	m = atoi(str);

	for (int i = 0; i < m; i++) {
		if (fgets(str, sizeof(str), stdin) == NULL)
			return 1;
		trim_line(str);
		char *p = strtok(str, " ");
		bool query = (strcmp(p, "q") == 0);
		p = strtok(NULL, " ");
		int u = atoi(p);
		int chain = chain_index[u];
		int chpos = (int) chain_position[u];
		if (query) {
			tree_node elem = query_full_tree(tree[chain], 0, chpos);
			long long result = elem.sum();
			elem = query_full_tree_single(tree[chain], chpos);
			result -= elem.surface;
			while (true) {
				int head = chains[chain].back();
				elem = query_full_tree_from(tree[chain], chpos);
				result += elem.surface_count * subtree[u];
				int p = parent[head];
				if (p < 0)
					break;
				chain = chain_index[p];
				chpos = (int) chain_position[p];
				elem = query_full_tree_single(tree[chain], chpos);
				if (elem.delta_below == 0 || head_connection[p] != head)
					result += subtree[u];
			}
			printf("%.10lf\n", (double) result / subtree[u]);
		} else {
			int head = chains[chain].back();
			int prev_head = -1;
			long long prev_sum = tree[chain].front().sum();
			while (true) {
				update_full_tree_surface_from(tree[chain], chpos);
				if (chpos > 0) {
					int lower_elem = chains[chain][(size_t) (chpos - 1)];
					update_full_tree_single_surface(tree[chain], chpos - 1, subtree[lower_elem]);
				}
				if (prev_head >= 0)
					update_full_tree_delta(tree[chain], chpos, subtree[prev_head]);
				int p = parent[head];
				if (p < 0)
					break;
				long long diff = tree[chain].front().sum() - prev_sum;
				head_connection[p] = head;
				prev_head = head;
				chain = chain_index[p];
				chpos = (int) chain_position[p];
				head = chains[chain].back();
				prev_sum = tree[chain].front().sum();
				update_full_tree_below(tree[chain], chpos, diff);
			}
			head_connection[u] = -1;
		}
	}

	for (int i = 0; i < num_chains; i++)
		chains[i].clear();

	return 0;
}

int main()
{
	int num_tests;

	if (fgets(str, sizeof(str), stdin) == NULL)
		return 1;
	num_tests = atoi(str);

	for (int i = 0; i < num_tests; i++)
		solve_problem();

	return 0;
}
