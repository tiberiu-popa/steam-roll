#include <algorithm>
#include <set>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#define MAX_N 200000

using namespace std;

vector<int> neighbours[MAX_N];
int parent[MAX_N];
int subtree[MAX_N];
bool color[MAX_N];

vector<int> chains[MAX_N];
int chain_index[MAX_N];
size_t chain_position[MAX_N];
int num_chains;

vector<int> chain_neighbours[MAX_N];
vector<int> sorted_chains;
set< pair< pair<int, int>, int> > intersections[MAX_N];
pair<int, int> head_optima[MAX_N];

pair<int, int> v[2][MAX_N];
int tree[2][MAX_N];
int tree_offsets[MAX_N];
int tree_sizes[MAX_N];

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
		for (size_t i = 0; i < neighbours[root].size(); i++)
			nodes.push_back(neighbours[root][i]);
	}
}

void heavy_light(int root)
{
	vector<int> nodes;

	tree_bfs(neighbours, root, nodes);
	for (size_t k = nodes.size(); k > 0;) {
		--k;
		int root = nodes[k];
		int heavy_node = -1;
		subtree[root] = 1;

		for (size_t i = 0; i < neighbours[root].size(); i++) {
			int node = neighbours[root][i];
			subtree[root] += subtree[node];
			if (heavy_node == -1 || subtree[node] > subtree[heavy_node]) {
				heavy_node = node;
			}
		}

		if (neighbours[root].empty()) {
			chains[num_chains].clear();
			insert_node_in_chain(num_chains, root);
			++num_chains;
		} else {
			int heavy_chain = chain_index[heavy_node];
			insert_node_in_chain(heavy_chain, root);
		}
	}
}

int bit_get_prefix_max(const int *tree, const pair<int, int> *v, int idx)
{
	int max_index = -1;

	while (idx >= 0) {
		int q = tree[idx];
		if (max_index < 0 || v[q] > v[max_index])
			max_index = q;
		idx = (idx & (idx + 1)) - 1;
	}

	return max_index;
}

int bit_get_interval_max(const int *tree, const pair<int, int> *v, int l, int r)
{
	int max_index = -1;

	for (int idx = (r & (r + 1)) - 1; l <= r; r = idx, idx = (idx & (idx + 1)) - 1) {
		int q;
		if (idx + 1 >= l) {
			q = tree[r];
		} else {
			q = r;
			idx = r - 1;
		}
		if (max_index < 0 || v[q] > v[max_index])
			max_index = q;
	}

	return max_index;
}
/*
int bit_get_element(int idx)
{
	int value;
	int parent;

	value = tree[idx];
	if (idx > 0) {
		parent = idx - ((idx + 1) & -(idx + 1));
		--idx;
		while (idx != parent) {
			value -= tree[idx];
			idx -= (idx + 1) & -(idx + 1);
		}
	}

	return value;
}
*/
int bit_set_element(int *tree, pair<int, int> *v, int tree_size, int idx, pair<int, int> value)
{
	v[idx] = value;
	for (int x = idx; x < tree_size; x |= (x + 1)) {
		if (v[tree[x]] == v[idx]) {
			int z = -1;
			if (x > 0) {
				int left = x & (x + 1);
				int right = x - 1;
				if (left <= right)
					z = bit_get_interval_max(tree, v, left, right) ;
			}
			tree[x] = (z >= 0 && v[z] > v[x]) ? z : x;
		} else {
			if (v[idx] > v[tree[x]])
				tree[x] = idx;
		}
	}
	return 0;
}

pair<int, int> add_first(pair<int, int> x, int delta)
{
	return make_pair(x.first + delta, x.second);
}

void add_to_first(pair<int, int> &x, int delta)
{
	x.first += delta;
}

pair<int, int> get_interval_max(int *tree, pair<int, int> *v, int left, int right)
{
	int idx = bit_get_interval_max(tree, v, left, right);
	return v[idx];
}

pair<int, int> offset_interval_max(int query, int offset, int left, int right, int pos)
{
	pair<int, int> res = get_interval_max(tree[query] + offset, v[query] + offset, left, right);
	add_to_first(res, (query != 0) ? -pos : pos);
	return res;
}

pair<int, int> get_prefix_max(int *tree, pair<int, int> *v, int right)
{
	int idx = bit_get_prefix_max(tree, v, right);
	return v[idx];
}

pair<int, int> offset_prefix_max(int query, int offset, int right, int pos)
{
	pair<int, int> res = get_prefix_max(tree[query] + offset, v[query] + offset, right);
	add_to_first(res, (query != 0) ? -pos : pos);
	return res;
}

void set_element(int offset, int tree_size, int chpos, pair<int, int> value)
{
	int pos = offset + chpos;
	bit_set_element(tree[0] + offset, v[0] + offset, tree_size, chpos, add_first(value, -pos));
	bit_set_element(tree[1] + offset, v[1] + offset, tree_size, chpos, add_first(value, pos));
}

void initial_update_value(const vector<int> &chain, int offset, int tree_size)
{
	for (size_t j = 0; j < chain.size(); j++) {
		int k = chain[j];
		if (!intersections[k].empty()) {
			int chpos = chain_position[k];
			int pos = offset + chpos;
			pair<int, int> cand = intersections[k].rbegin()->first;
			pair<int, int> value = add_first(v[0][pos], pos);
			if (cand > value)
				set_element(offset, tree_size, chpos, cand);
		}
	}
}

void update_value(int q, int n, int chain, int offset, int chpos, int pos)
{
	while (true) {
		pair<int, int> value = add_first(v[0][pos], pos);
		pair<int, int> cand = make_pair(color[q] ? 0 : -n, q);
		if (!intersections[q].empty())
			cand = max(cand, intersections[q].rbegin()->first);
		if (cand == value)
			break;
		set_element(offset, tree_sizes[chain], chpos, cand);

		int head = chains[chain].back();
		q = parent[head];
		if (q < 0)
			break;

		pair<int, int> old_source = head_optima[head];
		chpos = chain_position[head];
		pos = offset + chpos;

		pair<int, int> max_val = offset_prefix_max(0, offset, chpos, pos);
		if (max_val.first >= 0)
			add_to_first(max_val, 1);
		head_optima[head] = max_val;
		intersections[q].erase(make_pair(old_source, head));
		intersections[q].insert(make_pair(max_val, head));

		chain = chain_index[q];
		offset = tree_offsets[chain];
		chpos = chain_position[q];
		pos = offset + chpos;
	}
}

pair<int, int> query(int n, int chain, int offset, int tree_size, int chpos)
{
	int head = chains[chain].back();
	int pos = offset + chpos;
	pair<int, int> dr = offset_prefix_max(0, offset, chpos, pos);
	pair<int, int> ur = offset_interval_max(1, offset, chpos, tree_size - 1, pos);
	pair<int, int> result = max(dr, ur);
	int dist = tree_size - chpos;
	for (int p = parent[head]; p >= 0; p = parent[head]) {
		int prev_head = head;
		chain = chain_index[p];
		head = chains[chain].back();
		offset = tree_offsets[chain];
		chpos = chain_position[p];
		pos = offset + chpos;
		int htree_size = tree_sizes[chain];
		if (chpos + 1 <= tree_sizes[chain] - 1) {
			pair<int, int> cand = offset_interval_max(1, offset, chpos + 1, htree_size - 1, pos);
			if (cand.first >= 0) {
				add_to_first(cand, dist);
				if (cand > result)
					result = cand;
			}
		}
		if (0 <= chpos - 1) {
			pair<int, int> cand = offset_prefix_max(0, offset, chpos - 1, pos);
			if (cand.first >= 0) {
				add_to_first(cand, dist);
				if (cand > result)
					result = cand;
			}
		}
		pair<int, int> cand = make_pair(color[p] ? 0 : -n, p);
		if (!intersections[p].empty()) {
			set< pair< pair<int, int>, int > >::reverse_iterator it = intersections[p].rbegin();
			if (it->second == prev_head)
				++it;
			if (it != intersections[p].rend())
				cand = max(cand, it->first);
		}
		if (cand.first >= 0) {
			cand.first += dist;
			if (cand > result)
				result = cand;
		}
		dist += tree_sizes[chain] - chpos;
	}
	return result;
}

int solve_problem()
{
	int n, m;

	n = read_unsigned_integer<int>();
	m = read_unsigned_integer<int>();

	num_chains = 0;

	for (int i = 0; i < n; i++)
		neighbours[i].clear();

	parent[0] = -1;
	for (int i = 1; i < n; i++) {
		int p = read_unsigned_integer<int>() - 1;
		neighbours[p].push_back(i);
		parent[i] = p;
	}

	heavy_light(0);

	int tree_offset = 0;
	for (int i = 0; i < num_chains; i++) {
		int tree_size = (int) chains[i].size();
		tree_offsets[i] = tree_offset;
		tree_sizes[i] = tree_size;
		tree_offset += tree_size;
	}

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

	for (int i = 0; i < n; i++) {
		intersections[i].clear();
		head_optima[i] = make_pair(-1, -1);
	}

	for (int i = 0; i < n; i++) {
		int chain = chain_index[i];
		int chpos = chain_position[i];
		int pos = tree_offsets[chain] + chpos;
		tree[0][pos] = chpos & (chpos + 1);
		v[0][pos] = pair<int, int>(-pos - n, i);
		tree[1][pos] = chpos;
		v[1][pos] = pair<int, int>(pos - n, i);
	}

	for (int i = 0; i < n; i++) {
		int chain = chain_index[i];
		int offset = tree_offsets[chain];
		int chpos = chain_position[i];
		int tree_size = tree_sizes[chain];
		set_element(offset, tree_size, chpos, make_pair(0, i));
		color[i] = true;
	}

	for (size_t i = 0; i < sorted_chains.size(); i++) {
		int chain = sorted_chains[i];
		int offset = tree_offsets[chain];
		int tree_size = tree_sizes[chain];
		int head = chains[chain].back();
		int chpos = chain_position[head];
		int pos = offset + chpos;

		initial_update_value(chains[chain], offset, tree_size);
		pair<int, int> max_val = offset_prefix_max(0, offset, chpos, pos);
		int p = parent[head];
		if (p >= 0) {
			add_to_first(max_val, 1);
			head_optima[head] = max_val;
			intersections[p].insert(make_pair(max_val, head));
		}
	}

	for (int i = 0; i < m; i++) {
		int q = read_unsigned_integer<int>() - 1;
		int former_color = color[q];
		int chain = chain_index[q];
		int offset = tree_offsets[chain];
		int chpos = chain_position[q];
		int pos = offset + chpos;
		if (!former_color) {
			color[q] = true;
			update_value(q, n, chain, offset, chpos, pos);
		}
		int tree_size = tree_sizes[chain];
		pair<int, int> result = query(n, chain, offset, tree_size, chpos);
		printf("%d\n", result.second + 1);
		if (former_color) {
			color[q] = false;
			update_value(q, n, chain, offset, chpos, pos);
		}
	}

	for (int i = 0; i < num_chains; i++)
		chains[i].clear();

	return 0;
}

int main()
{
	int num_tests;

	num_tests = read_unsigned_integer<int>();
	for (int i = 0; i < num_tests; i++)
		solve_problem();

	return 0;
}
