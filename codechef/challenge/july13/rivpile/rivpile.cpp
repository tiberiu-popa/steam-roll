#include <algorithm>
#include <functional>
#include <limits>
#include <set>
#include <vector>
#include <cmath>
#include <cstdio>

#define MAX_N 250
#define MAX_M 250

using namespace std;

int x[MAX_N];
int y[MAX_N];
pair<int, int> disks[MAX_M];
vector< pair< int, pair<int, int> > > global_heap;
int dist[MAX_N][MAX_N];

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

template<typename T>
int get_lower_exp(T n)
{
	int e = 0;
	T k = (T) 1;
	while ((k << 1) <= n) {
		k <<= 1;
		++e;
	}
	return e;
}

template<typename T>
T get_lower_exp2(T n)
{
	T k = (T) 1;
	while ((k << 1) < n)
		k <<= 1;
	return k;
}

void build_lexp2(int *lexp2)
{
	for (int i = 1; i <= MAX_M; i++)
		lexp2[i] = get_lower_exp2(i);
}

long long sqr(long long x)
{
	return x * x;
}

long long sqdist(int i, int j)
{
	return sqr(x[i] - x[j]) + sqr(y[i] - y[j]);
}

template<typename T, typename Compare>
void make_heap_vector(vector<T> &v, Compare comp)
{
	make_heap(v.begin(), v.end(), comp);
}

template<typename T, typename Compare>
void push_heap_vector(vector<T> &v, T elem, Compare comp)
{
	v.push_back(elem);
	push_heap(v.begin(), v.end(), comp);
}

template<typename T, typename Compare>
T pop_heap_vector(vector<T> &v, Compare comp)
{
	pop_heap(v.begin(), v.end(), comp);
	T elem = v.back();
	v.pop_back();
	return elem;
}

long long ll_usqrt(long long n)
{
	int e = get_lower_exp(n);
	long long left = 1LL, right = 1LL << (e / 2 + 1);
	while (left < right) {
		long long mid = left + (right - left) / 2;
		long long sqm = mid * mid;
		if (sqm == n)
			return mid;
		else if (sqm < n)
			left = mid + 1;
		else
			right = mid;
	}
	return left;
}

int solve_problem()
{
	int sup_limit[MAX_N];
	int lexp2[MAX_M + 1];
	int n, m, w;

	lexp2[0] = 0;
	build_lexp2(lexp2);

	n = read_unsigned_integer<int>();
	m = read_unsigned_integer<int>();
	w = read_unsigned_integer<int>();

	for (int i = 0; i < n; i++) {
		x[i] = read_unsigned_integer<int>();
		y[i] = read_unsigned_integer<int>();
	}

	for (int i = 0; i < m; i++) {
		disks[i].first = read_unsigned_integer<int>();
		disks[i].second = read_unsigned_integer<int>();
	}
	sort(disks, disks + m);

	global_heap.clear();

	int result = numeric_limits<int>::max();

	for (int i = 0; i < n; i++) {
		int j = 0;
		for (; j < m && disks[j].first < y[i]; j++) { }
		sup_limit[i] = j;
		for (int k = j; k < m; k++)
			push_heap_vector(global_heap, make_pair(disks[k].second, make_pair(i, k)), greater< pair<int, pair<int, int> > >());
	}

	for (int i = 0; i < n; i++)
		for (int j = i + 1; j < n; j++)
			dist[i][j] = dist[j][i] = ll_usqrt(sqdist(i, j));

	while (!global_heap.empty()) {
		pair< int, pair<int, int> > q = pop_heap_vector(global_heap, greater< pair< int, pair<int, int> > >());
		int tree_min = q.first;
		int row = q.second.first;
		int col = q.second.second;
		if (disks[col].first + y[row] >= w) {
			result = tree_min;
			break;
		}
		for (int i = 0; i < n; i++)
			if (i != row) {
				int min_dist = dist[i][row] - disks[col].first;
				int sup = sup_limit[i];
				int j = 0;
				if (sup < 10) {
					for (; j < sup && disks[j].first < min_dist; j++) { }
				} else {
					for (int step = lexp2[sup]; step >= 1; step >>= 1)
						if (j + step < sup) {
							if (disks[j + step].first < min_dist)
								j += step;
						}
					if (disks[j].first < min_dist)
						++j;
				}

				if (j < sup) {
					int left = j;
					int right = sup_limit[i] - 1;
					sup_limit[i] = j;
					for (int k = left; k <= right; k++) {
						push_heap_vector(global_heap, make_pair(tree_min + disks[k].second, make_pair(i, k)), greater< pair< int, pair<int,int> > >());
					}
				}
			}
	}

	if (result == numeric_limits<int>::max())
		printf("impossible\n");
	else
		printf("%d\n", result);

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
