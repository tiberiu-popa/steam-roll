#include <algorithm>
#include <map>
#include <vector>
#include <limits>
#include <cmath>
#include <cstdio>
#include <cstring>

#define MAX_N 100

using namespace std;

int solve_problem(int num_case)
{
	int n, s, p;
	int v[MAX_N];

	if (scanf("%d %d %d", &n, &s, &p) != 3)
		return 1;

	for (int i = 0; i < n; i++)
		if (scanf("%d", &v[i]) != 1)
			return 1;

	int result = 0;
	int almost = 0;
	for (int i = 0; i < n; i++) {
		int q = v[i] / 3;
		switch (v[i] % 3) {
		case 0:
			if (q >= p)
				++result;
			else if (v[i] >= 3 && q >= p - 1)
				++almost;
			break;

		case 1:
			if (q + 1 >= p)
				++result;
			break;

		case 2:
			if (q + 1 >= p)
				++result;
			else if (q + 1 >= p - 1)
				++almost;
			break;

		default:
			break;
		}
	}

	printf("Case #%d: %d\n", num_case, result + min(almost, s));

	return 0;
}

int main(int argc, char *argv[])
{
	const char *filenames[] = {
		"dancing-googlers.in",
		"dancing-googlers.out",
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
