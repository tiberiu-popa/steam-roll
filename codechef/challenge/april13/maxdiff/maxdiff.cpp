#include <algorithm>
#include <vector>
#include <cmath>
#include <cstdio>
#include <cstring>

#define MAX_N 100

using namespace std;

void solve_problem()
{
	int v[MAX_N];
	int n, k;

	if (scanf("%d %d", &n, &k) != 2)
		return;

	for (int i = 0; i < n; i++)
		if (scanf("%d", &v[i]) != 1)
			return;

	sort(v, v + n);
	int a = 0, b = 0, c = 0;
	for (int i = 0; i < n; i++) {
		a += v[i];
		if (i < k)
			b += v[i];
		if (i < n - k)
			c += v[i];
	}
	int result = max(abs(a - 2 * b), abs(a - 2 * c));
	printf("%d\n", result);
}

int main()
{
	int num_tests;

	if (scanf("%d", &num_tests) != 1)
		return 1;
	for (int i = 0; i < num_tests; i++)
		solve_problem();

	return 0;
}
