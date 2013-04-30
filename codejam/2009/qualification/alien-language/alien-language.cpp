#include <bitset>
#include <cstdio>
#include <cstring>

#define MAX_L 15
#define MAX_D 5000

using namespace std;

bitset<MAX_D> masks[MAX_L][26];
char words[MAX_D][MAX_L + 1];

int solve_problem(int num_case, int d)
{
	char word[MAX_L * 28 + 1];
	bool inside_parenthesis = false;

	if (scanf("%s", word) != 1)
		return 1;

	bitset<MAX_D> mask;
	for (int i = 0; i < d; i++)
		mask.set((size_t) i);
	bitset<MAX_D> or_mask;
	size_t len = strlen(word);
	for (size_t i = 0, pos = 0; i < len; i++)
		if (word[i] == '(') {
			or_mask = 0;
			inside_parenthesis = true;
		} else if (word[i] == ')') {
			mask &= or_mask;
			inside_parenthesis = false;
			pos++;
		} else {
			if (inside_parenthesis) {
				or_mask |= masks[pos][word[i] - 'a'];
			} else {
				mask &= masks[pos][word[i] - 'a'];
				pos++;
			}
		}

	printf("Case #%d: %zu\n", num_case, mask.count());
	return 0;
}

int main(int argc, char *argv[])
{
	const char *filenames[] = {
		"alien-language.in",
		"alien-language.out",
	};
	int l, d, num_tests;

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

	if (scanf("%d %d %d", &l, &d, &num_tests) != 3)
		return 1;

	for (int i = 0; i < d; i++)
		if (scanf("%s", words[i]) != 1)
			return 1;

	for (int i = 0; i < d; i++)
		for (int j = 0; j < l; j++)
			masks[j][words[i][j] - 'a'].set((size_t) i);

	for (int i = 0; i < num_tests; i++)
		solve_problem(i + 1, d);

	return 0;
}
