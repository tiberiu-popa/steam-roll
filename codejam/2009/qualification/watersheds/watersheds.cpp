#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>

#define MAX_H 100
#define MAX_W 100

using namespace std;

int altitudes[MAX_H][MAX_W];
int edge[MAX_H][MAX_W];
char watersheds[MAX_H][MAX_W];
const int dz[][2] = {
	{ -1, 0 },
	{ 0, -1 },
	{ 0, 1 },
	{ 1, 0 },
};

int solve_problem(int num_case)
{
	int h, w;

	if (scanf("%d %d", &h, &w) != 2)
		return 1;

	for (int i = 0; i < h; i++)
		for (int j = 0; j < w; j++)
			if (scanf("%d", &altitudes[i][j]) != 1)
				return 1;

	for (int i = 0; i < h; i++)
		for (int j = 0; j < w; j++) {
			int min_value = altitudes[i][j];
			int min_index = -1;
			for (int k = 0; k < 4; k++) {
				int x = i + dz[k][0];
				int y = j + dz[k][1];
				if (x >= 0 && x < h && y >= 0 && y < w) {
					if (altitudes[x][y] < min_value) {
						min_value = altitudes[x][y];
						min_index = k;
					}
				}
			}
			edge[i][j] = min_index;
		}

	for (int i = 0; i < h; i++)
		memset(watersheds[i], 0, ((size_t) w) * sizeof(char));

	char ch = 'a';
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++)
			if (!watersheds[i][j]) {
				int x = i;
				int y = j;
				while (!watersheds[x][y]) {
					watersheds[x][y] = ch;
					int dir = edge[x][y];
					if (dir >= 0) {
						x += dz[dir][0];
						y += dz[dir][1];
					} else {
						break;
					}
				}
				char new_ch = watersheds[x][y];
				if (new_ch != ch) {
					x = i;
					y = j;
					while (watersheds[x][y] != new_ch) {
						watersheds[x][y] = new_ch;
						int dir = edge[x][y];
						x += dz[dir][0];
						y += dz[dir][1];
					}
				} else {
					ch++;
				}
			}
	}

	printf("Case #%d:\n", num_case);
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			if (j > 0)
				printf(" %c", watersheds[i][j]);
			else
				printf("%c", watersheds[i][j]);
		}
		printf("\n");
	}

	return 0;
}

int main(int argc, char *argv[])
{
	const char *filenames[] = {
		"watersheds.in",
		"watersheds.out",
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
