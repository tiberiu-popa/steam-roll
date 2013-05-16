#include <algorithm>
#include <functional>
#include <limits>
#include <set>
#include <vector>
#include <cmath>
#include <cstdio>
#include <cstring>

#define MAX_N 50
#define CAR_LENGTH 5

using namespace std;

char lane[MAX_N][10];
int s[MAX_N];
int p[MAX_N];
int start_lane[MAX_N];
int num_collisions[MAX_N];

set<int> edges[MAX_N];
bool visited[MAX_N];
int level[MAX_N];
int lane_parity = -1;

struct collision {

	double t;
	int x, y;
	bool start;

	collision(double t, int x, int y, bool start) : t(t), x(x), y(y), start(start) { }

	bool operator< (const collision &rhs) const
	{
		if (t != rhs.t)
			return t < rhs.t;
		return start < rhs.start;
	}

};

bool dfs(int root)
{
	visited[root] = true;
	if (start_lane[root] >= 0) {
		if (lane_parity < 0) {
			lane_parity = (start_lane[root] + level[root]) & 1;
		} else {
			int p = (start_lane[root] + level[root]) & 1;
			if (p != lane_parity)
				return true;
		}
	}
	for (set<int>::const_iterator it = edges[root].begin(); it != edges[root].end(); ++it) {
		int node = *it;
		if (visited[node]) {
			int d = (level[root] - level[node]) & 1;
			if (d == 0)
				return true;
		} else {
			level[node] = level[root] + 1;
			bool result = dfs(node);
			if (result)
				return true;
		}
	}
	return false;
}

void remove_node(int node)
{
	for (set<int>::const_iterator it = edges[node].begin(); it != edges[node].end(); ++it)
		edges[*it].erase(node);
	edges[node].clear();
}

int solve_problem(int num_case)
{
	vector<collision> v;
	int n;

	if (scanf("%d", &n) != 1)
		return 1;

	for (int i = 0; i < n; i++)
		if (scanf("%s %d %d", lane[i], &s[i], &p[i]) != 3)
			return 1;

	fill(start_lane, start_lane + n, -1);
	fill(num_collisions, num_collisions + n, 0);

	for (int i = 0; i < n; i++)
		for (int j = i + 1; j < n; j++) {
			if (s[i] > s[j]) {
				if (p[i] < p[j] + CAR_LENGTH) {
					int ds = s[i] - s[j];
					int dp = p[j] - p[i];
					double tl = (double) (dp - CAR_LENGTH) / ds;
					double tr = (double) (dp + CAR_LENGTH) / ds;
					v.push_back(collision(tl, i, j, true));
					v.push_back(collision(tr, i, j, false));
				}
			} else if (s[i] < s[j]) {
				if (p[j] < p[i] + CAR_LENGTH) {
					int ds = s[j] - s[i];
					int dp = p[i] - p[j];
					double tl = (double) (dp - CAR_LENGTH) / ds;
					double tr = (double) (dp + CAR_LENGTH) / ds;
					v.push_back(collision(tl, i, j, true));
					v.push_back(collision(tr, i, j, false));
				}
			} else {
				if (p[i] < p[j] + CAR_LENGTH && p[j] < p[i] + CAR_LENGTH) {
					start_lane[i] = (lane[i][0] == 'L');
					start_lane[j] = (lane[j][0] == 'L');
				}
			}
		}

	sort(v.begin(), v.end());

	for (int i = 0; i < n; i++)
		edges[i].clear();

	double result = numeric_limits<double>::max();
	for (size_t i = 0; i < v.size(); i++) {
		int x = v[i].x;
		int y = v[i].y;
		if (v[i].t < 0.0) {
			start_lane[x] = (lane[x][0] == 'L');
			start_lane[y] = (lane[y][0] == 'L');
			++num_collisions[x];
			++num_collisions[y];
		} else {
			if (v[i].start) {
				edges[x].insert(y);
				edges[y].insert(x);
				++num_collisions[x];
				++num_collisions[y];
			} else {
				if (--num_collisions[x] == 0) {
					start_lane[x] = -1;
					remove_node(x);
				}
				if (--num_collisions[y] == 0) {
					start_lane[y] = -1;
					remove_node(y);
				}
			}

			bool odd_cycle = false;
			fill(visited, visited + n, false);
			for (int j = 0; j < n; j++)
				if (!visited[j]) {
					level[j] = 0;
					lane_parity = -1;
					if (dfs(j)) {
						odd_cycle = true;
						break;
					}
				}

			if (odd_cycle) {
				result = v[i].t;
				break;
			}
		}
	}

	if (result == numeric_limits<double>::max())
		printf("Case #%d: %s\n", num_case, "Possible");
	else
		printf("Case #%d: %.5e\n", num_case, result);

	return 0;
}

int main(int argc, char *argv[])
{
	const char *filenames[] = {
		"cruise-control.in",
		"cruise-control.out",
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
