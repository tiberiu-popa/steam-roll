#include <algorithm>
#include <limits>
#include <map>
#include <set>
#include <vector>
#include <cstdio>

#define MAX_N 100000
#define MAX_VALUE 100000
#define pow2s(x) (((size_t) 1) << (x))

using namespace std;

struct strong_comparator {

	bool operator() (const pair<int, int> &lhs, const pair<int, int> &rhs) const
	{
		return lhs.first < rhs.first && lhs.second < rhs.second;
	}

};

struct sum_comparator {

	bool operator() (const pair<int, int> &lhs, const pair<int, int> &rhs) const
	{
		int s = lhs.first + lhs.second;
		int t = rhs.first + rhs.second;
		if (s != t)
			return s < t;
		else
			return lhs.first < rhs.first;
	}

};

struct downright_comparator {

	bool operator() (const pair<int, int> &lhs, const pair<int, int> &rhs) const
	{
		return make_pair(lhs.second, -lhs.first) < make_pair(rhs.second, -rhs.first);
	}

};

struct set_comparator {

	bool operator() (const pair<int, int> &lhs, const pair<int, int> &rhs) const
	{
		if (lhs.first != rhs.first) {
			return lhs.first < rhs.first;
		} else {
			return lhs.second > rhs.second;
		}
	}

};

set< pair<int, int>, set_comparator > up[MAX_N];

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

template<typename T, typename S>
pair<S, T> invpair(pair<T, S> x)
{
	return make_pair(x.second, x.first);
}

template<typename T>
void rotate_point(pair<T, T> &p)
{
	int tmp = p.second - p.first;
	p.first += p.second;
	p.second = tmp;
}

bool is_inside(const set< pair<int, int>, set_comparator > &up_antichain,
	const pair<int, int> &elem)
{
	set< pair<int, int> >::const_iterator it = up_antichain.lower_bound(elem);
	if (it != up_antichain.begin()) {
		strong_comparator scomp;
		--it;
		if (scomp(*it, elem))
			return true;
	}
	return false;
}

int zigzag(const vector< pair<int, int> > &v)
{
	int k = 0;

	for (size_t i = 0; i < v.size(); i++) {
		set< pair<int, int> >::iterator it;
		pair<int, int> elem = v[i];
		int left = 0, right = k - 1;
		int j = -1;
		while (left < right) {
			int mid = left + ((right - left) >> 1);
			if (is_inside(up[mid], elem)) {
				j = mid;
				left = mid + 1;
			} else {
				right = mid;
			}
		}
		if (left == right) {
			if (is_inside(up[left], elem))
				j = left;
		}
#if 0
		int j;
		for (j = k - 1; j >= 0; j--)
			if (is_inside(up[j], elem))
				break;
#endif
		++j;
		if (j + 1 >= k)
			k = j + 1;
		up[j].insert(elem);
	}

	return k;
}

size_t longest_increasing_subsequence(const vector< pair<int, int> > &v)
{
	vector<size_t> m;
	strong_comparator scomp;

	for (size_t i = 0; i < v.size(); i++) {
		size_t j = numeric_limits<size_t>::max();
		if (!m.empty()) {
			size_t left = 0;
			size_t right = m.size() - 1;
			while (left < right) {
				size_t mid = left + (right - left + 1) / 2;
				if (scomp(v[m[mid]], v[i])) {
					j = mid;
					left = mid + 1;
				} else {
					right = mid - 1;
				}
			}
			if (left < m.size() && scomp(v[m[left]], v[i]))
				j = left;
		}

		if (j == numeric_limits<size_t>::max())
			j = 0;
		else
			++j;

		if (j == m.size() || v[i] < v[m[j]]) {
			if (j == m.size()) {
				m.push_back(i);
			} else {
				m[j] = i;
			}
		}
	}

	return m.size();
}

int solve_problem()
{
	vector< pair<int, int> > v;
	int n;

	n = read_unsigned_integer<int>();

	for (int i = 0; i < n; i++) {
		int x = read_unsigned_integer<int>();
		int y = read_unsigned_integer<int>();
		v.push_back(make_pair(x, y));
	}

	for (size_t i = 0; i < v.size(); i++) {
		size_t j = i;
		while (i + 1 < v.size() && v[i].first == v[i + 1].first)
			++i;
		sort(&v[j], &v[i + 1]);
	}
	trim_unique(v);

	for (size_t i = 0; i < v.size(); i++)
		rotate_point(v[i]);

#if 1
	sort(v.begin(), v.end(), downright_comparator());
	size_t result = longest_increasing_subsequence(v);
#else
	sort(v.begin(), v.end(), sum_comparator());
	size_t result = (size_t) zigzag(v);
#endif

	printf("%zu\n", result);

	return 0;
}

int main()
{
	solve_problem();
	return 0;
}
