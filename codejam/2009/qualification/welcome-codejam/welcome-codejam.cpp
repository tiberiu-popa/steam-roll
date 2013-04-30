#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>

#define BUFFER_SIZE 500
#define BANNER_SIZE 19
#define MOD_NUMBER 10000

using namespace std;

const char *banner = "welcome to code jam";
int dp[BUFFER_SIZE][BANNER_SIZE + 1];

char *trim_line(char *str)
{
	return strtok(str, "\r\n");
}

int addmod(int x, int y)
{
	int sum = x + y;
	if (sum >= MOD_NUMBER)
		sum -= MOD_NUMBER;
	return sum;
}

int solve_problem(int num_case)
{
	char str[BUFFER_SIZE + 10];

	if (fgets(str, sizeof(str), stdin) == NULL)
		return 1;
	char *p = trim_line(str);
	size_t len = strlen(p);

	fill(dp[0], dp[0] + BANNER_SIZE + 1, 0);
	dp[0][0] = 1;
	dp[0][1] = (p[0] == banner[0]);
	for (size_t i = 1; i < len; i++)
		for (int j = 0; j <= BANNER_SIZE; j++) {
			dp[i][j] = dp[i - 1][j];
			if (j > 0 && p[i] == banner[j - 1])
				dp[i][j] = addmod(dp[i][j], dp[i - 1][j - 1]);
		}

	printf("Case #%d: %04d\n", num_case, dp[len - 1][BANNER_SIZE]);

	return 0;
}

int main(int argc, char *argv[])
{
	const char *filenames[] = {
		"welcome-codejam.in",
		"welcome-codejam.out",
	};
	char str[BUFFER_SIZE + 10];
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

	if (fgets(str, sizeof(str), stdin) == NULL)
		return 1;
	num_tests = atoi(str);
	for (int i = 0; i < num_tests; i++)
		solve_problem(i + 1);

	return 0;
}
