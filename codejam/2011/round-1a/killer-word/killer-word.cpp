#include <algorithm>
#include <map>
#include <set>
#include <vector>
#include <cmath>
#include <cstdio>
#include <cstring>

#define BUFFER_SIZE 10000
#define MAX_N 10000
#define NUM_CHARS 26

using namespace std;

char str[BUFFER_SIZE + 1];
char dictionary[MAX_N][BUFFER_SIZE + 1];
map< size_t, vector<int> > len_strings;
vector<size_t> positions[MAX_N][NUM_CHARS];

struct result_comparator {

	bool operator() (pair<int, int> left, pair<int, int> right)
	{
		if (left.first != right.first)
			return left.first > right.first;
		return left.second < right.second;
	}

};

result_comparator rcomp;

pair<int, int> solve_subcase(const vector<int> &indices, const char *str, size_t len)
{
	if (len == 0 || indices.size() == 1) {
		return make_pair(0, indices.front());
	}

	map< vector<size_t>, vector<int> > pos_map;
	pair<int, int> result(-1, 0);
	char ch = *str;

	for (vector<int>::const_iterator it = indices.begin(); it != indices.end(); ++it)
		pos_map[positions[*it][ch - 'a']].push_back(*it);

	for (map< vector<size_t>, vector<int> >::iterator it = pos_map.begin(); it != pos_map.end(); ++it) {
		pair<int, int> cand = solve_subcase(it->second, str + 1, len - 1);
		if (pos_map.size() > 1 && it->first.empty())
			++cand.first;
		if (rcomp(cand, result))
			result = cand;
	}

	return result;
}

int solve_problem(int num_case)
{
	int n, m;

	if (scanf("%d %d", &n, &m) != 2)
		return 1;

	len_strings.clear();
	for (int i = 0; i < n; i++)
		for (int j = 0; j < 26; j++)
			positions[i][j].clear();

	for (int i = 0; i < n; i++) {
		if (scanf("%s", dictionary[i]) != 1)
			return 1;
		size_t len = strlen(dictionary[i]);
		len_strings[len].push_back(i);
		for (size_t j = 0; j < len; j++)
			positions[i][dictionary[i][j] - 'a'].push_back(j);
	}

	printf("Case #%d:", num_case);
	for (int i = 0; i < m; i++) {
		if (scanf("%s", str) != 1)
			return 1;
		pair<int, int> result(-1, 0);
		for (map< size_t, vector<int> >::iterator it = len_strings.begin(); it != len_strings.end(); ++it) {
			pair<int, int> cand = solve_subcase(it->second, str, strlen(str));
			if (rcomp(cand, result))
				result = cand;
		}
		printf(" %s", dictionary[result.second]);
	}
	printf("\n");

	return 0;
}

int main(int argc, char *argv[])
{
	const char *filenames[] = {
		"killer-word.in",
		"killer-word.out",
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
