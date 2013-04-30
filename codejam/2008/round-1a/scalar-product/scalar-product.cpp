#include <algorithm>
#include <vector>
#include <cmath>
#include <cstdio>
#include <cstring>

using namespace std;

int solve_problem(int num_case)
{
	vector<int> x, y;
	int n;

	if (scanf("%d", &n) != 1)
		return 1;

	for (int i = 0; i < n; i++) {
		int elem;
		if (scanf("%d", &elem) != 1)
			return 1;
		x.push_back(elem);
	}

	for (int i = 0; i < n; i++) {
		int elem;
		if (scanf("%d", &elem) != 1)
			return 1;
		y.push_back(elem);
	}

	sort(x.begin(), x.end());
	sort(y.rbegin(), y.rend());

	long long sum = 0;
	for (vector<int>::iterator x_it = x.begin(), y_it = y.begin();
			x_it != x.end() && y_it != y.end(); ++x_it, ++y_it) {
		long long prod = *x_it;
		sum += prod * (*y_it);
	}

	printf("Case #%d: %lld\n", num_case, sum);

	return 0;
}

int main(int argc, char *argv[])
{
	const char *filenames[] = {
		"scalar-product.in",
		"scalar-product.out",
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
