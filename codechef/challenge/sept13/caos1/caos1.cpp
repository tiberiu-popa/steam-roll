#include <algorithm>
#include <limits>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

#define BUFFER_SIZE 100
#define MAX_R 50
#define MAX_C 50

using namespace std;

char str[BUFFER_SIZE + 10];
int grid[MAX_R][MAX_C];
int monster[MAX_R][MAX_C];

char *trim_line(char *str)
{
	return strtok(str, "\r\n");
}

int solve_problem()
{
	int r, c;

	if (fgets(str, sizeof(str), stdin) == NULL)
		return 1;
	trim_line(str);

	char *p = strtok(str, " ");
	r = atoi(p);
	p = strtok(NULL, " ");
	c = atoi(p);

	for (int i = 0; i < r; i++) {
		if (fgets(str, sizeof(str), stdin) == NULL)
			return 1;
		trim_line(str);
		for (int j = 0; j < c; j++)
			grid[i][j] = (str[j] == '#');
	}

	int result = 0;
	for (int i = 2; i + 2 < r; i++) {
		for (int j = 2; j + 2 < c; j++) {
			if (grid[i][j] == 0 && grid[i - 1][j] == 0 && grid[i - 2][j] == 0 &&
					grid[i + 1][j] == 0 && grid[i + 2][j] == 0 &&
					grid[i][j - 1] == 0 && grid[i][j - 2] == 0 &&
					grid[i][j + 1] == 0 && grid[i][j + 2] == 0)
				result++;
		}
	}
	printf("%d\n", result);

	return 0;
}

int main()
{
	int num_tests;

	if (fgets(str, sizeof(str), stdin) == NULL)
		return 1;
	trim_line(str);

	num_tests = atoi(str);
	for (int i = 0; i < num_tests; i++)
		solve_problem();

	return 0;
}
