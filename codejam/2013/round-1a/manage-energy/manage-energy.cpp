#include <algorithm>
#include <vector>
#include <cmath>
#include <cstdio>
#include <cstring>

#define MAX_N 10000

using namespace std;

int v[MAX_N];
long long x[MAX_N];

int solve_problem(int num_case)
{
	int e, r, n;

	if (scanf("%d %d %d", &e, &r, &n) != 3)
		return 1;

	for (int i = 0; i < n; i++)
		if (scanf("%d", &v[i]) != 1)
				return 1;

	if (r > e)
		r = e;

	x[0] = e;
	for (int i = 1; i < n; i++) {
		x[i] = r;
		for (int j = i - 1; j >= 0; j--)
			if (v[j] < v[i]) {
				if (x[j] + x[i] < e) {
					x[i] += x[j];
					x[j] = 0;
				} else {
					x[j] -= e - x[i];
					x[i] = e;
					break;
				}
			} else {
				break;
			}
	}

	long long sum = 0;
	for (int i = 0; i < n; i++)
		sum += x[i] * v[i];

	printf("Case #%d: %lld\n", num_case, sum);
	return 0;
}

int main()
{
	int num_tests;

	if (freopen("manage-energy.in", "rt", stdin) == NULL) {
		fprintf(stderr, "Could not reopen stdin\n");
		return 1;
	}
	if (freopen("manage-energy.out", "wt", stdout) == NULL) {
		fprintf(stderr, "Could not reopen stdout\n");
		return 1;
	}

	if (scanf("%d", &num_tests) != 1)
		return 1;
	for (int i = 0; i < num_tests; i++)
		solve_problem(i + 1);

	return 0;
}
