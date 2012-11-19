#include <algorithm>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#define ipow4(n) (1 << ((n) << 1))

#define MAX_N 1000
#define MAX_NUM_BITS 11

using namespace std;

struct rectangle {

	pair<int, int> top_left;
	pair<int, int> bottom_right;

};

struct tree_node {
	short count;
	short extra;
};

vector<int> xc, yc;
int offsets[MAX_NUM_BITS + 2];
rectangle rect[MAX_N];
tree_node counts[(ipow4(MAX_NUM_BITS + 1) - 1) / (4 - 1)];
long long result = 0;

void read_input(const char *filename, int &n, int &k)
{
	FILE *f;

	f = fopen(filename, "rt");
	if (f == NULL) {
		fprintf(stderr, "Could not open file: %s\n", filename);
		exit(EXIT_FAILURE);
	}

	if (fscanf(f, "%d %d", &n, &k) != 2)
		exit(EXIT_FAILURE);
	for (int i = 0; i < n; i++) {
		int x, y, z, t;
		if (fscanf(f, "%d %d %d %d", &x, &y, &z, &t) != 4)
			exit(EXIT_FAILURE);
		if (x == z || y == t) {
			--i;
			--n;
			continue;
		}
		rect[i].top_left = pair<int, int>(min(x, z), min(y, t));
		rect[i].bottom_right = pair<int, int>(max(x, z), max(y, t));
	}
	fclose(f);
}

void write_output(const char *filename, long long result)
{
	FILE *f;

	f = fopen(filename, "wt");
	if (f == NULL) {
		fprintf(stderr, "Could not open file: %s\n", filename);
		exit(EXIT_FAILURE);
	}

	fprintf(f, "%lld\n", result);
	fclose(f);
}

ptrdiff_t get_lower_pos(const vector<int> &v, int value)
{
	vector<int>::const_iterator it = lower_bound(v.begin(), v.end(), value);
	return it - v.begin();
}

void trim_unique(vector<int> &v)
{
	vector<int>::iterator it = unique(v.begin(), v.end());
	vector<int>::difference_type diff = it - v.begin();
	v.resize((vector<int>::size_type) diff);
}

int get_upper_shift(int n)
{
	int k = 0;
	int answer;

	while (n > 0) {
		n >>= 1;
		k++;
	}

	answer = 1 << (k - 1);
	if (n == answer)
		return k - 1;
	else
		return k;
}

void propagate_tree(tree_node &node, int next_index)
{
	short extra = node.extra;
	if (extra > 0) {
		node.extra = 0;
		node.count += extra;
		if (next_index >= 0) {
			for (int i = 0; i < 4; i++)
				counts[next_index + i].extra += extra;
		}
	}
}

void update_tree(int level, int index, int top, int left, int bottom, int right,
	int x, int y, int z, int t)
{
	tree_node &node = counts[index];
	int next_index = -1;
	if (top != bottom || left != right) {
		int pos = index - offsets[level];
		next_index = 4 * pos + offsets[level + 1];
	}

	propagate_tree(node, next_index);

	if (x <= top && bottom <= z && y <= left && right <= t) {
		node.extra++;
		propagate_tree(node, next_index);
		return;
	}

	int mid_vert = (top + bottom) >> 1;
	int mid_hor = (left + right) >> 1;
	if (x <= mid_vert && y <= mid_hor)
		update_tree(level + 1, next_index, top, left, mid_vert, mid_hor, x, y, z, t);
	if (x <= mid_vert && t > mid_hor)
		update_tree(level + 1, next_index + 1, top, mid_hor + 1, mid_vert, right, x, y, z, t);
	if (z > mid_vert && y <= mid_hor)
		update_tree(level + 1, next_index + 2, mid_vert + 1, left, bottom, mid_hor, x, y, z, t);
	if (z > mid_vert && t > mid_hor)
		update_tree(level + 1, next_index + 3, mid_vert + 1, mid_hor + 1, bottom, right, x, y, z, t);
}

int query_tree(int level, int index, int top, int left, int bottom, int right,
	int x, int y, int z, int t)
{
	tree_node &node = counts[index];
	int next_index = -1;
	if (top != bottom || left != right) {
		int pos = index - offsets[level];
		next_index = 4 * pos + offsets[level + 1];
	}

	propagate_tree(node, next_index);

	if (x <= top && bottom <= z && y <= left && right <= t) {
		return node.count;
	}

	int sum = 0;
	int mid_vert = (top + bottom) >> 1;
	int mid_hor = (left + right) >> 1;
	if (x <= mid_vert && y <= mid_hor)
		sum += query_tree(level + 1, next_index, top, left, mid_vert, mid_hor, x, y, z, t);
	if (x <= mid_vert && t > mid_hor)
		sum += query_tree(level + 1, next_index + 1, top, mid_hor + 1, mid_vert, right, x, y, z, t);
	if (z > mid_vert && y <= mid_hor)
		sum += query_tree(level + 1, next_index + 2, mid_vert + 1, left, bottom, mid_hor, x, y, z, t);
	if (z > mid_vert && t > mid_hor)
		sum += query_tree(level + 1, next_index + 3, mid_vert + 1, mid_hor + 1, bottom, right, x, y, z, t);
	return sum;
}

void propagate_all(int level, int index, int top, int left, int bottom, int right, int target)
{
	tree_node &node = counts[index];
	int pos = index - offsets[level];
	int next_index = -1;
	if (top != bottom || left != right) {
		next_index = 4 * pos + offsets[level + 1];
	}

	propagate_tree(node, next_index);

	int mid_vert = (top + bottom) >> 1;
	int mid_hor = (left + right) >> 1;
	if (next_index >= 0) {
		propagate_all(level + 1, next_index, top, left, mid_vert, mid_hor, target);
		propagate_all(level + 1, next_index + 1, top, mid_hor + 1, mid_vert, right, target);
		propagate_all(level + 1, next_index + 2, mid_vert + 1, left, bottom, mid_hor, target);
		propagate_all(level + 1, next_index + 3, mid_vert + 1, mid_hor + 1, bottom, right, target);
	} else {
		if (node.count == target) {
			size_t i = (size_t) top;
			size_t j = (size_t) left;
			long long x_diff = xc[i + 1] - xc[i];
			long long y_diff = yc[j + 1] - yc[j];
			result += x_diff * y_diff;
		}
	}
}

long long solve_problem(int n, int k)
{
	for (int i = 0; i < n; i++) {
		xc.push_back(rect[i].top_left.first);
		yc.push_back(rect[i].top_left.second);
		xc.push_back(rect[i].bottom_right.first);
		yc.push_back(rect[i].bottom_right.second);
	}
	sort(xc.begin(), xc.end());
	trim_unique(xc);
	sort(yc.begin(), yc.end());
	trim_unique(yc);

	int ushift = get_upper_shift(2 * n);
	int uexp = 1 << ushift;
	offsets[0] = 0;
	for (int i = 1; i <= ushift + 1; i++)
		offsets[i] = offsets[i - 1] + ipow4(i - 1);

	for (int i = 0; i < n; i++) {
		ptrdiff_t p = get_lower_pos(xc, rect[i].top_left.first);
		ptrdiff_t q = get_lower_pos(yc, rect[i].top_left.second);
		ptrdiff_t r = get_lower_pos(xc, rect[i].bottom_right.first);
		ptrdiff_t s = get_lower_pos(yc, rect[i].bottom_right.second);
		--r;
		--s;
		update_tree(0, 0, 0, 0, uexp - 1, uexp - 1, (int) p, (int) q, (int) r, (int) s);
	}

	propagate_all(0, 0, 0, 0, uexp - 1, uexp - 1, k);

	return result;
}

int main()
{
	int n, k;
	long long result;

	read_input("dreptunghiuri4.in", n, k);
	result = solve_problem(n, k);
	write_output("dreptunghiuri4.out", result);

	return 0;
}
