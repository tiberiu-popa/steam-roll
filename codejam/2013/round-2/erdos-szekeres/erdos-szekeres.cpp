#include <algorithm>
#include <vector>
#include <cmath>
#include <cstdio>
#include <cstring>

#define MAX_N 2000

using namespace std;

int a[MAX_N];
int b[MAX_N];
int v[MAX_N];

vector<int> edges[MAX_N];
int prev[MAX_N + 1];
int indeg[MAX_N];

template<class T>
void trim_unique(vector<T> &v)
{
	typename vector<T>::iterator it = unique(v.begin(), v.end());
	typename vector<T>::difference_type diff = it - v.begin();
	v.resize((typename vector<T>::size_type) diff);
}

int solve_problem(int num_case)
{
	int n;

	if (scanf("%d", &n) != 1)
		return 1;

	for (int i = 0; i < n; i++)
		if (scanf("%d", &a[i]) != 1)
			return 1;

	for (int i = 0; i < n; i++)
		if (scanf("%d", &b[i]) != 1)
			return 1;

	for (int i = 0; i < n; i++)
		edges[i].clear();

	fill(prev, prev + n + 1, -1);
	for (int i = 0; i < n; i++) {
		int last = prev[a[i]];
		if (last >= 0)
			edges[last].push_back(i);
		last = prev[a[i] - 1];
		if (last >= 0)
			edges[i].push_back(last);
		prev[a[i]] = i;
	}

	fill(prev, prev + n + 1, -1);
	for (int i = n - 1; i >= 0; i--) {
		int last = prev[b[i]];
		if (last >= 0)
			edges[last].push_back(i);
		last = prev[b[i] - 1];
		if (last >= 0)
			edges[i].push_back(last);
		prev[b[i]] = i;
	}

	for (int i = 0; i < n; i++) {
		sort(edges[i].begin(), edges[i].end());
		trim_unique(edges[i]);
	}

	fill(indeg, indeg + n, 0);
	for (int i = 0; i < n; i++)
		for (size_t j = 0; j < edges[i].size(); j++)
			indeg[edges[i][j]]++;

	vector<int> u;
	for (int i = 0; i < n; i++)
		if (indeg[i] == 0)
			u.push_back(i);

	make_heap(u.begin(), u.end());

	for (int i = 0; i < n; i++) {
		int elem = u.front();
		pop_heap(u.begin(), u.end());
		u.pop_back();
		for (size_t j = 0; j < edges[elem].size(); j++) {
			int node = edges[elem][j];
			if (--indeg[node] == 0) {
				u.push_back(node);
				push_heap(u.begin(), u.end());
			}
		}
		v[elem] = n - i;
	}

	printf("Case #%d:", num_case);
	for (int i = 0; i < n; i++)
		printf(" %d", v[i]);
	printf("\n");

	return 0;
}

int main(int argc, char *argv[])
{
	const char *filenames[] = {
		"erdos-szekeres.in",
		"erdos-szekeres.out",
	};
	int num_tests;

	for (int i = 1; i < argc && i <= 2; i++)
		filenames[i - 1] = argv[i];

	if (freopen(filenames[0], "rt", stdin) == NULL) {
		fprintf(stderr, "Could not reopen stdin\n");
		return 1;
	}
	if (freopen(filenames[1], "wt", stdout) == NULL) {
		fprintf(stderr, "Could not reopen stdout\n");
		return 1;
	}

	if (scanf("%d", &num_tests) != 1)
		return 1;
	for (int i = 0; i < num_tests; i++)
		solve_problem(i + 1);

	return 0;
}
