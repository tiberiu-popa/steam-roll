#include <algorithm>
#include <functional>
#include <limits>
#include <set>
#include <vector>
#include <cmath>
#include <cstdio>
#include <cstring>

#define MAX_N 200

using namespace std;

int s[MAX_N];
int t[MAX_N + 1];
double result[MAX_N];

int solve_problem(int num_case)
{
	vector< pair<int, int> > v;
	int n;

	if (scanf("%d", &n) != 1)
		return 1;

	for (int i = 0; i < n; i++)
		if (scanf("%d", &s[i]) != 1)
			return 1;

	for (int i = 0; i < n; i++)
		v.push_back(make_pair(s[i], i));
	sort(v.begin(), v.end());

	int sum = 0;
	t[0] = 0;
	for (size_t i = 0; i < v.size(); i++) {
		int elem = v[i].first;
		sum += elem;
		t[i + 1] = t[i] + elem;
	}

	for (size_t k = 0; k < v.size(); k++) {
		int index = v[k].second;
		result[index] = 0.0;
		for (size_t i = k; i < v.size(); i++) {
			double sup_limit = 1.0;
			if (i + 1 < v.size())
				sup_limit = (double) (v[i + 1].first - v[k].first) / sum;
			double inf_limit = (double) (v[i].first - v[k].first) / sum;
			double cand = (1 - ((double) (i + 1) * v[k].first - t[i + 1]) / sum) / (i + 1);
			if (cand >= inf_limit) {
				if (cand > sup_limit)
					cand = sup_limit;
				result[index] = max(result[index], cand);
			}
		}
	}

	printf("Case #%d:", num_case);
	for (int i = 0; i < n; i++)
		printf(" %lf", 100 * result[i]);
	printf("\n");

	return 0;
}

int main(int argc, char *argv[])
{
	const char *filenames[] = {
		"safety-numbers.in",
		"safety-numbers.out",
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
