#include <cstdio>

#define MAX_N 150000

using namespace std;

int v[MAX_N];

int solve_problem()
{
	int n, k;

	if (scanf("%d %d", &n, &k) != 2)
		return 1;

	for (int i = 0; i < n; i++)
		if (scanf("%d", &v[i]) != 1)
			return 1;

	int s = 0;
	for (int i = 0; i < k; i++)
		s += v[i];

	int min_value = s;
	int idx = 0;

	for (int i = 1; i + k <= n; i++) {
		s += v[i + k - 1] - v[i - 1];
		if (s < min_value) {
			min_value = s;
			idx = i;
		}
	}

	printf("%d\n", idx + 1);

	return 0;
}

int main()
{
	solve_problem();
	return 0;
}
