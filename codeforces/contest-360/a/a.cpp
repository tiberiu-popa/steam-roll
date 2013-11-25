#include <algorithm>
#include <cstdio>

#define MAX_N 5000
#define MAX_M 5000
#define MAX_VALUE 1000000000

using namespace std;

struct op {
	int t;
	int left, right;
	int value;
};

int v[MAX_N];
int u[MAX_N];
int delta[MAX_N];
op operations[MAX_M];

int solve_problem()
{
	int n, m;

	if (scanf("%d %d", &n, &m) != 2)
		return 1;

	fill(v, v + n, MAX_VALUE);
	for (int i = 0; i < m; i++) {
		op &e = operations[i];
		if (scanf("%d %d %d %d", &e.t, &e.left, &e.right, &e.value) != 4)
			return 1;
		--e.left;
		--e.right;
		if (e.t == 1) {
			for (int j = e.left; j <= e.right; j++)
				delta[j] += e.value;
		} else {
			for (int j = e.left; j <= e.right; j++) {
				int cand = e.value - delta[j];
				if (cand < v[j])
					v[j] = cand;
			}
		}
	}

	copy(v, v + n, u);

	for (int i = 0; i < m; i++) {
		op &e = operations[i];
		if (e.t == 1) {
			for (int j = e.left; j <= e.right; j++)
				u[j] += e.value;
		} else {
			int maxval = *max_element(u + e.left, u + e.right + 1);
			if (maxval < e.value) {
				printf("NO\n");
				return 0;
			}
		}
	}

	printf("YES\n");
	for (int i = 0; i < n; i++)
		printf((i > 0) ? " %d" : "%d", v[i]);
	printf("\n");

	return 0;
}

int main()
{
	solve_problem();
	return 0;
}
