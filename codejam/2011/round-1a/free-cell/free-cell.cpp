#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>

using namespace std;

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

int solve_problem(int num_case)
{
	long long n;
	int pd, pg;

	if (scanf("%lld %d %d", &n, &pd, &pg) != 3)
		return 1;

	int d = 100 / gcd(pd, 100);

	bool result = (pg != 100 || pd == 100) && (pg != 0 || pd == 0) && d <= n;
	printf("Case #%d: %s\n", num_case, result ? "Possible" : "Broken");

	return 0;
}

int main(int argc, char *argv[])
{
	const char *filenames[] = {
		"free-cell.in",
		"free-cell.out",
	};
	int num_tests;

	for (int i = 1; i < argc && i <= 2; i++)
		filenames[i - 1] = argv[i];

	if (freopen(filenames[0], "rt", stdin) == NULL) {
		fprintf(stderr, "Could not reopen stdin\n");
		return 1;
	}
	if (freopen(filenames[1], "wt", stdout) == NULL) {
		fprintf(stderr, "Could not reopen stdout\n");
		return 1;
	}

	if (scanf("%d", &num_tests) != 1)
		return 1;
	for (int i = 0; i < num_tests; i++)
		solve_problem(i + 1);

	return 0;
}
