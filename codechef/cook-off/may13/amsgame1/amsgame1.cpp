#include <algorithm>
#include <functional>
#include <vector>
#include <cmath>
#include <cstdio>
#include <cstring>

#define MAX_N 1000

using namespace std;

int v[MAX_N];

int gcd(int x, int y)
{
	if (x < y)
		swap(x, y);
	while (y > 0) {
		int r = x % y;
		x = y;
		y = r;
	}
	return x;
}

int solve_problem()
{
	int n;

	if (scanf("%d", &n) != 1)
		return 1;

	for (int i = 0; i < n; i++)
		if (scanf("%d", &v[i]) != 1)
			return 1;

	int result = v[0];
	for (int i = 1; i < n; i++)
		result = gcd(result, v[i]);
	printf("%d\n", result);

	return 0;
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
