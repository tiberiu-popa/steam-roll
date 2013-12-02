#include <algorithm>
#include <cstdio>

#define MAX_N 100000
#define MAX_M 20

using namespace std;

int pos[MAX_N];
int v[MAX_M];
int dp[1 << MAX_M];

int solve_problem()
{
	int n, m, d;

	if (scanf("%d %d %d", &n, &m, &d) != 3)
		return 1;

	for (int i = 0; i < m; i++) {
		int num_elems;
		if (scanf("%d", &num_elems) != 1)
			return 1;
		for (int j = 0; j < num_elems; j++) {
			int elem;
			if (scanf("%d", &elem) != 1)
				return 1;
			--elem;
			pos[elem] = i;
		}
	}

	fill(v, v + m, -1);
	for (int i = 0; i < n; i++) {
		int mask = 0;
		v[pos[i]] = i;
		if (i >= d - 1) {
			for (int j = 0; j < m; j++)
				if (v[j] > i - d)
					mask |= (1 << j);
			dp[mask] = 1;
		}
	}

	int result = m;
	int max_mask = 1 << m;
	for (int i = 0; i < max_mask; i++)
		if (dp[i]) {
			for (int j = 0; j < m; j++)
				dp[i | (1 << j)] = 1;
		} else {
			int k = i;
			int x = 0;
			while (k > 0) {
				if (k & 1)
					++x;
				k >>= 1;
			}
			x = m - x;
			if (x < result)
				result = x;
		}

	printf("%d\n", result);

	return 0;
}

int main()
{
	solve_problem();
	return 0;
}
