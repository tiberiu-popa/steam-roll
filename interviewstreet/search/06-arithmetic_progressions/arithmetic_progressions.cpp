#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>
#include <cstring>

#define MOD_PRIME 1000003

using namespace std;

// Simple bit manipulations
int get_msb(int n);
int get_upper_exp(int n);

// Modulo operations
int mulmod(int x, int y, int mod);
int powmod(int x, int n, int mod);
int invmod(int x, int mod);

vector<int> partial_prods, partial_invs;

struct info {

	long long value;
	int product;

	info() : value(0), product(1) { }

	info(long long value, int product) : value(value), product(product) { }

};

struct node {

	info nfo;
	long long extra;

	node() : extra(0) { }

};

void print_node_vec(const vector<node> &vec);

void update_elem(vector<node> &tree, node &elem, int start, int end,
	int left_index, int right_index, long long extra)
{
	elem.nfo.value += extra * (end - start + 1);
	int product = partial_prods[end + 1];
	product = mulmod(product, partial_invs[start], MOD_PRIME);
	product = powmod(product, (int) extra, MOD_PRIME);
	elem.nfo.product = mulmod(elem.nfo.product, product, MOD_PRIME);
	if (start < end) {
		tree[left_index].extra += extra;
		tree[right_index].extra += extra;
	}
}

void propagate_flips(vector<node> &tree, node &elem, int start, int end,
	int left_index, int right_index)
{
	long long extra = elem.extra;
	if (extra > 0) {
		update_elem(tree, elem, start, end, left_index, right_index, extra);
		elem.extra = 0;
	}
}

info update_tree(vector<node> &tree, int index, int start, int end,
	int left, int right, long long extra)
{
	node &elem = tree[index];
	int left_index = (index << 1) + 1;
	int right_index = left_index + 1;

	// Propagate first, only after that return
	propagate_flips(tree, elem, start, end, left_index, right_index);

	if (right < start || end < left)
		return elem.nfo;

	if (left <= start && end <= right) {
		update_elem(tree, elem, start, end, left_index, right_index, extra);
	} else {
		int mid = start + ((end - start) >> 1);
		info left_nfo = update_tree(tree, left_index, start, mid,
			left, right, extra);
		info right_nfo = update_tree(tree, right_index, mid + 1, end,
			left, right, extra);
		elem.nfo.value = left_nfo.value + right_nfo.value;
		elem.nfo.product = mulmod(left_nfo.product, right_nfo.product, MOD_PRIME);
	}
	return elem.nfo;
}

info query_tree(vector<node> &tree, int index, int start, int end,
	int left, int right)
{
	if (right < start || end < left)
		return info();

	int left_index = (index << 1) + 1;
	int right_index = left_index + 1;
	node &elem = tree[index];

	propagate_flips(tree, elem, start, end, left_index, right_index);

	if (left <= start && end <= right)
		return elem.nfo;

	int mid = start + ((end - start) >> 1);
	info left_nfo = query_tree(tree, left_index, start, mid,
		left, right);
	info right_nfo = query_tree(tree, right_index, mid + 1, end,
		left, right);
	int prod = mulmod(left_nfo.product, right_nfo.product, MOD_PRIME);
	info result(left_nfo.value + right_nfo.value, prod);
	return result;
}

void solve_problem()
{
	int n, q;
	vector<int> common_diffs;
	vector<int> powers;

	cin >> n;
	for (int i = 0; i < n; i++) {
		int a, d, p;
		cin >> a >> d >> p;
		common_diffs.push_back(d);
		powers.push_back(p);
	}

	const int num_bits = get_upper_exp(n);
	size_t tree_size = (1 << (num_bits + 1)) - 1;
	vector<node> tree(tree_size);

	size_t offset = (1 << num_bits) - 1;
	for (size_t i = 0; i < powers.size(); i++) {
		size_t j = i + offset;
		tree[j].nfo.value = powers[i];
		tree[j].nfo.product = powmod(common_diffs[i], powers[i], MOD_PRIME);
	}

	for (int k = num_bits - 1; k >= 0; k--) {
		offset = (1 << k) - 1;
		for (size_t i = 0; i <= offset; i++) {
			size_t index = offset + i;
			size_t left_index = 2 * index + 1;
			size_t right_index = left_index + 1;
			info &parent = tree[index].nfo;
			info &left = tree[left_index].nfo;
			info &right = tree[right_index].nfo;
			parent.value = left.value + right.value;
			parent.product = mulmod(left.product, right.product, MOD_PRIME);
		}
	}

	//print_node_vec(tree);

	vector<int> factorials;
	factorials.push_back(0);
	factorials.push_back(1);
	for (int i = 2; i < MOD_PRIME; i++) {
		int fact = mulmod(factorials[i - 1], i, MOD_PRIME);
		factorials.push_back(fact);
	}

	partial_prods.push_back(1);
	partial_invs.push_back(1);
	for (int i = 0; i < n; i++) {
		int prod = mulmod(partial_prods[i], common_diffs[i], MOD_PRIME);
		partial_prods.push_back(prod);
		partial_invs.push_back(invmod(prod, MOD_PRIME));
	}

	int final_right = (1 << num_bits) - 1;
	cin >> q;
	for (int i = 0; i < q; i++) {
		int type, left, right;
		long long extra;
		cin >> type >> left >> right;
		--left;
		--right;
		if (type == 0) {
			info result = query_tree(tree, 0, 0, final_right, left, right);
			int pos = 0;
			if (result.value < MOD_PRIME)
				pos = (int) result.value;
			int v = mulmod(factorials[pos], result.product, MOD_PRIME);
			cout << result.value << ' ' << v << endl;
		} else {
			cin >> extra;
			update_tree(tree, 0, 0, final_right, left, right, extra);
		}
	}

	//print_node_vec(tree);
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

int mulmod(int x, int y, int mod)
{
	long long result = x;
	return (result * y) % mod;
}

int powmod(int x, int n, int mod)
{
	long long result = 1;
	long long base = x;
	while (n > 0) {
		if (n & 1)
			result = (result * base) % mod;
		base = (base * base) % mod;
		n >>= 1;
	}
	return (int) result;
}

int invmod(int x, int mod)
{
	int result[2][2] = { { 1, 0}, { 0, 1 } };
	int base[2][2] = { { 0, 1}, { 1, 0 } };
	int tmp[2][2];

	int y = x % mod;
	x = mod;
	while (y > 0) {
		int q = x / y;
		int r = x % y;
		base[1][1] = -q;
		memset(tmp, 0, sizeof(tmp));
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 2; j++) {
				tmp[i][j] = 0;
				for (int k = 0; k < 2; k++)
					tmp[i][j] += result[i][k] * base[k][j];
			}
		memcpy(result, tmp, sizeof(result));
		x = y;
		y = r;
	}
	int inv = result[1][0];
	if (inv < 0)
		inv += mod;
	return inv;
}

void print_node_vec(const vector<node> &vec)
{
	int i = 0;
	for (vector<node>::const_iterator it = vec.begin(); it != vec.end(); ++it) {
		cout << '[' << it->nfo.value << ' ' << it->nfo.product << ' ' << it->extra << "] ";
		i++;
		if ((i & (i + 1)) == 0)
			cout << endl;
	}
	cout << endl;

	cout << endl;
}
