#include <algorithm>
#include <map>
#include <set>
#include <vector>
#include <cstdio>

#define MAX_M 200000

using namespace std;

int values[MAX_M];
int finite_differences[MAX_M];

template<class T>
T get_iterator_value(const set<T> &coll, typename set<T>::const_iterator it, T default_value)
{
	if (it == coll.end())
		return default_value;
	else
		return *it;
}

void solve_problem()
{
	int m;

	if (scanf("%d", &m) != 1)
		return;
	for (int i = 0; i < m; i++) {
		int x;
		if (scanf("%d", &x) != 1)
			return;
		if (m < x)
			x = m;
		values[i] = x;
	}

	sort(values, values + m);

	finite_differences[0] = values[0];
	for (int i = 1; i < m; i++)
		finite_differences[i] = values[i] - values[i - 1];

	set<int> non_zeroes;
	for (int i = 0; i < m; i++)
		if (finite_differences[i] != 0)
			non_zeroes.insert(i);

	int result = 0;
	for (int i = 1; i <= m; i++) {
		int to_remove = -1;

		set<int>::iterator it = non_zeroes.lower_bound(m - i);
		int pos = get_iterator_value(non_zeroes, it, m);
		if (pos < m) {
			if (--finite_differences[pos] == 0)
				to_remove = pos;
		}

		if (pos != m - i) {
			int first_elem = get_iterator_value(non_zeroes, non_zeroes.begin(), m);
			if (pos == first_elem)
				break;
			--it;
			int prev_pos = *it;
			if (--finite_differences[prev_pos] == 0)
				non_zeroes.erase(prev_pos);
			int remaining = pos - (m - i);
			int mid_pos = prev_pos + remaining;
			if (++finite_differences[mid_pos] == 1)
				non_zeroes.insert(mid_pos);
		}
		if (to_remove >= 0)
			non_zeroes.erase(to_remove);

		++result;
	}

	printf("%d\n", result);
}

int main()
{
	if (freopen("lumanari.in", "rt", stdin) == NULL)
		return 1;
	if (freopen("lumanari.out", "wt", stdout) == NULL)
		return 1;

	solve_problem();

	return 0;
}
