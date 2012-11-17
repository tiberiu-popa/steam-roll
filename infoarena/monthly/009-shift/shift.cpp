#include <algorithm>
#include <limits>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

using namespace std;

#define MAX_LENGTH 100000
#define NUM_CHARS 26

char line[MAX_LENGTH + 1];
char tape[NUM_CHARS][2];
int weights[NUM_CHARS][2];

void read_input(const char *filename)
{
	FILE *f;
	char ch[2];

	f = fopen(filename, "rt");
	if (f == NULL) {
		fprintf(stderr, "Could not open file: %s\n", filename);
		exit(EXIT_FAILURE);
	}

	if (fscanf(f, "%s", line) != 1)
		exit(EXIT_FAILURE);

	for (int i = 0; i < NUM_CHARS; i++) {
		if (fscanf(f, "%s", ch) != 1)
			exit(EXIT_FAILURE);
		tape[i][0] = ch[0];
		if (fscanf(f, "%s", ch) != 1)
			exit(EXIT_FAILURE);
		tape[i][1] = ch[0];
	}

	for (int i = 0; i < NUM_CHARS; i++)
		if (fscanf(f, "%d %d", &weights[i][0], &weights[i][1]) != 2)
			exit(EXIT_FAILURE);

	fclose(f);
}

void write_output(const char *filename, int result)
{
	FILE *f;

	f = fopen(filename, "wt");
	if (f == NULL) {
		fprintf(stderr, "Could not open file: %s\n", filename);
		exit(EXIT_FAILURE);
	}

	fprintf(f, "%d\n", result);
	fclose(f);
}

int solve_problem()
{
	int reverse_tape[NUM_CHARS][2][2];
	int char_counts[NUM_CHARS];
	int old[2][2] = { { 0, 0 }, { 0, 1 } };
	int cost[2] = { 0, 0 };
	int new_cost[2] = { 0, 0 };

	memset(char_counts, 0, sizeof(char_counts));
	for (int i = 0; i < NUM_CHARS; i++)
		for (int j = 0; j < 2; j++) {
			int ch = tape[i][j] - 'a';
			int pos = char_counts[ch]++;
			reverse_tape[ch][pos][0] = i;
			reverse_tape[ch][pos][1] = j;
		}

	size_t len = strlen(line);
	for (size_t i = 0; i < len; i++) {
		int ch = line[i] - 'a';
		for (int j = 0; j < 2; j++) {
			int *pos = reverse_tape[ch][j];
			int min_value = numeric_limits<int>::max();
			for (int k = 0; k < 2; k++) {
				int candidate = cost[k] + abs(old[k][0] - pos[0]);
				min_value = min(min_value, candidate);
			}
			min_value += weights[pos[0]][pos[1]];
			new_cost[j] = min_value;
		}
		memcpy(cost, new_cost, sizeof(cost));
		memcpy(old, reverse_tape[ch], sizeof(old));
	}

	return min(cost[0], cost[1]);
}

int main()
{
	int result;

	read_input("shift.in");
	result = solve_problem();
	write_output("shift.out", result);

	return 0;
}
