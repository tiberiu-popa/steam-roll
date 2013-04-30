#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <cmath>
#include <cstdio>
#include <cstring>

#define MAX_N 2000
#define MAX_M 2000

using namespace std;

unordered_set<int> ordinary[MAX_M];
unordered_set<int> reverse_set[MAX_N + 1];
int special[MAX_M];
int malted[MAX_N + 1];

int solve_problem(int num_case)
{
	unordered_set<int> zero_counts[2];
	vector<int> x, y;
	int n, m;

	if (scanf("%d %d", &n, &m) != 2)
		return 1;

	fill(special, special + m, 0);
	fill(malted, malted + n + 1, 0);

	for (int i = 0; i < m; i++)
		ordinary[i].clear();

	for (int i = 0; i <= n; i++)
		reverse_set[i].clear();

	for (int i = 0; i < m; i++) {
		int num_elems;
		if (scanf("%d", &num_elems) != 1)
			return 1;
		for (int j = 0; j < num_elems; j++) {
			int x, y;
			if (scanf("%d %d", &x, &y) != 2)
				return 1;
			if (y) {
				special[i] = x;
			} else {
				ordinary[i].insert(x);
				reverse_set[x].insert(i);
			}
		}
	}

	for (int i = 0; i < m; i++)
		if (ordinary[i].empty())
			zero_counts[0].insert(i);

	bool possible = true;
	int pos = 0;
	while (!zero_counts[pos].empty()) {
		for (int i : zero_counts[pos]) {
			int x = special[i];
			if (x > 0) {
				for (int j : reverse_set[x]) {
					ordinary[j].erase(x);
					if (ordinary[j].empty())
						zero_counts[pos ^ 1].insert(j);
				}
				malted[x] = 1;
			} else {
				possible = false;
				break;
			}
		}
		if (!possible)
			break;
		zero_counts[pos].clear();
		pos ^= 1;
	}

	printf("Case #%d:", num_case);
	if (possible) {
		for (int i = 1; i <= n; i++)
			printf(" %d", malted[i]);
	} else {
		printf(" IMPOSSIBLE");
	}
	printf("\n");

	return 0;
}

int main(int argc, char *argv[])
{
	const char *filenames[] = {
		"milkshakes.in",
		"milkshakes.out",
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
