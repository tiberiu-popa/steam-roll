#include <stdio.h>

#define MAX_ELEMS 15

int nodes[1 << MAX_ELEMS];

int get_msb(int n)
{
	int msb = 0;

	for (; n > 0; n >>= 1)
		msb++;

	return msb - 1;
}

void solve_problem()
{
	int v[MAX_ELEMS];
	int n, m;
	int winner;
	int msb, nsb, tmp;
	int i, j;

	scanf("%d", &n);
	for (i = 0; i < n; i++)
		scanf("%d", &v[i]);

	m = 1 << n;
	for (i = 0; i < m; i++)
		nodes[i] = -1;
	for (i = 0; i < n; i++)
		nodes[1 << i] = 1;

	for (i = 1; i < m; i++)
		if (nodes[i] < 0) {
			msb = get_msb(i);
			j = i & ~(1 << msb);
			if (nodes[j] == 1) {
				nsb = get_msb(j);
				if (v[msb] > v[nsb])
					nodes[i] = 1;
			}
		}

	for (i = 0; i < m; i++)
		if (nodes[i] < 0) {
			msb = 0;
			winner = 0;
			for (tmp = i; tmp > 0; tmp >>= 1) {
				if (tmp & 1) {
					j = i & ~(1 << msb);
					if (nodes[j] == 1) {
						winner = 1;
						break;
					}
				}
				msb++;
			}
			nodes[i] = winner ^ 1;
		}

	printf("%s\n", nodes[m - 1] ? "Bob" : "Alice");
}

int main()
{
	int i, num_tests;

	scanf("%d", &num_tests);
	for (i = 0; i < num_tests; i++)
		solve_problem();

	return 0;
}
