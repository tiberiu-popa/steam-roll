#include <algorithm>
#include <vector>
#include <cstdio>

#define MAX_N 100

using namespace std;

int m[MAX_N];
float r[MAX_N];
int c[MAX_N];

int solve_problem()
{
	float d;
	int u, n;

	if (scanf("%f %d %d", &d, &u, &n) != 3)
		return 1;

	for (int i = 0; i < n; i++)
		if (scanf("%d %f %d", &m[i], &r[i], &c[i]) != 3)
			return 1;

	vector< pair<float, int> > v;
	v.push_back(make_pair(u * d, 0));
	for (int i = 0; i < n; i++)
		v.push_back(make_pair((float) c[i] / m[i] + u * r[i], i + 1));

	int result = min_element(v.begin(), v.end())->second;
	printf("%d\n", result);

	return 0;
}

int main()
{
	int num_tests;

	if (scanf("%d", &num_tests) != 1)
		return 1;

	for (int i = 0; i < num_tests; i++)
		solve_problem();

	return 0;
}
