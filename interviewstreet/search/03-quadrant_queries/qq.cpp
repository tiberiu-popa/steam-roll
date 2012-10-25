#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct quad_count {

	int num_pp, num_pn;
	int num_np, num_nn;

	quad_count() : num_pp(0), num_pn(0), num_np(0), num_nn(0) { }

	quad_count& operator+= (const quad_count &rhs)
	{
		num_pp += rhs.num_pp;
		num_pn += rhs.num_pn;
		num_np += rhs.num_np;
		num_nn += rhs.num_nn;
		return *this;
	}

	quad_count operator+ (const quad_count &rhs)
	{
		quad_count result(*this);
		return (result += rhs);
	}

	friend ostream& operator<< (ostream& out, const quad_count &qc);

};

struct quad {

	quad_count qc;
	int flip_x, flip_y;

	quad() : flip_x(0), flip_y(0) { }

};

ostream& operator<< (ostream& out, const quad_count &qc)
{
	out << qc.num_pp << ' ' << qc.num_np << ' '
		<< qc.num_nn << ' ' << qc.num_pn;
	return out;
}

void print_quad_qcvec(vector<quad> &vec);
void print_quad_flipvec(vector<quad> &vec);

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

void propagate_flips(vector<quad> &tree, quad &node,
	int left_index, int right_index, bool has_children)
{
	if (node.flip_x != 0) {
		swap(node.qc.num_pp, node.qc.num_pn);
		swap(node.qc.num_np, node.qc.num_nn);
		node.flip_x = 0;
		if (has_children) {
			tree[left_index].flip_x ^= 1;
			tree[right_index].flip_x ^= 1;
		}
	}
	if (node.flip_y != 0) {
		swap(node.qc.num_pp, node.qc.num_np);
		swap(node.qc.num_pn, node.qc.num_nn);
		node.flip_y = 0;
		if (has_children) {
			tree[left_index].flip_y ^= 1;
			tree[right_index].flip_y ^= 1;
		}
	}
}

quad_count update_tree(vector<quad> &tree, int index, int start, int end,
	int left, int right, bool axis)
{
	quad_count qc;

	if (right < start || end < left)
		return qc;

	int left_index = (index << 1) + 1;
	int right_index = left_index + 1;
	quad &node = tree[index];

	propagate_flips(tree, node, left_index, right_index, start < end);

	if (left <= start && end <= right) {
		if (axis) {
			qc.num_pp = node.qc.num_pn - node.qc.num_pp;
			qc.num_pn = node.qc.num_pp - node.qc.num_pn;
			qc.num_np = node.qc.num_nn - node.qc.num_np;
			qc.num_nn = node.qc.num_np - node.qc.num_nn;
		} else {
			qc.num_pp = node.qc.num_np - node.qc.num_pp;
			qc.num_pn = node.qc.num_nn - node.qc.num_pn;
			qc.num_np = node.qc.num_pp - node.qc.num_np;
			qc.num_nn = node.qc.num_pn - node.qc.num_nn;
		}
		node.qc += qc;
		if (start < end) {
			if (axis) {
				tree[left_index].flip_x ^= 1;
				tree[right_index].flip_x ^= 1;
			} else {
				tree[left_index].flip_y ^= 1;
				tree[right_index].flip_y ^= 1;
			}
		}
		return qc;
	} else {
		int mid = start + ((end - start) >> 1);
		quad_count left_qc = update_tree(tree, left_index, start, mid,
			left, right, axis);
		quad_count right_qc = update_tree(tree, right_index, mid + 1, end,
			left, right, axis);
		quad_count sum_qc(left_qc + right_qc);
		node.qc += sum_qc;
		return sum_qc;
	}
}

quad_count query_tree(vector<quad> &tree, int index, int start, int end,
	int left, int right)
{
	quad_count qc;

	if (right < start || end < left)
		return qc;

	int left_index = (index << 1) + 1;
	int right_index = left_index + 1;
	quad &node = tree[index];

	propagate_flips(tree, node, left_index, right_index, start < end);

	if (left <= start && end <= right)
		return node.qc;

	int mid = start + ((end - start) >> 1);
	quad_count left_qc = query_tree(tree, left_index, start, mid,
		left, right);
	quad_count right_qc = query_tree(tree, right_index, mid + 1, end,
		left, right);
	return left_qc + right_qc;
}

void solve_problem()
{
	vector<int> x;
	vector<int> y;
	int n;
	int q;

	cin >> n;
	for (int i = 0; i < n; i++) {
		int px, py;
		cin >> px >> py;
		x.push_back(px);
		y.push_back(py);
	}

	const int num_bits = get_upper_exp(n);
	size_t tree_size = (1 << (num_bits + 1)) - 1;
	vector<quad> tree(tree_size);

	size_t offset = (1 << num_bits) - 1;
	size_t limit = (size_t) n;
	for (size_t i = 0; i < limit; i++)
		if (x[i] > 0) {
			if (y[i] > 0)
				tree[offset + i].qc.num_pp++;
			else
				tree[offset + i].qc.num_pn++;
		} else {
			if (y[i] > 0)
				tree[offset + i].qc.num_np++;
			else
				tree[offset + i].qc.num_nn++;
		}

	for (int k = num_bits - 1; k >= 0; k--) {
		offset = (1 << k) - 1;
		for (size_t i = 0; i <= offset; i++) {
			size_t index = offset + i;
			size_t left_index = 2 * index + 1;
			size_t right_index = left_index + 1;
			struct quad &parent = tree[index];
			struct quad &left = tree[left_index];
			struct quad &right = tree[right_index];
			parent.qc.num_pp = left.qc.num_pp + right.qc.num_pp;
			parent.qc.num_pn = left.qc.num_pn + right.qc.num_pn;
			parent.qc.num_np = left.qc.num_np + right.qc.num_np;
			parent.qc.num_nn = left.qc.num_nn + right.qc.num_nn;
		}
	}

	//print_quad_qcvec(tree);
	//print_quad_flipvec(tree);

	int final_right = (1 << num_bits) - 1;
	cin >> q;
	for (int i = 0; i < q; i++) {
		char type;
		int left, right;
		cin >> type >> left >> right;
		--left;
		--right;
		if (type == 'C') {
			quad_count qc = query_tree(tree, 0, 0, final_right, left, right);
			cout << qc << endl;
		} else {
			bool axis = (type == 'X');
			update_tree(tree, 0, 0, final_right, left, right, axis);
		}
	}

	//print_quad_qcvec(tree);
	//print_quad_flipvec(tree);
}

int main()
{
	solve_problem();
	return 0;
}

void print_quad_qcvec(vector<quad> &vec)
{
	for (vector<quad>::iterator it = vec.begin(); it != vec.end(); ++it)
		cout << '[' << it->qc << "] ";
	cout << endl;

	cout << endl;
}

void print_quad_flipvec(vector<quad> &vec)
{
	for (vector<quad>::iterator it = vec.begin(); it != vec.end(); ++it)
		cout << it->flip_x << ' ';
	cout << endl;

	for (vector<quad>::iterator it = vec.begin(); it != vec.end(); ++it)
		cout << it->flip_y << ' ';
	cout << endl;

	cout << endl;
}
