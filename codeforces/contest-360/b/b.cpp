#include <algorithm>
#include <vector>
#include <cstdio>

#define MAX_N 2000

using namespace std;

struct comparator {

	bool operator() (const pair<long long, long long> &lhs, const pair<long long, long long> &rhs) const
	{
		if (lhs.first != rhs.first)
			return lhs.first < rhs.first;
		else
			return lhs.second > rhs.second;
	}

};

int v[MAX_N];
pair<long long, long long> u[MAX_N];
vector<int> m;

long long mul_ll(long long x, long long y)
{
	return x * y;
}

bool check_diff(int n, int k, int c)
{
	for (int i = 0; i < n; i++)
		u[i] = make_pair(v[i] + mul_ll(i, c), v[i] - mul_ll(i, c));
	sort(u, u + n, comparator());

	m.clear();
	for (int i = 0; i < n; i++) {
		size_t left = 0;
		size_t right = m.size();
		while (left < right) {
			size_t mid = left + (right - left) / 2;
			if (u[m[mid]].second < u[i].second)
				right = mid;
			else
				left = mid + 1;
		}

		if (left == m.size())
			m.push_back(i);
		else
			m[left] = i;
	}

	return int(m.size()) + k >= n;
}

int solve_problem()
{
	int n, k;

	if (scanf("%d %d", &n, &k) != 2)
		return 1;

	for (int i = 0; i < n; i++)
		if (scanf("%d", &v[i]) != 1)
			return 1;

	int left = 0;
	int right = *max_element(v, v + n) - *min_element(v, v + n);
	while (left < right) {
		int mid = left + (right - left) / 2;
		if (check_diff(n, k, mid))
			right = mid;
		else
			left = mid + 1;
	}
	printf("%d\n", left);

	return 0;
}

int main()
{
	solve_problem();
	return 0;
}
