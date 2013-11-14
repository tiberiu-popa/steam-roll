#include <algorithm>
#include <limits>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#define MAX_N 5000

using namespace std;

int a[MAX_N];
int right[MAX_N][MAX_N];

int solve_problem()
{
	int n;

	if (scanf("%d", &n) != 1)
		return 1;

	for (int i = 0; i < n; i++)
		if (scanf("%d", &a[i]) != 1)
			return 1;

	for (int i = 0; i < n; i++)
		for (int j = i - 1; j >= 0; j--)
			right[j][i] = right[j + 1][i] + (a[j] < a[i]);

	int longest_run = 0;
	int mult = 0;
	for (int i = 0; i < n; i++) {
		int cnt = 0;
		for (int j = i + 1; j < n; j++) {
			if (a[i] > a[j]) {
				int run = 2 * (cnt - right[i][j]) + 1;
				if (run > longest_run) {
					longest_run = run;
					mult = 1;
				} else if (run == longest_run) {
					++mult;
				}
			}
			cnt += (a[j] < a[i]);
		}
	}

	for (int i = 1; i < n; i++) {
		int x = a[i];
		int j;
		for (j = i; j > 0 && x < a[j - 1]; j--) {
			a[j] = a[j - 1];
			--longest_run;
		}
		a[j] = x;
	}

	printf("%d %d\n", -longest_run, mult);

	return 0;
}

int main()
{
	solve_problem();
	return 0;
}
