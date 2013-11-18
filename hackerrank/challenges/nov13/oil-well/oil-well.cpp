#include <algorithm>
#include <limits>
#include <vector>
#include <cstdio>

#define MAX_R 50
#define MAX_C 50

using namespace std;

int grid[MAX_R][MAX_C];
int dp[MAX_R][MAX_R][MAX_C][MAX_C];
vector< pair< int, vector<int> > > x;
vector< pair< int, vector<int> > > y;

int trimax(int a, int b, int c)
{
	return (a > b) ? max(a, c) : max(b, c);
}

int bounding_rectangle(size_t left, size_t right, size_t bottom, size_t top)
{
	if (dp[left][right][bottom][top] != numeric_limits<int>::max())
		return dp[left][right][bottom][top];

	int result = numeric_limits<int>::max();
	int cand;

	const pair< int, vector<int> > &lp = x[left];
	int lx = lp.first;
	const vector<int> &lv = lp.second;

	const pair< int, vector<int> > &rp = x[right];
	int rx = rp.first;
	const vector<int> &rv = rp.second;

	const pair< int, vector<int> > &bp = y[bottom];
	int by = bp.first;
	const vector<int> &bv = bp.second;

	const pair< int, vector<int> > &tp = y[top];
	int ty = tp.first;
	const vector<int> &tv = tp.second;

	if (left < right) {
		cand = 0;
		vector<int>::const_iterator it = lower_bound(lv.begin(), lv.end(), by);
		for (; it != lv.end(); ++it) {
			int cy = *it;
			if (cy > ty)
				break;
			cand += trimax(rx - lx, cy - by, ty - cy);
		}
		cand += bounding_rectangle(left + 1, right, bottom, top);
		if (cand < result)
			result = cand;

		cand = 0;
		it = lower_bound(rv.begin(), rv.end(), by);
		for (; it != rv.end(); ++it) {
			int cy = *it;
			if (cy > ty)
				break;
			cand += trimax(rx - lx, cy - by, ty - cy);
		}
		cand += bounding_rectangle(left, right - 1, bottom, top);
		if (cand < result)
			result = cand;
	}

	if (bottom < top) {
		cand = 0;
		vector<int>::const_iterator it = lower_bound(bv.begin(), bv.end(), lx);
		for (; it != bv.end(); ++it) {
			int cx = *it;
			if (cx > rx)
				break;
			cand += trimax(ty - by, cx - lx, rx - cx);
		}
		cand += bounding_rectangle(left, right, bottom + 1, top);
		if (cand < result)
			result = cand;

		cand = 0;
		it = lower_bound(tv.begin(), tv.end(), lx);
		for (; it != tv.end(); ++it) {
			int cx = *it;
			if (cx > rx)
				break;
			cand += trimax(ty - by, cx - lx, rx - cx);
		}
		cand += bounding_rectangle(left, right, bottom, top - 1);
		if (cand < result)
			result = cand;
	}

	if (result == numeric_limits<int>::max())
		result = 0;

	return (dp[left][right][bottom][top] = result);
}

int solve_problem()
{
	int r, c;

	if (scanf("%d %d", &r, &c) != 2)
		return 1;

	for (int i = 0; i < r; i++)
		for (int j = 0; j < c; j++)
			if (scanf("%d", &grid[i][j]) != 1)
				return 1;

	for (int i = 0; i < r; i++) {
		vector<int> v;
		for (int j = 0; j < c; j++)
			if (grid[i][j] != 0)
				v.push_back(j);
		if (!v.empty())
			x.push_back(make_pair(i, v));
	}

	for (int j = 0; j < c; j++) {
		vector<int> v;
		for (int i = 0; i < r; i++)
			if (grid[i][j] != 0)
				v.push_back(i);
		if (!v.empty())
			y.push_back(make_pair(j, v));
	}

	int result = 0;
	if (!x.empty()) {
		for (size_t i = 0; i < x.size(); i++)
			for (size_t j = i; j < x.size(); j++)
				for (size_t k = 0; k < y.size(); k++)
					for (size_t l = k; l < y.size(); l++)
						dp[i][j][k][l] = numeric_limits<int>::max();
		result = bounding_rectangle(0, x.size() - 1, 0, y.size() - 1);
	}
	printf("%d\n", result);

	return 0;
}

int main()
{
	solve_problem();
	return 0;
}
