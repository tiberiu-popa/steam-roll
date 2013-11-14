#include <limits>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#define GRID_SIZE 8
#define BUFFER_SIZE 8

using namespace std;

int grid[GRID_SIZE][GRID_SIZE];
char str[BUFFER_SIZE + 10];

int solve_problem()
{
	vector< pair<int, int> > knights;
	int result = 0;
	int x, y;
	int i, j;

	for (i = 0; i < GRID_SIZE; i++) {
		if (fgets(str, sizeof(str), stdin) == NULL)
			return 1;
		char *p = strtok(str, " \r\n");
		for (j = 0; j < GRID_SIZE; j++) {
			grid[i][j] = (p[j] == '#');
			if (p[j] == 'K')
				knights.push_back(make_pair(i, j));
		}
	}

	if (knights[0].first % 4 == knights[1].first % 4 && knights[0].second % 4 == knights[1].second % 4) {
		x = knights[0].first % 4;
		y = knights[0].second % 4;
		for (i = x; i < GRID_SIZE; i += 4)
			for (j = y; j < GRID_SIZE; j += 4)
				if (!grid[i][j])
					result = 1;
		for (i = x ^ 2; i < GRID_SIZE; i += 4)
			for (j = y ^ 2; j < GRID_SIZE; j += 4)
				if (!grid[i][j])
					result = 1;
	}

	printf("%s\n", result ? "YES" : "NO");

	if (fgets(str, sizeof(str), stdin) == NULL)
		return 1;

	return 0;
}

int main()
{
	int num_tests;

	if (fgets(str, sizeof(str), stdin) == NULL)
		return 1;

	num_tests = atoi(str);
	for (int i = 0; i < num_tests; i++)
		solve_problem();

	return 0;
}
