#include <algorithm>
#include <limits>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

#define BUFFER_SIZE 100
#define MAX_R 20
#define MAX_C 20

using namespace std;

char str[BUFFER_SIZE + 10];
int grid[MAX_R][MAX_C];
int monster[MAX_R][MAX_C];
int sprague[MAX_R][MAX_C][MAX_R][MAX_C];

char *trim_line(char *str)
{
	return strtok(str, "\r\n");
}

int grundy(int r, int c)
{
	const int xdiff[] = { -1, -1, 1, 1 };
	const int ydiff[] = { -1, 1, 1, -1 };

	for (int i = 0; i < r; i++)
		for (int j = 0; j < c; j++)
			sprague[i][j][i][j] = (monster[i][j] != 0);

	for (int ds = 1; ds < r + c - 1; ds++)
		for (int dx = max(0, ds - c + 1); dx < r && dx <= ds && ds - dx < c; dx++) {
			int dy = ds - dx;
			for (int i = 0; i + dx < r; i++)
				for (int j = 0; j + dy < c; j++) {
					int k = i + dx;
					int l = j + dy;
					if (dx == 0) {
						bool exists = false;
						for (int y = j; y <= l; y++)
							if (monster[i][y]) {
								exists = true;
								break;
							}
						sprague[i][j][k][l] = exists;
					} else if (dy == 0) {
						bool exists = false;
						for (int x = i; x <= k; x++)
							if (monster[x][j]) {
								exists = true;
								break;
							}
						sprague[i][j][k][l] = exists;
					} else {
						int xc[] = { i, i, k, k };
						int yc[] = { j, l, l, j };
						vector<int> numbers;
						for (int x = i; x <= k; x++)
							for (int y = j; y <= l; y++) {
								if (monster[x][y]) {
									int cand = 0;
									for (int t = 0; t < 4; t++) {
										int xs = xc[t];
										int ys = yc[t];
										int xe = x + xdiff[t];
										int ye = y + ydiff[t];
										if (xs > xe)
											swap(xs, xe);
										if (ys > ye)
											swap(ys, ye);
										if (xs >= i && xs <= k && ys >= j && ys <= l &&
												xe >= i && xe <= k && ye >= j && ye <= l) {
											cand ^= sprague[xs][ys][xe][ye];
										}
									}
									numbers.push_back(cand);
								}
							}
						sort(numbers.begin(), numbers.end());
						int value = 0;
						vector<int>::iterator it = numbers.begin();
						for (;; value++) {
							while (it != numbers.end() && *it < value)
								++it;
							if (it == numbers.end() || value != *it)
								break;
						}
						sprague[i][j][k][l] = value;
					}
				}
		}

	return sprague[0][0][r - 1][c - 1] != 0;
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

	for (int i = 2; i + 2 < r; i++) {
		for (int j = 2; j + 2 < c; j++) {
			if (grid[i][j] == 0 && grid[i - 1][j] == 0 && grid[i - 2][j] == 0 &&
					grid[i + 1][j] == 0 && grid[i + 2][j] == 0 &&
					grid[i][j - 1] == 0 && grid[i][j - 2] == 0 &&
					grid[i][j + 1] == 0 && grid[i][j + 2] == 0)
				monster[i - 2][j - 2] = 1;
			else
				monster[i - 2][j - 2] = 0;
		}
	}

	int result = grundy(max(0, r - 4), max(0, c - 4));
	printf("%s\n", result ? "Asuna" : "Kirito");

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
