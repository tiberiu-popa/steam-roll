#include <algorithm>
#include <functional>
#include <vector>
#include <cmath>
#include <cstdio>
#include <cstring>

#define BUFFER_SIZE 27
#define MAX_BASE 10
#define PRECOMP_LIMIT 1000

using namespace std;

bool precomp[PRECOMP_LIMIT][MAX_BASE + 1];

char *trim_line(char *str)
{
	return strtok(str, "\r\n");
}

int compute_digit_sum(int x, int base)
{
	int sum = 0;
	while (x > 0) {
		int digit = x % base;
		x /= base;
		sum += digit * digit;
	}
	return sum;
}

bool is_happy(int x, int base)
{
	int y = x;

	do {
		x = compute_digit_sum(x, base);
		y = compute_digit_sum(y, base);
		y = compute_digit_sum(y, base);
	} while (x != y);

	return (x == 1);
}

bool fast_is_happy(int x, int base)
{
	while (x >= PRECOMP_LIMIT) {
		x = compute_digit_sum(x, base);
	}

	return precomp[x][base];
}

int solve_problem(int num_case)
{
	vector<int> bases;
	char str[BUFFER_SIZE + 10];

	if (fgets(str, sizeof(str), stdin) == NULL)
		return 1;

	char *p = trim_line(str);
	for (char *q = strtok(p, " "); q != NULL; q = strtok(NULL, " ")) {
		bases.push_back(atoi(q));
	}

	sort(bases.begin(), bases.end(), greater<int>());
	for (int x = 2; ; x++) {
		bool happy = true;
		for (vector<int>::iterator it = bases.begin(); it != bases.end(); ++it) {
			if (!fast_is_happy(x, *it)) {
				happy = false;
				break;
			}
		}
		if (happy) {
			printf("Case #%d: %d\n", num_case, x);
			break;
		}
	}

	return 0;
}

int main(int argc, char *argv[])
{
	const char *filenames[] = {
		"multi-happiness.in",
		"multi-happiness.out",
	};
	int num_tests;
	char str[BUFFER_SIZE + 10];

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

	for (int i = 1; i < PRECOMP_LIMIT; i++)
		for (int j = 2; j <= MAX_BASE; j++)
			precomp[i][j] = is_happy(i, j);

	if (fgets(str, sizeof(str), stdin) == NULL)
		return 1;
	num_tests = atoi(str);
	for (int i = 0; i < num_tests; i++)
		solve_problem(i + 1);

	return 0;
}
