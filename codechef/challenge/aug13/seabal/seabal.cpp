#include <algorithm>
#include <map>
#include <set>
#include <vector>
#include <cstdio>
#include <cstring>

#define MAX_N 100000
#define MAX_M 100000

using namespace std;

struct pair_comparator {

	bool operator() (const pair<int, int> &lhs, const pair<int, int> &rhs)
	{
		return make_pair(lhs.second, lhs.first) < make_pair(rhs.second, rhs.first);
	}

};

struct tree_node {
	vector< pair<int, int> > points;
};

int a[MAX_N];
int l[MAX_M];
int r[MAX_M];
vector<tree_node> tree;

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
	parent.points.resize(left.points.size() + right.points.size());
	merge(left.points.begin(), left.points.end(), right.points.begin(), right.points.end(), parent.points.begin(), pair_comparator());
}

void build_interval_tree(vector<tree_node> &tree, int m,
	const map<int, size_t> &xind)
{
	const int num_bits = get_upper_exp(xind.size());
	size_t tree_size = pow2(num_bits + 1) - 1;
	tree.resize(tree_size);

	size_t offset = pow2(num_bits) - 1;
	for (int j = 0; j < m; j++) {
		size_t i = xind.find(l[j])->second;
		tree[offset + i].points.push_back(make_pair(l[j], r[j]));
	}

	for (size_t i = 0; i < xind.size(); i++)
		sort(tree[offset + i].points.begin(), tree[offset + i].points.end());

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

ptrdiff_t count_between(const vector< pair<int, int> > &v, int lower, int upper)
{
	pair_comparator pc;
	ptrdiff_t tx = lower_bound(v.begin(), v.end(), make_pair(-1, lower), pc) - v.begin();
	ptrdiff_t ty = upper_bound(v.begin(), v.end(), make_pair(-1, upper + 1), pc) - v.begin();
	return ty - tx;
}

ptrdiff_t query_tree(vector<tree_node> &tree, int index, int start, int end,
	int left, int right, int lower, int upper)
{
	if (right < start || end < left)
		return 0;

	int left_index = (index << 1) + 1;
	int right_index = left_index + 1;
	tree_node &parent_node = tree[(size_t) index];

	if (left <= start && end <= right)
		return count_between(parent_node.points, lower, upper);

	int mid = start + ((end - start) >> 1);
	ptrdiff_t left_count = query_tree(tree, left_index, start, mid,
		left, right, lower, upper);
	ptrdiff_t right_count = query_tree(tree, right_index, mid + 1, end,
		left, right, lower, upper);
	return left_count + right_count;
}

ptrdiff_t query_full_tree(vector<tree_node> &tree, int left, int right, int lower, int upper)
{
	return query_tree(tree, 0, 0, tree.size() / 2, left, right, lower, upper);
}

int solve_problem()
{
	int n, m, k;

	n = read_unsigned_integer<int>();
	m = read_unsigned_integer<int>();

	for (int i = 0; i < n; i++)
		a[i] = read_unsigned_integer<int>();

	for (int i = 0; i < m; i++) {
		l[i] = read_unsigned_integer<int>() - 1;
		r[i] = read_unsigned_integer<int>() - 1;
	}

	vector<int> vx;
	map<int, size_t> xind;

	for (int i = 0; i < m; i++)
		vx.push_back(l[i]);

	sort(vx.begin(), vx.end());
	trim_unique(vx);

	for (size_t i = 0; i < vx.size(); i++)
		xind[vx[i]] = i;

	build_interval_tree(tree, m, xind);

	set< pair<int, int> > intervals;
	int result = 0;

	k = read_unsigned_integer<int>();
	for (int i = 0; i < k; i++) {
		int q = result + read_signed_integer<int>() - 1;
		if (a[q] > 0 && --a[q] == 0) {
			int l = q, r = q;
			set< pair<int, int> >::iterator it = intervals.upper_bound(make_pair(q, -1));
			set< pair<int, int> >::iterator u_it = it;
			bool del_upper = false;
			if (it != intervals.end() && it->first == q + 1) {
				r = it->second;
				del_upper = true;
			}
			if (it != intervals.begin()) {
				--it;
				if (it->second + 1 == q) {
					l = it->first;
					intervals.erase(it);
				}
			}
			if (del_upper)
				intervals.erase(u_it);
			intervals.insert(make_pair(l, r));

			int tx = lower_bound(vx.begin(), vx.end(), l) - vx.begin();
			int ty = upper_bound(vx.begin(), vx.end(), q) - vx.begin();
			if (tx < ty)
				result += (int) query_full_tree(tree, tx, ty - 1, q, r);
		}

		printf("%d\n", result);
	}

	return 0;
}

int main()
{
	solve_problem();
	return 0;
}
