#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>

#define BUFFER_SIZE 1

using namespace std;

int solve_problem(int num_case)
{
	int n;
	int pos[2] = { 1, 1 };
	int move[2] = { 0, 0 };
	char str[BUFFER_SIZE + 1];

	if (scanf("%d", &n) != 1)
		return 1;

	for (int i = 0; i < n; i++) {
		int elem;
		if (scanf("%s %d", str, &elem) != 2)
			return 1;
		if (str[0] == 'O') {
			move[0] = 1 + max(move[1], move[0] + abs(pos[0] - elem));
			pos[0] = elem;
		} else {
			move[1] = 1 + max(move[0], move[1] + abs(pos[1] - elem));
			pos[1] = elem;
		}
	}

	printf("Case #%d: %d\n", num_case, max(move[0], move[1]));

	return 0;
}

int main(int argc, char *argv[])
{
	const char *filenames[] = {
		"bot-trust.in",
		"bot-trust.out",
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
