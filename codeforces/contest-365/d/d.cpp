#include <algorithm>
#include <vector>
#include <cstdio>

#define MAX_N 50
#define MAX_VAL 10000

using namespace std;

int p[MAX_N * MAX_VAL + 1];
int v[MAX_N];

int solve_problem()
{
	vector<int> positions;
	int n, d;

	if (scanf("%d %d", &n, &d) != 2)
		return 1;

	for (int i = 0; i < n; i++)
		if (scanf("%d", &v[i]) != 1)
			return 1;

	int s = 0;
	for (int i = 0; i < n; i++)
		s += v[i];

	sort(v, v + n);

	p[0] = 1;
	for (int i = 0; i < n; i++)
		for (int j = s; j >= 0; j--)
			if (p[j])
				p[j + v[i]] = 1;

	for (int i = 0; i <= s; i++)
		if (p[i])
			positions.push_back(i);

	int t = 0;
	int accum = 0;
	while (true) {
		vector<int>::iterator it = lower_bound(positions.begin(), positions.end(), accum + d);
		if (it != positions.begin()) {
			if (it == positions.end() || *it > accum + d)
				--it;
		}
		if (*it <= accum)
			break;
		accum = *it;
		++t;
	}

	printf("%d %d\n", accum, t);

	return 0;
}

int main()
{
	solve_problem();
	return 0;
}
