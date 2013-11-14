#include <algorithm>
#include <limits>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#define MAX_M 3000

using namespace std;

int d[MAX_M];

int solve_problem()
{
	int n, m;

	if (scanf("%d %d", &n, &m) != 2)
		return 1;

	for (int i = 0; i < m; i++)
		if (scanf("%d", &d[i]) != 1)
			return 1;

	sort(d, d + m);

	int result = (m == 0);
	if (!result && d[0] > 1 && d[m - 1] < n) {
		int longest_run = 0;
		int run = 0;
		for (int i = 0; i < m; i++) {
			if (i == 0 || d[i - 1] + 1 == d[i]) {
				++run;
			} else {
				run = 1;
			}
			if (run > longest_run)
				longest_run = run;
		}
		result = (longest_run <= 2);
	}

	printf("%s\n", result ? "YES" : "NO");

	return 0;
}

int main()
{
	solve_problem();
	return 0;
}
