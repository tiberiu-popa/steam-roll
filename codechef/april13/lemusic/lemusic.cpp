#include <algorithm>
#include <map>
#include <cmath>
#include <cstdio>
#include <cstring>

#define MAX_N 100000

using namespace std;

int b[MAX_N];
int l[MAX_N];
int v[MAX_N];

void solve_problem()
{
	map<int, int> smallest;
	int n;

	if (scanf("%d", &n) != 1)
		return;

	long long sum = 0;
	for (int i = 0; i < n; i++) {
		if (scanf("%d %d", &b[i], &l[i]) != 2)
			return;
		map<int, int>::iterator it = smallest.find(b[i]);
		if (it == smallest.end()) {
			smallest[b[i]] = l[i];
		} else {
			if (l[i] < it->second) {
				sum += it->second;
				smallest[b[i]] = l[i];
			} else {
				sum += l[i];
			}
		}
	}
	sum *= (long long) smallest.size();

	int num_elems = 0;
	for (map<int, int>::iterator it = smallest.begin(); it != smallest.end(); ++it)
		v[num_elems++] = it->second;
	sort(v, v + num_elems);
	for (int i = 0; i < num_elems; i++) {
		long long prod = i + 1;
		sum += prod * v[i];
	}

	printf("%lld\n", sum);
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
