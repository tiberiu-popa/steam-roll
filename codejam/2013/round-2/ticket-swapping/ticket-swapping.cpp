#include <algorithm>
#include <limits>
#include <vector>
#include <cmath>
#include <cstdio>
#include <cstring>

#define MAX_M 1000
#define MOD_NUMBER 1000002013

using namespace std;

struct entry {

	int t, p;
	bool out;

	entry(int t, int p, bool out) : t(t), p(p), out(out) { }

	bool operator< (const entry &rhs) const
	{
		if (t != rhs.t)
			return t < rhs.t;
		if (out != rhs.out)
			return out < rhs.out;
		return p < rhs.p;
	}

};

int addmod(int x, int y)
{
	int sum = x + y;
	if (sum >= MOD_NUMBER)
		sum -= MOD_NUMBER;
	return sum;
}

int submod(int x, int y)
{
	int diff = x - y;
	if (diff < 0)
		diff += MOD_NUMBER;
	return diff;
}

int mulmod(int x, int y)
{
	long long prod = x;
	prod = (prod * y) % MOD_NUMBER;
	return (int) prod;
}

int nC2mod(int n)
{
	if (n & 1)
		return mulmod((n - 1) / 2, n);
	else
		return mulmod(n - 1, n / 2);
}

int solve_problem(int num_case)
{
	int n, m;

	if (scanf("%d %d", &n, &m) != 2)
		return 1;

	vector< pair<int, int> > o, e;
	vector<entry> v;
	for (int i = 0; i < m; i++) {
		int x, y, z;
		if (scanf("%d %d %d", &x, &y, &z) != 3)
			return 1;
		o.push_back(make_pair(x, z));
		e.push_back(make_pair(y, z));
		v.push_back(entry(x, z, false));
		v.push_back(entry(y, z, true));
	}
	sort(v.begin(), v.end());

	int baseline = 0;
	for (size_t i = 0; i < o.size(); i++) {
		int dt = e[i].first - o[i].first;
		if (dt > 0) {
			int diff = submod(mulmod(dt, n), nC2mod(dt));
			baseline = addmod(baseline, mulmod(e[i].second, diff));
		}
	}

	int result = 0;
	vector< pair<int, int> > u;
	for (size_t i = 0; i < v.size(); i++) {
		if (!v[i].out) {
			u.push_back(make_pair(v[i].t, v[i].p));
		} else {
			int p = v[i].p;
			while (p > 0) {
				pair<int, int> last = u.back();
				int num_cards;
				if (p < last.second) {
					num_cards = p;
					u[u.size() - 1].second -= p;
					p = 0;
				} else {
					num_cards = last.second;
					p -= last.second;
					u.pop_back();
				}
				int t1 = last.first;
				int t2 = v[i].t;
				int dt = t2 - t1;
				if (dt > 0) {
					int diff = submod(mulmod(dt, n), nC2mod(dt));
					result = addmod(result, mulmod(num_cards, diff));
				}
			}
		}
	}
	result = submod(baseline, result);

	printf("Case #%d: %d\n", num_case, result);

	return 0;
}

int main(int argc, char *argv[])
{
	const char *filenames[] = {
		"ticket-swapping.in",
		"ticket-swapping.out",
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
