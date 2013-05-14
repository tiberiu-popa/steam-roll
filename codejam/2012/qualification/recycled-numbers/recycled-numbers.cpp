#include <algorithm>
#include <map>
#include <vector>
#include <limits>
#include <cmath>
#include <cstdio>
#include <cstring>

#define MAX_N 2000000

using namespace std;

int v[MAX_N];

void compute_digits(vector<int> &digits, int n)
{
	digits.clear();
	while (n > 0) {
		digits.push_back(n % 10);
		n /= 10;
	}
}

int construct_number(vector<int> &digits, size_t pos)
{
	int n = 0;
	for (size_t i = 0; i < digits.size(); i++) {
		n = 10 * n + digits[pos];
		if (pos > 0)
			--pos;
		else
			pos = digits.size() - 1;
	}
	return n;
}

int solve_problem(int num_case)
{
	vector<int> digits;
	int x, y;

	if (scanf("%d %d", &x, &y) != 2)
		return 1;

	int result = 0;
	fill(v + x, v + y + 1, 0);
	for (int i = x; i <= y; i++)
		if (!v[i]) {
			int count = 0;
			compute_digits(digits, i);
			for (size_t j = 0; j < digits.size(); j++)
				if (digits[j] > 0) {
					int m = construct_number(digits, j);
					if (m >= x && m <= y && !v[m]) {
						++count;
						v[m] = 1;
					}
				}
			result += count * (count - 1) / 2;
		}

	printf("Case #%d: %d\n", num_case, result);

	return 0;
}

int main(int argc, char *argv[])
{
	const char *filenames[] = {
		"recycled-numbers.in",
		"recycled-numbers.out",
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
