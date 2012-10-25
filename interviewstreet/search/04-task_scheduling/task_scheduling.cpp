#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>

using namespace std;

struct task {

	int d, m;
	int pos;

	bool operator< (const task& rhs) const
	{
		if (d < rhs.d) {
			return true;
		} else if (d == rhs.d) {
			if (m < rhs.m)
				return true;
			else if (m == rhs.m)
				return pos < rhs.pos;
		}
		return false;
	}

	friend ostream& operator<< (ostream& out, const task &obj);

};

ostream& operator<< (ostream& out, const task &obj)
{
	out << '(' << obj.d << ' ' << obj.m << ')';
	return out;
}

struct node {

	int value;
	int extra;

	node() : value(0), extra(0) { }

	node(int value) : value(value), extra(0) { }

};

void print_node_vec(const vector<node> &vec);

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

void propagate_flips(vector<node> &tree, node &elem,
	int left_index, int right_index, bool has_children)
{
	int extra = elem.extra;
	if (extra > 0) {
		elem.value += extra;
		if (has_children) {
			tree[left_index].extra += extra;
			tree[right_index].extra += extra;
		}
		elem.extra = 0;
	}
}

int update_tree(vector<node> &tree, int index, int start, int end,
	int left, int right, int extra)
{
	node &elem = tree[index];
	int left_index = (index << 1) + 1;
	int right_index = left_index + 1;

	// Propagate first, only after that return
	propagate_flips(tree, elem, left_index, right_index, start < end);

	if (right < start || end < left)
		return elem.value;

	if (left <= start && end <= right) {
		elem.value += extra;
		if (start < end) {
			tree[left_index].extra += extra;
			tree[right_index].extra += extra;
		}
	} else {
		int mid = start + ((end - start) >> 1);
		int left_max = update_tree(tree, left_index, start, mid,
			left, right, extra);
		int right_max = update_tree(tree, right_index, mid + 1, end,
			left, right, extra);
		elem.value = max(left_max, right_max);
	}
	return elem.value;
}

void solve_problem()
{
	int t;
	vector<task> tasks;

	cin >> t;
	for (int i = 0; i < t; i++) {
		task elem;
		cin >> elem.d >> elem.m;
		elem.pos = i;
		tasks.push_back(elem);
	}

	vector<task> ordered_tasks(tasks);
	sort(ordered_tasks.begin(), ordered_tasks.end());
	for (size_t i = 0; i < ordered_tasks.size(); i++)
		tasks[ordered_tasks[i].pos].pos = i;
	ordered_tasks.clear();

	const int num_bits = get_upper_exp(t);
	size_t tree_size = (1 << (num_bits + 1)) - 1;
	vector<node> tree(tree_size);

	int small_number = numeric_limits<int>::min() + 1;
	for (size_t i = 0; i < tree_size; i++)
		tree[i].value = small_number;

	int final_right = (1 << num_bits) - 1;
	for (int i = 0; i < t; i++) {
		task &elem = tasks[i];
		int pos = elem.pos;
		update_tree(tree, 0, 0, final_right, pos, pos, -small_number - elem.d);
		update_tree(tree, 0, 0, final_right, pos, t - 1, elem.m);
		//print_node_vec(tree);
		int answer = max(0, tree[0].value);
		cout << answer << endl;
	}
}

int main()
{
	solve_problem();
	return 0;
}

void print_node_vec(const vector<node> &vec)
{
	int i = 0;
	for (vector<node>::const_iterator it = vec.begin(); it != vec.end(); ++it) {
		cout << '[' << it->value << ' ' << it->extra << "] ";
		i++;
		if ((i & (i + 1)) == 0)
			cout << endl;
	}
	cout << endl;

	cout << endl;
}
