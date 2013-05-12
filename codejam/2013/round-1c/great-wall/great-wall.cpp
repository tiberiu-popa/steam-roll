#include <algorithm>
#include <map>
#include <vector>
#include <limits>
#include <cmath>
#include <cstdio>
#include <cstring>

#define MAX_N 1000

int days[MAX_N];
int num_attacks[MAX_N];
int w[MAX_N];
int e[MAX_N];
int strength[MAX_N];
int delta_di[MAX_N];
int delta_pi[MAX_N];
int delta_si[MAX_N];

using namespace std;

struct tree_node {

	tree_node() : elem(numeric_limits<int>::max()), flip(false) { }

	int elem;
	bool flip;

};

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
	parent.elem = min(left.elem, right.elem);
}

void build_interval_tree(vector<tree_node> &tree, size_t num_elems)
{
	const int num_bits = get_upper_exp(num_elems);
	size_t tree_size = pow2(num_bits + 1) - 1;
	tree.resize(tree_size);

	size_t offset = pow2(num_bits) - 1;
	for (size_t i = 0; i < num_elems; i++) {
		int elem = 0;
		tree[offset + i].elem = elem;
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

void flip_node(vector<tree_node> &tree, size_t index, int max_val)
{
	if (max_val > tree[index].elem) {
		tree[index].elem = max_val;
		tree[index].flip = true;
	}
}

void propagate_flips(vector<tree_node> &tree, tree_node &node,
	int left_index, int right_index, bool has_children)
{
	if (node.flip) {
		node.flip = false;
		if (has_children) {
			flip_node(tree, (size_t) left_index, node.elem);
			flip_node(tree, (size_t) right_index, node.elem);
		}
	}
}

tree_node update_tree(vector<tree_node> &tree, int index, int start, int end,
	int left, int right, int max_val)
{
	tree_node &node = tree[(size_t) index];

	int left_index = (index << 1) + 1;
	int right_index = left_index + 1;

	propagate_flips(tree, node, left_index, right_index, start < end);

	if (right < start || end < left)
		return node;

	if (left <= start && end <= right) {
		if (max_val > node.elem) {
			node.elem = max_val;
			if (start < end) {
				flip_node(tree, (size_t) left_index, node.elem);
				flip_node(tree, (size_t) right_index, node.elem);
			}
		}
	} else {
		int mid = start + ((end - start) >> 1);
		tree_node left_node = update_tree(tree, left_index, start, mid,
			left, right, max_val);
		tree_node right_node = update_tree(tree, right_index, mid + 1, end,
			left, right, max_val);
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

tree_node update_full_tree(vector<tree_node> &tree, int left, int right, int max_val)
{
	return update_tree(tree, 0, 0, tree.size() / 2, left, right, max_val);
}

tree_node query_full_tree(vector<tree_node> &tree, int left, int right)
{
	return query_tree(tree, 0, 0, tree.size() / 2, left, right);
}

void trim_unique(vector<int> &v)
{
	vector<int>::iterator it = unique(v.begin(), v.end());
	vector<int>::difference_type diff = it - v.begin();
	v.resize((vector<int>::size_type) diff);
}

int solve_problem(int num_case)
{
	int n;

	if (scanf("%d", &n) != 1)
		return 1;

	for (int i = 0; i < n; i++)
		if (scanf("%d %d %d %d %d %d %d %d", &days[i], &num_attacks[i], &w[i], &e[i], &strength[i], &delta_di[i], &delta_pi[i], &delta_si[i]) != 8)
			return 1;

	vector< pair<int, int> > attacks;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < num_attacks[i]; j++)
			attacks.push_back(make_pair(days[i] + j * delta_di[i], i));
	}
	sort(attacks.begin(), attacks.end());

	vector<int> points;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < num_attacks[i]; j++) {
			points.push_back(w[i] + j * delta_pi[i]);
			points.push_back(e[i] + j * delta_pi[i]);
		}
	}
	sort(points.begin(), points.end());
	trim_unique(points);

	map<int, size_t> reverse_index;
	for (size_t i = 0; i < points.size(); i++)
		reverse_index[points[i]] = i;

	int result = 0;
	vector<tree_node> tree;
	vector< pair<int, pair<int, int> > > q;

	build_interval_tree(tree, 2 * points.size());

	for (size_t i = 0; i < attacks.size(); i++) {
		int d = attacks[i].first;
		int t = attacks[i].second;
		q.push_back(make_pair(strength[t], make_pair(w[t], e[t])));
		if (i + 1 == attacks.size() || attacks[i + 1].first > d) {
			for (size_t j = 0; j < q.size(); j++) {
				int west = 2 * reverse_index[q[j].second.first];
				int east = 2 * reverse_index[q[j].second.second];
				int elem = query_full_tree(tree, west, east).elem;
				if (elem < q[j].first) {
					++result;
				}
			}
			for (size_t j = 0; j < q.size(); j++) {
				int west = 2 * reverse_index[q[j].second.first];
				int east = 2 * reverse_index[q[j].second.second];
				update_full_tree(tree, west, east, q[j].first);
			}
			q.clear();
		}
		w[t] += delta_pi[t];
		e[t] += delta_pi[t];
		strength[t] += delta_si[t];
		if (strength[t] <= 0)
			strength[t] = 0;
	}

	printf("Case #%d: %d\n", num_case, result);

	return 0;
}

int main(int argc, char *argv[])
{
	const char *filenames[] = {
		"great-wall.in",
		"great-wall.out",
	};
	int num_tests;

	for (int i = 1; i < argc && i <= 2; i++)
		filenames[i - 1] = argv[i];

	if (freopen(filenames[0], "rt", stdin) == NULL) {
		fprintf(stderr, "Could not reopen stdin\n");
		return 1;
	}
	if (freopen(filenames[1], "wt", stdout) == NULL) {
		fprintf(stderr, "Could not reopen stdout\n");
		return 1;
	}

	if (scanf("%d", &num_tests) != 1)
		return 1;
	for (int i = 0; i < num_tests; i++)
		solve_problem(i + 1);

	return 0;
}
