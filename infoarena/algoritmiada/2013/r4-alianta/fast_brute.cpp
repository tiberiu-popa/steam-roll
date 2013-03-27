#include <algorithm>
#include <set>
#include <vector>
#include <cstdio>

#define MAX_N 36

using namespace std;

set<int> neighbours[MAX_N];

int generate_cliques(set<long long> &masks, int current, int n)
{
	int max_count = 0;
	if (current == n)
		return max_count;

	long long neighbour_mask = 0LL;
	for (int i = 0; i < current; i++)
		if (neighbours[current].find(i) != neighbours[current].end())
			neighbour_mask |= 1LL << i;
	set<long long> additional;
	set<long long> clutter;
	for (set<long long>::iterator it = masks.begin(); it != masks.end(); ++it) {
		long long mask = *it;
		long long new_mask = mask & ~neighbour_mask;
		if (new_mask == mask)
			clutter.insert(mask);
		new_mask |= 1LL << current;
		additional.insert(new_mask);
		int count = 0;
		while (new_mask > 0) {
			count++;
			new_mask &= new_mask - 1;
		}
		if (count > max_count)
			max_count = count;
	}
	for (set<long long>::iterator it = additional.begin(); it != additional.end(); ++it)
		masks.insert(*it);
	for (set<long long>::iterator it = clutter.begin(); it != clutter.end(); ++it)
		masks.erase(*it);

	int cand_count = generate_cliques(masks, current + 1, n);
	return max(cand_count, max_count);
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

	set<long long> masks;
	masks.insert(0LL);
	int max_count = generate_cliques(masks, 0, n);
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
