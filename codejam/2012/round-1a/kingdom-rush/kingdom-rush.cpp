#include <algorithm>
#include <functional>
#include <set>
#include <cmath>
#include <cstdio>
#include <cstring>

#define MAX_N 1000

using namespace std;

int a[MAX_N];
int b[MAX_N];
bool v[MAX_N];

int solve_problem(int num_case)
{
	set< pair<int, int>, greater< pair<int, int> > > one_star;
	set< pair<int, int> > two_stars;
	int n;

	if (scanf("%d", &n) != 1)
		return 1;

	for (int i = 0; i < n; i++)
		if (scanf("%d %d", &a[i], &b[i]) != 2)
			return 1;

	fill(v, v + n, false);
	for (int i = 0; i < n; i++) {
		one_star.insert(make_pair(a[i], i));
		two_stars.insert(make_pair(b[i], i));
	}

	int result = 0;
	int num_stars = 0;
	while (true) {
		while (!two_stars.empty()) {
			set< pair<int, int> >::iterator it = two_stars.begin();
			if (it->first <= num_stars) {
				int i = it->second;
				if (v[i]) {
					num_stars++;
				} else {
					one_star.erase(make_pair(a[i], i));
					num_stars += 2;
				}
				two_stars.erase(it);
				++result;
			} else {
				break;
			}
		}

		set< pair<int, int> >::iterator u_it = one_star.upper_bound(make_pair(num_stars, n));
		if (u_it != one_star.end()) {
			int index = -1;
			for (; u_it != one_star.end(); ++u_it) {
				int i = u_it->second;
				if (index < 0 || b[i] > b[index])
					index = i;
			}

			v[index] = true;
			one_star.erase(make_pair(a[index], index));
			num_stars++;
			++result;
		} else {
			break;
		}
	}
	if (one_star.empty() && two_stars.empty())
		printf("Case #%d: %d\n", num_case, result);
	else
		printf("Case #%d: Too Bad\n", num_case);

	return 0;
}

int main(int argc, char *argv[])
{
	const char *filenames[] = {
		"kingdom-rush.in",
		"kingdom-rush.out",
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
