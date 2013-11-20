#include <limits>
#include <vector>
#include <cstdio>
#include <cstring>

#define BASE 10

using namespace std;

int contains_digits(int x, int k)
{
	bool used[BASE];
	memset(used, 0, sizeof(used));
	while (x > 0) {
		int d = x % 10;
		used[d] = true;
		x /= 10;
	}
	for (int i = 0; i <= k; i++)
		if (!used[i])
			return 0;
	return 1;
}

int solve_problem()
{
	int n, k;

	if (scanf("%d %d", &n, &k) != 2)
		return 1;

	int result = 0;
	for (int i = 0; i < n; i++) {
		int x;
		if (scanf("%d", &x) != 1)
			return 1;
		if (contains_digits(x, k))
			++result;
	}
	printf("%d\n", result);

	return 0;
}

int main()
{
	solve_problem();
	return 0;
}
