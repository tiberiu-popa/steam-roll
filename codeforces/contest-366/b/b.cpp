#include <limits>
#include <cstdio>

#define MAX_N 100000

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

	int min_s = numeric_limits<int>::max();
	int result = -1;
	for (int i = 0; i < k; i++) {
		int s = 0;
		for (int j = i; j < n; j += k)
			s += v[j];
		if (s < min_s) {
			min_s = s;
			result = i;
		}
	}

	printf("%d\n", result + 1);

	return 0;
}

int main()
{
	solve_problem();
	return 0;
}
