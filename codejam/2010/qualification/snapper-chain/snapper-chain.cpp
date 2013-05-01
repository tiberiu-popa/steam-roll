#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>

using namespace std;

int solve_problem(int num_case)
{
	int n, k;

	if (scanf("%d %d", &n, &k) != 2)
		return 1;

	printf("Case #%d: %s\n", num_case, (k + 1) % (1 << n) == 0 ? "ON" : "OFF");

	return 0;
}

int main(int argc, char *argv[])
{
	const char *filenames[] = {
		"snapper-chain.in",
		"snapper-chain.out",
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
