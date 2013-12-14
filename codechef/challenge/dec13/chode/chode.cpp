#include <algorithm>
#include <cstdio>
#include <cctype>
#include <cstring>

#define NUM_CHARACTERS 26
#define BUFFER_SIZE 150000

using namespace std;

char seq[NUM_CHARACTERS + 10];
char str[BUFFER_SIZE + 10];
pair<int, int> freq[NUM_CHARACTERS];
int perm[NUM_CHARACTERS];

char *trim_line(char *str)
{
	return strtok(str, "\r\n");
}

int solve_problem()
{
	if (fgets(seq, sizeof(seq), stdin) == NULL)
		return 1;
	trim_line(seq);

	if (fgets(str, sizeof(str), stdin) == NULL)
		return 1;
	trim_line(str);

	for (int i = 0; i < NUM_CHARACTERS; i++)
		freq[i] = make_pair(0, i);

	size_t len = strlen(str);
	for (size_t i = 0; i < len; i++) {
		char ch = str[i];
		if (isalpha((unsigned char) ch)) {
			int idx = (ch <= 'Z') ? (ch - 'A') : (ch - 'a');
			++freq[idx].first;
		}
	}
	sort(freq, freq + NUM_CHARACTERS);
	for (int i = 0; i < NUM_CHARACTERS; i++)
		perm[freq[i].second] = seq[i] - 'a';

	for (size_t i = 0; i < len; i++) {
		char ch = str[i];
		if (isalpha((unsigned char) ch))
			str[i] = (ch <= 'Z') ? ('A' + perm[ch - 'A']) : ('a' + perm[ch - 'a']);
	}
	printf("%s\n", str);

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
