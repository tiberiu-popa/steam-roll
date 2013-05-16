#include <algorithm>
#include <functional>
#include <limits>
#include <set>
#include <vector>
#include <cmath>
#include <cstdio>
#include <cstring>

#define MAX_N 100
#define WATER_SPEED 10
#define MY_HEIGHT 50

using namespace std;

int dx[] = { 1, -1, 0, 0 };
int dy[] = { 0, 0, 1, -1 };
int c[MAX_N][MAX_N];
int f[MAX_N][MAX_N];
int visited[MAX_N][MAX_N];
double min_dist[MAX_N][MAX_N];

int solve_problem(int num_case)
{
	set< pair< double, pair<int, int> > > q;
	int n, m, h;

	if (scanf("%d %d %d", &h, &n, &m) != 3)
		return 1;

	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			if (scanf("%d", &c[i][j]) != 1)
				return 1;

	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			if (scanf("%d", &f[i][j]) != 1)
				return 1;

	for (int i = 0; i < n; i++) {
		fill(visited[i], visited[i] + m, false);
		fill(min_dist[i], min_dist[i] + m, numeric_limits<double>::max());
	}

	pair<int, int> source(0, 0);
	q.insert(make_pair(0.0, source));
	while (!q.empty()) {
		set< pair< double, pair<int, int> > >::iterator it = q.begin();
		double t = it->first;
		int x = it->second.first;
		int y = it->second.second;
		visited[x][y] = true;
		q.erase(it);

		if (x == n - 1 && y == m - 1)
			break;

		double level = h - WATER_SPEED * t;
		for (int i = 0; i < 4; i++) {
			int nx = x + dx[i];
			int ny = y + dy[i];
			if (nx >= 0 && nx < n && ny >= 0 && ny < m && !visited[nx][ny]) {
				if (max(f[x][y], f[nx][ny]) + MY_HEIGHT <= min(c[x][y], c[nx][ny])) {
					double nt = t;
					double nlevel = level;
					double limit = c[nx][ny] - MY_HEIGHT;
					if (nlevel > limit) {
						nt += (double) (nlevel - limit) / WATER_SPEED;
						nlevel = limit;
					}
					if (nt > 0.0) {
						if (nlevel >= f[x][y] + 20.0)
							nt += 1.0;
						else
							nt += 10.0;
					}
					if (nt < min_dist[nx][ny]) {
						pair<int, int> np(nx, ny);
						if (min_dist[nx][ny] != numeric_limits<double>::max())
							q.erase(make_pair(min_dist[nx][ny], np));
						min_dist[nx][ny] = nt;
						q.insert(make_pair(nt, np));
					}
				}
			}
		}
	}
	printf("Case #%d: %lf\n", num_case, min_dist[n - 1][m - 1]);

	return 0;
}

int main(int argc, char *argv[])
{
	const char *filenames[] = {
		"tide-inout.in",
		"tide-inout.out",
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
