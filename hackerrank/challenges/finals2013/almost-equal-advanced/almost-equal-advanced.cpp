#include <algorithm>
#include <iostream>
#include <vector>
#include <cstdio>

#define MAX_N 100000
#define MAX_Q 100000
#define NUM_BUCKETS 316

using namespace std;

struct reverse_entry {

	int pos, left, right;

	reverse_entry() : pos(-1), left(0), right(0) { }

	reverse_entry(int pos, int left, int right) : pos(pos), left(left), right(right) { }

};

struct query_entry {

	int left_bucket, right, idx, left;

	query_entry() : left_bucket(0), right(0), idx(-1), left(0) { }

	query_entry(int left_bucket, int right, int idx, int left)
		: left_bucket(left_bucket), right(right), idx(idx), left(left) { }

	bool operator< (const query_entry &rhs) const
	{
		if (left_bucket != rhs.left_bucket)
			return left_bucket < rhs.left_bucket;
		if (right != rhs.right)
			return right < rhs.right;
		if (idx != rhs.idx)
			return idx < rhs.idx;
		return left < rhs.left;
	}

};

int h[MAX_N];
int v[MAX_N];
long long qres[MAX_Q];
long long tree[MAX_N + 2];
reverse_entry re[MAX_N];
query_entry qe[MAX_Q];
vector<int> values;

#ifdef _WIN32
int getchar_unlocked()
{
	return getchar();
}
#endif

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

char read_character()
{
	return skip_whitespace();
}

template<typename T>
int get_msb(T n)
{
	int k = 0;
	for (; n > 0; n >>= 1)
		k++;
	return k;
}

template<typename T>
int get_lower_exp(T n)
{
	return get_msb(n >> 1);
}

template<typename T>
int get_upper_exp(T n)
{
	return get_msb(n - 1);
}

template<typename T>
T i_lsqrt(T n)
{
	int e = get_lower_exp(n);
	T left = 1, right = ((T) 1) << (e / 2 + 1);
	while (left < right) {
		T mid = left + (right - left + 1) / 2;
		T sqm = mid * mid;
		if (sqm == n)
			return mid;
		else if (sqm < n)
			left = mid;
		else
			right = mid - 1;
	}
	return left;
}

template<class T>
void trim_unique(vector<T> &v)
{
	typename vector<T>::iterator it = unique(v.begin(), v.end());
	typename vector<T>::difference_type diff = it - v.begin();
	v.resize((typename vector<T>::size_type) diff);
}

template<typename T>
T bit_get_interval(const T *tree, const int *v, int l, int r)
{
	T sum = 0;

	while (l <= r) {
		int idx = (r & (r + 1)) - 1;
		if (idx + 1 >= l) {
			sum += tree[r];
		} else {
			sum += v[r];
			idx = r - 1;
		}
		r = idx;
	}

	return sum;
}

template<typename T>
void bit_update_element(T *tree, int *v, int tree_size, int idx, T value)
{
	v[idx] += value;
	while (idx < tree_size) {
		tree[idx] += value;
		idx |= (idx + 1);
	}
}

int count_pairs(int left, int right, int k)
{
	int cnt = 0;
	for (int i = left; i < right; i++)
		for (int j = i + 1; j < right; j++)
			if (abs(v[i] - v[j]) <= k)
				++cnt;
	return cnt;
}

template<typename T>
void increment(T *tree, int *v, T &result, int tree_size, int idx)
{
	result += bit_get_interval(tree, v, re[idx].left, re[idx].right);
	bit_update_element(tree, v, tree_size, re[idx].pos, 1LL);
}

template<typename T>
void decrement(T *tree, int *v, T &result, int tree_size, int idx)
{
	bit_update_element(tree, v, tree_size, re[idx].pos, -1LL);
	result -= bit_get_interval(tree, v, re[idx].left, re[idx].right);
}

int solve_problem()
{
	int n, k, q;

	n = read_unsigned_integer<int>();
	k = read_unsigned_integer<int>();

	for (int i = 0; i < n; i++) {
		h[i] = read_signed_integer<int>();
		values.push_back(h[i]);
	}
	values.push_back(-(k + 1));
	int max_elem = *max_element(values.begin(), values.end());
	values.push_back(max_elem + (k + 1));

	sort(values.begin(), values.end());
	trim_unique(values);

	for (int i = 0; i < n; i++) {
		vector<int>::iterator p_it = lower_bound(values.begin(), values.end(), h[i]);
		vector<int>::iterator l_it = lower_bound(values.begin(), p_it + 1, h[i] - k);
		vector<int>::iterator r_it = upper_bound(p_it, values.end(), h[i] + k);
		re[i] = reverse_entry(p_it - values.begin(), l_it - values.begin(), r_it - values.begin() - 1);
	}

	int m = i_lsqrt(n);
	int bucket_size = n / m;
	int tree_size = (int) values.size();

	q = read_unsigned_integer<int>();
	for (int i = 0; i < q; i++) {
		int left = read_unsigned_integer<int>();
		int right = read_unsigned_integer<int>();
		int lb = left / bucket_size;
		qe[i] = query_entry(lb, right, i, left);
	}
	sort(qe, qe + q);

	long long result = 0;
	int left = qe[0].left, right = left - 1;
	for (int i = 0; i < q; i++) {
		for (; left < qe[i].left; left++)
			decrement(tree, v, result, tree_size, left);
		while (left > qe[i].left)
			increment(tree, v, result, tree_size, --left);
		while (right < qe[i].right)
			increment(tree, v, result, tree_size, ++right);
		for (; right > qe[i].right; right--)
			decrement(tree, v, result, tree_size, right);
		qres[qe[i].idx] = result;
	}

	for (int i = 0; i < q; i++)
		cout << qres[i] << endl;

	return 0;
}

int main()
{
	solve_problem();
	return 0;
}
