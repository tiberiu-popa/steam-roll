#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>

#define MAX_A 99999

using namespace std;

double p[MAX_A];

int solve_problem(int num_case)
{
	int a, b;

	if (scanf("%d %d", &a, &b) != 2)
		return 1;

	for (int i = 0; i < a; i++)
		if (scanf("%lf", &p[i]) != 1)
			return 1;

	double result = b + 2;
	double cp = 1.0;
	for (int i = 0; i <= a; i++) {
		double cand = 2 * (a - i) + b - a + 1 + (1 - cp) * (b + 1);
		if (cand < result)
			result = cand;
		cp *= p[i];
	}
	printf("Case #%d: %lf\n", num_case, result);

	return 0;
}

int main(int argc, char *argv[])
{
	const char *filenames[] = {
		"password-problem.in",
		"password-problem.out",
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
