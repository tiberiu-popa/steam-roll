#include <algorithm>
#include <map>
#include <cmath>
#include <cstdio>
#include <cstring>

#define BUFFER_SIZE 40000

using namespace std;

char str[BUFFER_SIZE + 1];

void add_string(int *x, const char *s)
{
	size_t len = strlen(s);
	for (size_t i = 0; i < len; i++)
		x[s[i] - 'a']++;
}

int solve_problem()
{
	int n;
	int x[26], y[26];

	memset(x, 0, sizeof(x));
	memset(y, 0, sizeof(y));

	if (scanf("%s", str) != 1)
		return 1;
	add_string(x, str);

	if (scanf("%s", str) != 1)
		return 1;
	add_string(x, str);

	if (scanf("%d", &n) != 1)
		return 1;

	for (int i = 0; i < n; i++) {
		if (scanf("%s", str) != 1)
			return 1;
		add_string(y, str);
	}

	bool result = true;
	for (int i = 0; i < 26; i++)
		if (x[i] < y[i]) {
			result = false;
			break;
		}

	printf("%s\n", result ? "YES" : "NO");
	return 0;
}

int main()
{
	int num_tests;

	if (scanf("%d", &num_tests) != 1)
		return 1;
	for (int i = 0; i < num_tests; i++) {
		int ret = solve_problem();
		if (ret != 0) {
			fprintf(stderr, "Read error at case #%d\n", i + 1);
			break;
		}
	}

	return 0;
}
