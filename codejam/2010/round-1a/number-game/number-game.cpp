#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>

using namespace std;

const double phi = (sqrt(5) + 1) / 2;

int solve_problem(int num_case)
{
	int a, b, c, d;

	if (scanf("%d %d %d %d", &a, &b, &c, &d) != 4)
		return 1;

	long long result = 0;
	for (int i = c; i <= d; i++) {
		int inf_b = (int) ceil(i * phi);
		int sup_a = (int) floor(i / phi);
		if (inf_b <= b)
			result += b - max(inf_b, a) + 1;
		if (sup_a >= a)
			result += min(sup_a, b) - a + 1;
	}

	printf("Case #%d: %lld\n", num_case, result);

	return 0;
}

int main(int argc, char *argv[])
{
	const char *filenames[] = {
		"number-game.in",
		"number-game.out",
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
