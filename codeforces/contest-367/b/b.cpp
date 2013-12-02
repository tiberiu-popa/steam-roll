#include <algorithm>
#include <map>
#include <vector>
#include <cstdio>
#include <cstring>

#define MAX_N 200000

using namespace std;

int a[MAX_N];
int b[MAX_N];
int orig[MAX_N];
int cnt[MAX_N];

long long mul_ll(long long x, long long y)
{
	return x * y;
}

int solve_problem()
{
	int n, m, p;

	if (scanf("%d %d %d", &n, &m, &p) != 3)
		return 1;

	for (int i = 0; i < n; i++)
		if (scanf("%d", &a[i]) != 1)
			return 1;

	for (int i = 0; i < m; i++)
		if (scanf("%d", &b[i]) != 1)
			return 1;

	map<int, size_t> positions;
	for (int i = 0; i < m; i++) {
		int x = b[i];
		if (positions.find(x) == positions.end()) {
			size_t sz = positions.size();
			positions[x] = sz;
		}
		++orig[positions[x]];
	}

	vector<int> results;
	for (int i = 0; i < p; i++) {
		if (i + mul_ll(m - 1, p) >= n)
			break;
		size_t num_good = 0;
		memset(cnt, 0, positions.size() * sizeof(*cnt));
		for (int j = i; j < n; j += p) {
			map<int, size_t>::iterator it = positions.find(a[j]);
			if (it != positions.end()) {
				size_t k = it->second;
				if (cnt[k] == orig[k])
					--num_good;
				++cnt[k];
				if (cnt[k] == orig[k])
					++num_good;
			}
			if (j >= i + m * p) {
				it = positions.find(a[j - m * p]);
				if (it != positions.end()) {
					size_t k = it->second;
					if (cnt[k] == orig[k])
						--num_good;
					--cnt[k];
					if (cnt[k] == orig[k])
						++num_good;
				}
			}
			if (j >= i + (m - 1) * p) {
				if (num_good == positions.size())
					results.push_back(j - (m - 1) * p + 1);
			}
		}
	}

	printf("%ld\n", (unsigned long) results.size());
	if (!results.empty() || true) {
		sort(results.begin(), results.end());
		for (size_t i = 0; i < results.size(); i++)
			if (i > 0)
				printf(" %d", results[i]);
			else
				printf("%d", results[i]);
		printf("\n");
	}

	return 0;
}

int main()
{
	solve_problem();
	return 0;
}
