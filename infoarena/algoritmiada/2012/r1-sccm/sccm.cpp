#include <algorithm>
#include <set>
#include <vector>
#include <cstdio>
#include <cstdlib>

#define MAX_N 80000

using namespace std;

vector< pair<int, size_t> > x;
vector< pair<int, size_t> > y;
vector< pair<size_t, size_t> > z;
set< pair<size_t, size_t> > up[MAX_N];
set< pair<size_t, size_t> > down[MAX_N];

template<class S, class T>
pair<T, S> reverse_pair(pair<S, T> elem)
{
	return pair<T, S>(elem.second, elem.first);
}

void read_input(const char *filename, int &n, int &m)
{
	FILE *f;
	int k;

	f = fopen(filename, "rt");
	if (f == NULL) {
		fprintf(stderr, "Could not open file: %s\n", filename);
		exit(EXIT_FAILURE);
	}

	if (fscanf(f, "%d %d", &n, &m) != 2)
		exit(EXIT_FAILURE);
	k = min(n, m);
	for (int i = 0; i < n; i++) {
		int elem;
		if (fscanf(f, "%d", &elem) != 1)
			exit(EXIT_FAILURE);
		if (elem <= k)
			x.push_back(pair<int, size_t>(elem, x.size()));
	}
	for (int i = 0; i < m; i++) {
		int elem;
		if (fscanf(f, "%d", &elem) != 1)
			exit(EXIT_FAILURE);
		if (elem <= k)
			y.push_back(pair<int, size_t>(elem, y.size()));
	}
	fclose(f);
}

void write_output(const char *filename, int result)
{
	FILE *f;

	f = fopen(filename, "wt");
	if (f == NULL) {
		fprintf(stderr, "Could not open file: %s\n", filename);
		exit(EXIT_FAILURE);
	}

	fprintf(f, "%d\n", result);
	fclose(f);
}

bool is_inside(const set< pair<size_t, size_t> > &up_antichain,
	const pair<size_t, size_t> &elem)
{
	set< pair<size_t, size_t> >::const_iterator it = up_antichain.lower_bound(elem);
	if (it != up_antichain.begin()) {
		--it;
		if (elem.second > it->second)
			return true;
	}
	return false;
}

int solve_problem(int n, int m)
{
	(void) n;
	(void) m;

	sort(x.begin(), x.end());
	sort(y.begin(), y.end());

	for (size_t i = 0; i < x.size(); i++)
		z.push_back(pair<size_t, size_t>(x[i].second, y[i].second));

	int k = 0;
	for (size_t i = 0; i < z.size(); i++) {
		set< pair<size_t, size_t> >::iterator it;
		pair<size_t, size_t> elem = z[i];
		pair<size_t, size_t> rev_elem = reverse_pair(elem);
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
		it = down[j].upper_bound(rev_elem);
		set< pair<size_t, size_t> >::iterator d_it = it;
		for (; d_it != down[j].end(); ++d_it) {
			if (d_it->second < elem.first) {
				break;
			} else {
				up[j].erase(reverse_pair(*d_it));
			}
		}
		down[j].erase(it, d_it);
		up[j].insert(elem);
		down[j].insert(reverse_pair(elem));
#if 0
		for (int j = 0; j < k; j++) {
			printf("%d:", j);
			for (it = up[j].begin(); it != up[j].end(); ++it)
				printf(" (%lu,%lu)", it->first, it->second);
			printf("\n");
		}
		printf("\n");
#endif
	}

	return k;
}

int main()
{
	int n, m;
	int result;

	read_input("sccm.in", n, m);
	result = solve_problem(n, m);
	write_output("sccm.out", result);

	return 0;
}
