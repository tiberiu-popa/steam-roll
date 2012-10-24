#include <iostream>
#include <string>
#include <vector>
#include <cstring>

using namespace std;

#define CARRYLESS_BIT 0
#define CARRYFUL_BIT 1
#define LEAF_A_BIT 2
#define LEAF_B_BIT 3

#define CARRYLESS_MASK (1 << CARRYLESS_BIT)
#define CARRYFUL_MASK (1 << CARRYFUL_BIT)
#define LEAF_A_MASK (1 << LEAF_A_BIT)
#define LEAF_B_MASK (1 << LEAF_B_BIT)

#define get_carryless(mask) (((mask) & CARRYLESS_MASK) != 0)
#define get_carryful(mask) (((mask) & CARRYFUL_MASK) != 0)
#define get_leaf_a(mask) (((mask) & LEAF_A_MASK) != 0)
#define get_leaf_b(mask) (((mask) & LEAF_B_MASK) != 0)

#define set_carryless(mask) ((mask) |= CARRYLESS_MASK)
#define set_carryful(mask) ((mask) |= CARRYFUL_MASK)
#define set_leaf_a(mask) ((mask) |= LEAF_A_MASK)
#define set_leaf_b(mask) ((mask) |= LEAF_B_MASK)

#define clr_carryless(mask) ((mask) &= ~CARRYLESS_MASK)
#define clr_carryful(mask) ((mask) &= ~CARRYFUL_MASK)
#define clr_leaf_a(mask) ((mask) &= ~LEAF_A_MASK)
#define clr_leaf_b(mask) ((mask) &= ~LEAF_B_MASK)

// Simple bit manipulations
int get_msb(int n);
int get_upper_exp(int n);

struct node {

	int mask;

	node() : mask(0) { }

};

void print_node_vec(const vector<node> &vec);

void combine_masks(const int &left_mask, const int &right_mask, int &mask)
{
	if (get_carryless(left_mask)) {
		if (get_carryful(right_mask))
			set_carryless(mask);
		else
			clr_carryless(mask);
	} else {
		if (get_carryless(right_mask))
			set_carryless(mask);
		else
			clr_carryless(mask);
	}
	if (get_carryful(left_mask)) {
		if (get_carryful(right_mask))
			set_carryful(mask);
		else
			clr_carryful(mask);
	} else {
		if (get_carryless(right_mask))
			set_carryful(mask);
		else
			clr_carryful(mask);
	}
}

node update_tree(vector<node> &tree, int index, int start, int end,
	int target, int number, bool value)
{
	node &elem = tree[index];
	int left_index = (index << 1) + 1;
	int right_index = left_index + 1;

	if (target < start || end < target)
		return elem;

	if (target <= start && end <= target) {
		if (number == 0) {
			if (value)
				set_leaf_a(elem.mask);
			else
				clr_leaf_a(elem.mask);
		} else {
			if (value)
				set_leaf_b(elem.mask);
			else
				clr_leaf_b(elem.mask);
		}
		bool a_bit = get_leaf_a(elem.mask);
		bool b_bit = get_leaf_b(elem.mask);
		if (a_bit || b_bit) {
			set_carryful(elem.mask);
			if (a_bit && b_bit) {
				set_carryless(elem.mask);
			} else {
				clr_carryless(elem.mask);
			}
		} else {
			clr_carryless(elem.mask);
			clr_carryful(elem.mask);
		}
	} else {
		int mid = start + ((end - start) >> 1);
		node left_node = update_tree(tree, left_index, start, mid,
			target, number, value);
		node right_node = update_tree(tree, right_index, mid + 1, end,
			target, number, value);
		combine_masks(left_node.mask, right_node.mask, elem.mask);
	}
	return elem;
}

node query_tree(vector<node> &tree, int index, int start, int end,
	int left, int right)
{
	if (right < start || end < left)
		return node();

	int left_index = (index << 1) + 1;
	int right_index = left_index + 1;
	node &elem = tree[index];

	if (left <= start && end <= right)
		return elem;

	int mid = start + ((end - start) >> 1);
	node left_node = query_tree(tree, left_index, start, mid,
		left, right);
	node right_node = query_tree(tree, right_index, mid + 1, end,
		left, right);
	node result;
	if (mid < left)
		result.mask = right_node.mask;
	else if (mid + 1 > right)
		result.mask = left_node.mask;
	else
		combine_masks(left_node.mask, right_node.mask, result.mask);
	return result;
}

void solve_problem()
{
	int n, q;
	string a, b;
	vector<int> common_diffs;
	vector<int> powers;

	cin >> n >> q;
	cin >> a >> b;

	const int num_bits = get_upper_exp(n);
	size_t tree_size = (1 << (num_bits + 1)) - 1;
	vector<node> tree(tree_size);

	size_t str_size = a.size();
	size_t offset = (1 << num_bits) - 1;
	for (size_t i = 0; i < str_size; i++) {
		size_t j = i + offset;
		int a_bit = a[str_size - 1 - i] - '0';
		int b_bit = b[str_size - 1 - i] - '0';
		if (a_bit)
			set_leaf_a(tree[j].mask);
		if (b_bit)
			set_leaf_b(tree[j].mask);
		if (a_bit || b_bit) {
			set_carryful(tree[j].mask);
			if (a_bit && b_bit) {
				set_carryless(tree[j].mask);
			}
		}
	}

	for (int k = num_bits - 1; k >= 0; k--) {
		offset = (1 << k) - 1;
		for (size_t i = 0; i <= offset; i++) {
			size_t index = offset + i;
			size_t left_index = 2 * index + 1;
			size_t right_index = left_index + 1;
			node &parent = tree[index];
			node &left = tree[left_index];
			node &right = tree[right_index];
			combine_masks(left.mask, right.mask, parent.mask);
		}
	}

	int final_right = (1 << num_bits) - 1;
	offset = (1 << num_bits) - 1;
	string type;
	for (int i = 0; i < q; i++) {
		int index, value;
		cin >> type >> index;
		if ("get_c" == type) {
			bool c_bit = false;
			if (index < n) {
				size_t j = index + offset;
				bool a_bit = get_leaf_a(tree[j].mask);
				bool b_bit = get_leaf_b(tree[j].mask);
				c_bit = a_bit ^ b_bit;
			}
			if (index != 0) {
				node elem = query_tree(tree, 0, 0, final_right, 0, index - 1);
				c_bit ^= get_carryless(elem.mask);
			}
			cout << c_bit;
		} else {
			cin >> value;
			int number = ("set_a" == type) ? 0 : 1;
			update_tree(tree, 0, 0, final_right, index, number, value != 0);
			//print_node_vec(tree);
		}
	}
}

int main()
{
	solve_problem();
	return 0;
}

int get_msb(int n)
{
	int k = 0;
	for (int i = n; i > 0; i >>= 1)
		k++;
	return k;
}

int get_upper_exp(int n)
{
	int k = get_msb(n);
	int result = 1 << (k - 1);
	if (result == n)
		return k - 1;
	else
		return k;
}

void print_node_vec(const vector<node> &vec)
{
	int i = 0;
	for (vector<node>::const_iterator it = vec.begin(); it != vec.end(); ++it) {
		int mask = it->mask;
		cout << '[' << get_carryless(mask) << ' ' << get_carryful(mask)
			<< " : " << get_leaf_a(mask) << ' ' << get_leaf_b(mask) << "] ";
		i++;
		if ((i & (i + 1)) == 0)
			cout << endl;
	}
	cout << endl;

	cout << endl;
}
