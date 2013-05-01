#include <algorithm>
#include <map>
#include <unordered_set>
#include <cmath>
#include <cstdio>
#include <cstring>

#define MAX_N 20
#define MAX_M 20

using namespace std;

int dist[4 * MAX_N * MAX_M];
int s[MAX_N][MAX_M], w[MAX_N][MAX_M], t[MAX_N][MAX_M];

void relax_path(map< int, unordered_set<int> > &reverse_distance,
	int m, int nx, int ny, int nc, int cand_dist)
{
	int nv = 4 * (m * nx + ny) + nc;
	if (dist[nv] < 0 || cand_dist < dist[nv]) {
		if (dist[nv] >= 0) {
			unordered_set<int> &cand_us = reverse_distance[dist[nv]];
			cand_us.erase(nv);
			if (cand_us.empty())
				reverse_distance.erase(dist[nv]);
		}
		dist[nv] = cand_dist;
		reverse_distance[dist[nv]].insert(nv);
	}
}

int solve_problem(int num_case)
{
	int n, m;

	if (scanf("%d %d", &n, &m) != 2)
		return 1;

	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			if (scanf("%d %d %d", &s[i][j], &w[i][j], &t[i][j]) != 3)
				return 1;

	fill(dist, dist + 4 * n * m, -1);

	map< int, unordered_set<int> > reverse_distance;
	int source = 4 * (n - 1) * m;
	dist[source] = 0;
	reverse_distance[0].insert(source);
	while (!reverse_distance.empty()) {
		map< int, unordered_set<int> >::iterator us_it = reverse_distance.begin();
		unordered_set<int> &min_us = us_it->second;
		unordered_set<int>::iterator min_it = min_us.begin();
		int min_dist = us_it->first;
		int min_vertex = *min_it;
		int corner = min_vertex % 4;
		min_vertex /= 4;
		int x = min_vertex / m;
		int y = min_vertex % m;
		min_us.erase(min_it);
		if (min_us.empty())
			reverse_distance.erase(us_it);

		if (y > 0 && (corner == 0 || corner == 3)) {
			relax_path(reverse_distance, m, x, y - 1, corner ^ 1, min_dist + 2);
		}

		if (y < m - 1 && (corner == 1 || corner == 2)) {
			relax_path(reverse_distance, m, x, y + 1, corner ^ 1, min_dist + 2);
		}

		if (x > 0 && (corner == 2 || corner == 3)) {
			relax_path(reverse_distance, m, x - 1, y, 3 - corner, min_dist + 2);
		}

		if (x < n - 1 && (corner == 0 || corner == 1)) {
			relax_path(reverse_distance, m, x + 1, y, 3 - corner, min_dist + 2);
		}

		int sum = s[x][y] + w[x][y];
		int r = (min_dist - t[x][y]) % sum;
		if (r < 0)
			r += sum;

		if (r >= s[x][y]) {
			relax_path(reverse_distance, m, x, y, corner ^ 1, min_dist + 1);
		} else {
			relax_path(reverse_distance, m, x, y, corner ^ 1, min_dist + (s[x][y] - r) + 1);
		}

		if (r < s[x][y]) {
			relax_path(reverse_distance, m, x, y, 3 - corner, min_dist + 1);
		} else {
			relax_path(reverse_distance, m, x, y, 3 - corner, min_dist + (sum - r) + 1);
		}
	}

	printf("Case #%d: %d\n", num_case, dist[4 * (m - 1) + 2]);

	return 0;
}

int main(int argc, char *argv[])
{
	const char *filenames[] = {
		"crossing-road.in",
		"crossing-road.out",
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
