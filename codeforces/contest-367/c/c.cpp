#include <algorithm>
#include <functional>
#include <iostream>
#include <cstdio>

#define MAX_M 100000

using namespace std;

int v[MAX_M];

long long euler_decomposition(long long n)
{
	if (n & 1)
		return n * (n - 1) / 2 + 1;
	else
		return n * n / 2;
}

int solve_problem()
{
	int n, m;

	if (scanf("%d %d", &n, &m) != 2)
		return 1;

	for (int i = 0; i < m; i++) {
		int q;
		if (scanf("%d %d", &q, &v[i]) != 2)
			return 1;
	}

	sort(v, v + m, greater<int>());

	int left = 0;
	int right = m;
	while (left < right) {
		int mid = left + (right - left + 1) / 2;
		if (euler_decomposition(mid) <= n)
			left = mid;
		else
			right = mid - 1;
	}

	long long result = 0;
	for (int i = 0; i < left; i++)
		result += v[i];
	cout << result << endl;

	return 0;
}

int main()
{
	solve_problem();
	return 0;
}
