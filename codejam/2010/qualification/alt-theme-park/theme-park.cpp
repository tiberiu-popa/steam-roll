#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>

#define MAX_N 1000

using namespace std;

int g[MAX_N];
long long ride[MAX_N];
int next[MAX_N];
long long cycle[MAX_N];
int visited[MAX_N];

int solve_problem(int num_case)
{
	int r, n;
	long long k;

	if (scanf("%d %lld %d", &r, &k, &n) != 3)
		return 1;

	for (int i = 0; i < n; i++)
		if (scanf("%d", &g[i]) != 1)
			return 1;

	long long sum = 0;
	int j = 0;
	for (int i = 0; i < n; i++) {
		bool first = true;
		for (; first || i != j; j = (j + 1) % n) {
			if (sum + g[j] <= k) {
				sum += g[j];
			} else {
				next[i] = j;
				ride[i] = sum;
				break;
			}
			first = false;
		}
		if (!first && i == j) {
			next[i] = i;
			ride[i] = sum;
		}
		sum -= g[i];
		if (i == j)
			j = (j + 1) % n;
	}

	int cycle_start = -1;
	int cycle_length = -1;
	fill(visited, visited + n, -1);
	j = 0;
	for (int i = 0; ; i++)
		if (visited[j] >= 0) {
			cycle_start = visited[j];
			cycle_length = i - visited[j];
			break;
		} else {
			cycle[i] = ride[j];
			visited[j] = i;
			j = next[j];
		}

	long long result = 0;
	for (int i = 0; i < cycle_start && i < r; i++)
		result += cycle[i];

	if (r > cycle_start) {
		if (r > cycle_start + cycle_length) {
			long long cycle_sum = 0;
			for (int i = cycle_start; i < cycle_start + cycle_length; i++)
				cycle_sum += cycle[i];
			int diff = r - cycle_start;
			result += (diff / cycle_length) * cycle_sum;
			int rem = diff % cycle_length;
			for (int i = cycle_start; i < cycle_start + rem; i++)
				result += cycle[i];
		} else {
			for (int i = cycle_start; i < r; i++)
				result += cycle[i];
		}
	}

	printf("Case #%d: %lld\n", num_case, result);

	return 0;
}

int main(int argc, char *argv[])
{
	const char *filenames[] = {
		"theme-park.in",
		"theme-park.out",
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
