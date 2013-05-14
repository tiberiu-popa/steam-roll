#include <algorithm>
#include <functional>
#include <limits>
#include <vector>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstring>

#define MAX_TESTS 30
#define BUFFER_SIZE 1000
#define NUM_CHARACTERS 26

using namespace std;

pair<int, int> googlerese[NUM_CHARACTERS];
pair<int, int> english[NUM_CHARACTERS];
int reverse_index[NUM_CHARACTERS];
char str[BUFFER_SIZE];

char *trim_line(char *str)
{
	return strtok(str, "\r\n");
}

void update_index(pair<int, int> *index, const char *p, int &num_chars)
{
	if (p != NULL) {
		size_t len = strlen(p);
		for (size_t i = 0; i < len; i++) {
			char ch = p[i];
			if (isalpha(ch)) {
				if (isupper(ch))
					ch = tolower(ch);
				++num_chars;
				if (index[ch - 'a'].first == 0)
					index[ch - 'a'].first = num_chars;
			}
		}
	}
}

int process_corpus(pair<int, int> *index, const char *filename)
{
	FILE *f;
	int num_chars = 0;

	f = fopen(filename, "rt");
	if (f == NULL)
		return 1;

	for (int i = 0; i < NUM_CHARACTERS; i++)
		index[i].second = i;

	while (fgets(str, sizeof(str), f) != NULL) {
		const char *p = trim_line(str);
		update_index(index, p, num_chars);
	}

	sort(index, index + NUM_CHARACTERS, greater< pair<int, int> >());

	fclose(f);

	return 0;
}

int solve_problem(int num_case)
{
	if (fgets(str, sizeof(str), stdin) == NULL)
		return 1;

	trim_line(str);
	size_t len = strlen(str);
	for (size_t j = 0; j < len; j++) {
		char ch = str[j];
		if (islower(ch)) {
			if (ch == 'z' || ch == 'q')
				str[j] ^= 'z' ^ 'q';
			else
				str[j] = 'a' + english[reverse_index[ch - 'a']].second;
		}
	}
	printf("Case #%d: %s\n", num_case, str);

	return 0;
}

int main(int argc, char *argv[])
{
	const char *filenames[] = {
		"speaking-tongues.in",
		"speaking-tongues.out",
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

	process_corpus(googlerese, "problem.gog");
	process_corpus(english, "problem.eng");

	for (int i = 0; i < NUM_CHARACTERS; i++)
		reverse_index[googlerese[i].second] = i;

	if (fgets(str, sizeof(str), stdin) == NULL)
		return 1;
	num_tests = atoi(str);
	for (int i = 0; i < num_tests; i++) {
		int ret = solve_problem(i + 1);
		if (ret != 0)
			return ret;
	}

	return 0;
}
