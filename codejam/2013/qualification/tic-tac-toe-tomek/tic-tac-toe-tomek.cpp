#include <algorithm>
#include <vector>
#include <cmath>
#include <cstdio>
#include <cstring>

#define BUFFER_SIZE 10

using namespace std;

int get_player(char ch)
{
	switch (ch) {
		case 'X':
			return 0;
		case 'O':
			return 1;
		case 'T':
			return 2;
		default:
			return 3;
	}
}

int has_won(int player, int grid[][4])
{
	for (int i = 0; i < 4; i++) {
		int won = 1;
		for (int j = 0; j < 4; j++)
			if (grid[i][j] != player && grid[i][j] != 2) {
				won = 0;
				break;
			}
		if (won)
			return 1;
	}

	for (int j = 0; j < 4; j++) {
		int won = 1;
		for (int i = 0; i < 4; i++)
			if (grid[i][j] != player && grid[i][j] != 2) {
				won = 0;
				break;
			}
		if (won)
			return 1;
	}

	int d_won = 1;
	for (int i = 0; i < 4; i++)
		if (grid[i][i] != player && grid[i][i] != 2) {
			d_won = 0;
			break;
		}
	if (d_won)
		return 1;

	d_won = 1;
	for (int i = 0; i < 4; i++)
		if (grid[i][3 - i] != player && grid[i][3 - i] != 2) {
			d_won = 0;
			break;
		}
	if (d_won)
		return 1;

	return 0;
}

int solve_problem(int num_case)
{
	char *p;
	int grid[4][4];
	char str[BUFFER_SIZE];

	for (int i = 0; i < 5; i++) {
		if (fgets(str, sizeof(str), stdin) == NULL) {
			if (i < 4)
				return 1;
		}
		if (i < 4) {
			for (int j = 0; j < 4; j++)
				grid[i][j] = get_player(str[j]);
		}
	}

	printf("Case #%d: ", num_case);
	if (has_won(0, grid)) {
		printf("X won\n");
	} else if (has_won(1, grid)) {
		printf("O won\n");
	} else {
		int full = 1;
		for (int i = 0; i < 4 && full; i++)
			for (int j = 0; j < 4; j++)
				if (grid[i][j] == 3) {
					full = 0;
					break;
				}
		if (full)
			printf("Draw\n");
		else
			printf("Game has not completed\n");
	}

	return 0;
}

int main()
{
	char *p;
	int num_tests;
	char str[BUFFER_SIZE];

	if (freopen("tic-tac-toe-tomek.in", "rt", stdin) == NULL) {
		fprintf(stderr, "Could not reopen stdin\n");
		return 1;
	}
	if (freopen("tic-tac-toe-tomek.out", "wt", stdout) == NULL) {
		fprintf(stderr, "Could not reopen stdin\n");
		return 1;
	}

	if (fgets(str, sizeof(str), stdin) == NULL)
		return 1;
	p = strtok(str, " \r\n");
	num_tests = atoi(p);
	for (int i = 0; i < num_tests; i++)
		solve_problem(i + 1);

	return 0;
}
