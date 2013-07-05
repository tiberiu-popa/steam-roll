#include <algorithm>
#include <map>
#include <cmath>
#include <cstdio>
#include <cstring>

#define MAX_N 100000
// (1 << (NUM_BITS - 1)) >= MAX_N
#define NUM_BITS 18

using namespace std;

int b[MAX_N];
int min_q[MAX_N][NUM_BITS];
int max_q[MAX_N][NUM_BITS];

int get_lower_exp(int n)
{
	int k = -1;
	while (n > 0) {
		n /= 2;
		k++;
	}
	return k;
}

int min_interval(int x, int y)
{
	int diff = y - x;
	int e = get_lower_exp(diff);
	return min(min_q[x][e], min_q[y - (1 << e)][e]);
}

int max_interval(int x, int y)
{
	int diff = y - x;
	int e = get_lower_exp(diff);
	return max(max_q[x][e], max_q[y - (1 << e)][e]);
}

int solve_problem()
{
	int n, q;

	if (scanf("%d", &n) != 1)
		return 1;

	for (int i = 0; i < n; i++) {
		if (scanf("%d", &b[i]) != 1)
			return 1;
	}

	for (int i = 0; i < n; i++)
		min_q[i][0] = max_q[i][0] = b[i];

	for (int j = 1; (1 << j) <= n; j++) {
		int offset = 1 << j;
		for (int i = 0; i + offset <= n; i++) {
			min_q[i][j] = min(min_q[i][j - 1], min_q[i + offset / 2][j - 1]);
			max_q[i][j] = max(max_q[i][j - 1], max_q[i + offset / 2][j - 1]);
		}
	}

	if (scanf("%d", &q) != 1)
		return 1;

	for (int i = 0; i < q; i++) {
		int x, y;
		if (scanf("%d %d", &x, &y) != 2)
			return 1;
		y++;
		int l = min_interval(x, y);
		int r = max_interval(x, y);
		double result = (l + r) / 2.0;
		double cand = l + max(max_interval(0, x), max_interval(y, n));
		if (cand > result)
			result = cand;
		printf("%.1lf\n", result);
	}

	return 0;
}

int main()
{
	int num_tests = 1;

	for (int i = 0; i < num_tests; i++) {
		int ret = solve_problem();
		if (ret != 0) {
			fprintf(stderr, "Read error at case #%d\n", i + 1);
			break;
		}
	}

	return 0;
}
