#include <cstdio>

#define NUM_POSTS 4

using namespace std;

int solve_problem()
{
	int a[NUM_POSTS], b[NUM_POSTS], c[NUM_POSTS], d[NUM_POSTS];
	int n;

	if (scanf("%d", &n) != 1)
		return 1;

	for (int i = 0; i < 4; i++)
		if (scanf("%d %d %d %d", &a[i], &b[i], &c[i], &d[i]) != 4)
			return 1;

	for (int i = 0; i < 4; i++) {
		if (a[i] + c[i] <= n) {
			printf("%d %d %d\n", i + 1, a[i], n - a[i]);
			return 0;
		} else if (a[i] + d[i] <= n) {
			printf("%d %d %d\n", i + 1, a[i], n - a[i]);
			return 0;
		} else if (b[i] + c[i] <= n) {
			printf("%d %d %d\n", i + 1, b[i], n - b[i]);
			return 0;
		} else if (b[i] + d[i] <= n) {
			printf("%d %d %d\n", i + 1, b[i], n - b[i]);
			return 0;
		}
	}

	printf("-1\n");

	return 0;
}

int main()
{
	solve_problem();
	return 0;
}
