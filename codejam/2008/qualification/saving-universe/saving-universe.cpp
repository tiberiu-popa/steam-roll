#include <algorithm>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <cmath>
#include <cstdio>
#include <cstring>

#define BUFFER_SIZE 100
#define MAX_S 100
#define MAX_Q 1000

using namespace std;

int dp[MAX_S];

char *trim_line(char *str)
{
	return strtok(str, "\r\n");
}

int solve_problem(int num_case)
{
	char str[BUFFER_SIZE + 5];
	int s, q;
	int last_index = -1;
	map<string, int> engine_map;
	vector<string> engines;
	vector<string> queries;
	multiset<int> values;

	if (fgets(str, sizeof(str), stdin) == NULL)
		return 1;
	s = atoi(str);

	for (int i = 0; i < s; i++) {
		if (fgets(str, sizeof(str), stdin) == NULL)
			return 1;
		char *p = trim_line(str);
		engine_map[p] = i;
		engines.push_back(p);
	}

	if (fgets(str, sizeof(str), stdin) == NULL)
		return 1;
	q = atoi(str);

	for (int i = 0; i < q; i++) {
		if (fgets(str, sizeof(str), stdin) == NULL)
			return 1;
		queries.push_back(trim_line(str));
	}

	fill(dp, dp + s, 0);
	for (int i = 0; i < s; i++)
		values.insert(0);
	for (int i = 0; i < q; i++) {
		map<string, int>::iterator it = engine_map.find(queries[(size_t) i]);
		int index = -1;
		if (it != engine_map.end()) {
			index = it->second;
		}
		if (last_index >= 0 && index != last_index) {
			int last_value = *values.begin() + 1;
			dp[last_index] = last_value;
			values.insert(last_value);
		}
		if (index >= 0) {
			if (dp[index] >= 0) {
				values.erase(values.find(dp[index]));
				dp[index] = -1;
			}
		}
		last_index = index;
	}

	printf("Case #%d: %d\n", num_case, *values.begin());
	return 0;
}

int main(int argc, char *argv[])
{
	const char *filenames[] = {
		"saving-universe.in",
		"saving-universe.out",
	};
	int num_tests;
	char str[BUFFER_SIZE + 5];

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

	if (fgets(str, sizeof(str), stdin) == NULL)
		return 1;
	num_tests = atoi(str);
	for (int i = 0; i < num_tests; i++)
		solve_problem(i + 1);

	return 0;
}
