#include <algorithm>
#include <set>
#include <vector>
#include <cmath>
#include <cstdio>
#include <cstring>

#define BUFFER_SIZE 100
#define NUM_CHARS 26

using namespace std;

char grid[NUM_CHARS][NUM_CHARS];

int solve_problem(int num_case)
{
	int c, d, n;
	char str[BUFFER_SIZE + 1];
	vector<char> v;
	multiset<char> used;
	set<char> opposed[NUM_CHARS];

	memset(grid, 0, sizeof(grid));

	if (scanf("%d", &c) != 1)
		return 1;

	for (int i = 0; i < c; i++) {
		if (scanf("%s", str) != 1)
			return 1;
		for (int j = 0; j < 2; j++)
			grid[str[j] - 'A'][str[j ^ 1] - 'A'] = str[2];
	}

	if (scanf("%d", &d) != 1)
		return 1;

	for (int i = 0; i < d; i++) {
		if (scanf("%s", str) != 1)
			return 1;
		for (int j = 0; j < 2; j++)
			opposed[str[j] - 'A'].insert(str[j ^ 1]);
	}

	if (scanf("%d", &n) != 1)
		return 1;

	if (scanf("%s", str) != 1)
		return 1;

	for (int i = 0; i < n; i++) {
		char new_ch = 0;
		if (!v.empty()) {
			new_ch = grid[v.back() - 'A'][str[i] - 'A'];
		}
		if (new_ch) {
			pair< multiset<char>::iterator, multiset<char>::iterator > ip = used.equal_range(v.back());
			used.erase(ip.first);
			v.pop_back();
			v.push_back(new_ch);
			used.insert(new_ch);
		} else {
			set<char> op_chars = opposed[str[i] - 'A'];
			bool found = false;
			for (set<char>::iterator it = op_chars.begin(); it != op_chars.end(); ++it)
				if (used.find(*it) != used.end()) {
					found = true;
					break;
				}
			if (found) {
				v.clear();
				used.clear();
			} else {
				v.push_back(str[i]);
				used.insert(str[i]);
			}
		}
	}

	printf("Case #%d: ", num_case);
	printf("[");
	for (vector<char>::iterator it = v.begin(); it != v.end(); ++it) {
		if (it != v.begin())
			printf(", ");
		printf("%c", *it);
	}
	printf("]\n");

	return 0;
}

int main(int argc, char *argv[])
{
	const char *filenames[] = {
		"magicka.in",
		"magicka.out",
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
