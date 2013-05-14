#include <algorithm>
#include <limits>
#include <vector>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstring>

#define NUM_CHARACTERS 26
#define BUFFER_SIZE 4000
#define MAX_LENGTH ((size_t) 10)
#define WINDOW_SIZE 5

using namespace std;

void update_score(int &score, int new_score)
{
	if (new_score >= 0) {
		if (score < 0 || new_score < score)
			score = new_score;
	}
}

struct trie {

	trie *next[NUM_CHARACTERS];
	bool leaf;

	trie() : leaf(false)
	{
		memset(next, 0, sizeof(next));
	}

	~trie()
	{
		for (int i = 0; i < NUM_CHARACTERS; i++)
			if (next[i] != NULL)
				delete next[i];
	}

	void insert(const char *p)
	{
		char ch = *p;
		if (ch) {
			if (islower(ch)) {
				int pos = ch - 'a';
				if (next[pos] == NULL)
					next[pos] = new trie;
				next[pos]->insert(p + 1);
			}
		} else {
			leaf = true;
		}
	}

	int search(const char *p, const char *end, int num_left)
	{
		if (p == end) {
			return num_left;
		} else {
			if (num_left > 0) {
				int i = *p - 'a';
				if (i >= 0 && i < NUM_CHARACTERS && next[i] != NULL)
					return next[i]->search(p + 1, end, num_left - 1);
				else
					return -1;
			} else {
				int result = -1;
				for (int i = 0; i < NUM_CHARACTERS; i++)
					if (next[i] != NULL) {
						char ch = 'a' + i;
						int cand = next[i]->search(p + 1, end, (ch == *p) ? 0 : WINDOW_SIZE - 1);
						update_score(result, cand);
					}
				return result;
			}
		}
	}

	void search(int *result, const char *p, const char *end, int num_left, int score)
	{
		if (p == end) {
			update_score(result[num_left], score);
		} else {
			if (num_left > 0) {
				int i = *p - 'a';
				if (i >= 0 && i < NUM_CHARACTERS && next[i] != NULL)
					next[i]->search(result, p + 1, end, num_left - 1, score);
			} else {
				for (int i = 0; i < NUM_CHARACTERS; i++)
					if (next[i] != NULL) {
						char ch = 'a' + i;
						bool same_char = (ch == *p);
						int next_num_left = same_char ? 0 : (WINDOW_SIZE - 1);
						int next_score = same_char ? score : (score + 1);
						next[i]->search(result, p + 1, end, next_num_left, next_score);
					}
			}
		}
	}

};

vector<trie *> words(MAX_LENGTH + 1);
int dp[BUFFER_SIZE + 1][WINDOW_SIZE];
char str[BUFFER_SIZE + 10];

char *trim_line(char *str)
{
	return strtok(str, "\r\n");
}

int solve_problem(int num_case)
{
	int best_score[WINDOW_SIZE];

	if (fgets(str, sizeof(str), stdin) == NULL)
		return 1;

	trim_line(str);
	size_t len = strlen(str);

	fill(dp[0], dp[0] + WINDOW_SIZE, 0);

	for (size_t i = 1; i <= len; i++) {
		fill(dp[i], dp[i] + WINDOW_SIZE, -1);
		size_t sup_j = min(MAX_LENGTH, i);
		for (size_t j = 1; j <= sup_j; j++) {
			for (int k = 0; k < WINDOW_SIZE; k++) {
				int prev_score = dp[i - j][k];
				if (prev_score >= 0) {
					fill(best_score, best_score + WINDOW_SIZE, -1);
					words[j]->search(best_score, str + i - j, str + i, k, prev_score);
					for (int l = 0; l < WINDOW_SIZE; l++)
						update_score(dp[i][l], best_score[l]);
				}
			}
		}
	}

	sort(dp[len], dp[len] + WINDOW_SIZE);
	for (int k = 0; k < WINDOW_SIZE; k++)
		if (dp[len][k] >= 0) {
			printf("Case #%d: %d\n", num_case, dp[len][k]);
			break;
		}

	return 0;
}

int process_dictionary(const char *filename)
{
	FILE *f;

	f = fopen(filename, "rt");
	if (f == NULL)
		return 1;

	for (vector<trie *>::iterator it = words.begin(); it != words.end(); ++it)
		*it = new trie;

	while (fgets(str, sizeof(str), f) != NULL) {
		char *p = trim_line(str);
		size_t len = strlen(p);
		words.at(len)->insert(p);
	}

	fclose(f);

	return 0;
}

int main(int argc, char *argv[])
{
	const char *filenames[] = {
		"garbled-email.in",
		"garbled-email.out",
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

	process_dictionary("garbled_email_dictionary.txt");

	if (fgets(str, sizeof(str), stdin) == NULL)
		return 1;
	num_tests = atoi(str);
	for (int i = 0; i < num_tests; i++)
		solve_problem(i + 1);

	return 0;
}
