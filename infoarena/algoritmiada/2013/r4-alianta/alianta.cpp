#include <algorithm>
#include <set>
#include <vector>
#include <cstdio>

#define MAX_N 36

using namespace std;

set<int> neighbours[MAX_N];
long long neighbour_masks[MAX_N];
int first_masks[1 << (MAX_N / 2)];
bool second_masks[1 << ((MAX_N + 1) / 2)];
int neighbour_set[1 << ((MAX_N + 1) / 2)];

int find_2_power(int n)
{
	int exponent = -1;
	for (; n > 0; n >>= 1)
		++exponent;
	return exponent;
}

void solve_problem()
{
	int n, m;

	if (scanf("%d %d", &n, &m) != 2)
		return;
	for (int i = 0; i < n; i++)
		neighbours[i].clear();
	for (int i = 0; i < m; i++) {
		int x, y;
		if (scanf("%d %d", &x, &y) != 2)
			return;
		--x;
		--y;
		neighbours[x].insert(y);
		neighbours[y].insert(x);
	}

	for (int i = 0; i < n; i++) {
		long long neighbour_mask = (1LL << n) - 1;
		for (set<int>::iterator it = neighbours[i].begin(); it != neighbours[i].end(); ++it)
			neighbour_mask &= ~(1LL << (*it));
		neighbour_masks[i] = neighbour_mask;
	}

	int max_first_mask = 1 << (n / 2);
	first_masks[0] = 0;
	for (int i = 1; i < max_first_mask; i++) {
		int j = i & (i - 1);
		if (j > 0) {
			int vertex = find_2_power(i & ~j);
			int cand_count = 1 + first_masks[j & neighbour_masks[vertex]];
			first_masks[i] = max(first_masks[j], cand_count);
		} else {
			first_masks[i] = 1;
		}
	}

	int max_count = 0;

	int first_all_mask = (1 << (n / 2)) - 1;
	int max_second_mask = 1 << ((n + 1) / 2);
	second_masks[0] = true;
	neighbour_set[0] = 0;
	for (int i = 1; i < max_second_mask; i++) {
		int j = i & (i - 1);
		int set_mask;
		if (j > 0) {
			int vertex = find_2_power(i & ~j);
			long long vertex_neighbours = neighbour_masks[n / 2 + vertex];
			set_mask = neighbour_set[j] & vertex_neighbours;
			vertex_neighbours >>= n / 2;
			second_masks[i] = second_masks[j] && ((j & vertex_neighbours) == j);
		} else {
			int vertex = find_2_power(i);
			set_mask = neighbour_masks[n / 2 + vertex] & first_all_mask;
			second_masks[i] = true;
		}
		neighbour_set[i] = set_mask;
		if (second_masks[i]) {
			int num_bits = 0;
			for (int tmp = i; tmp > 0; tmp &= tmp - 1)
				num_bits++;
			int count = num_bits + first_masks[set_mask];
			if (count > max_count)
				max_count = count;
		}
	}

	printf("%d\n", max_count);
}

int main()
{
	if (freopen("alianta.in", "rt", stdin) == NULL)
		return 1;
	if (freopen("alianta.out", "wt", stdout) == NULL)
		return 1;

	int num_tests;
	if (scanf("%d", &num_tests) != 1)
		return 1;
	for (int i = 0; i < num_tests; i++)
		solve_problem();

	return 0;
}
