#include <algorithm>
#include <map>
#include <set>
#include <vector>
#include <limits>
#include <cmath>
#include <cstdio>
#include <cstring>

#define MAX_H 30
#define MAX_W 30
#define MAX_D 50

using namespace std;

int gcd(int x, int y)
{
	if (x < y)
		swap(x, y);
	while (y > 0) {
		int r = x % y;
		x = y;
		y = r;
	}
	return x;
}

struct frac {

	int x, y;

	frac() : x(0), y(1) { }

	frac(int x) : x(x), y(1) { }

	frac(int x, int y)
	{
		int g = gcd(x, y);
		this->x = x / g;
		this->y = y / g;
	}

	bool is_integer() const
	{
		return y == 1;
	}

	double to_double() const
	{
		return (double) x / y;
	}

	bool operator== (const frac &rhs) const
	{
		return (x == rhs.x) && (y == rhs.y);
	}

	bool operator< (const frac &rhs) const
	{
		return x * rhs.y < y * rhs.x;
	}

};

vector< pair<frac, frac> > cross[MAX_D + 1][MAX_D + 1];
int delta_x[] = { +1, -1, -1, +1 };
int delta_y[] = { +1, +1, -1, -1 };
int rotations[][2][2] = {
	{ { +1, 0 }, { 0, +1 } },
	{ { 0, -1 }, { +1, 0 } },
	{ { -1, 0 }, { 0, -1 } },
	{ { 0, +1 }, { -1, 0 } },
};
char grid[MAX_H][MAX_W + 10];

void apply_rotation(int &x, int &y, int d)
{
	int s = rotations[d][0][0] * x + rotations[d][0][1] * y;
	int t = rotations[d][1][0] * x + rotations[d][1][1] * y;
	x = s;
	y = t;
}

bool rotated_mirror(int ix, int iy, int cx, int cy, int d)
{
	int ox = cx - ix;
	int oy = cy - iy;
	apply_rotation(ox, oy, d);
	return grid[ix + ox][iy + oy] == '#';
}

template<class T>
void trim_unique(vector<T> &v)
{
	typename vector<T>::iterator it = unique(v.begin(), v.end());
	typename vector<T>::difference_type diff = it - v.begin();
	v.resize((typename vector<T>::size_type) diff);
}

void fill_cross_vector(vector< pair<frac, frac> > &v, int x, int y)
{
	for (int i = 1; i <= x; i++) {
		frac j(x + (2 * i - 1) * y, 2 * x);
		v.push_back(make_pair(i, j));
	}
	for (int j = 1; j <= y; j++) {
		frac i(y + (2 * j - 1) * x, 2 * y);
		v.push_back(make_pair(i, j));
	}

	sort(v.begin(), v.end());
	trim_unique(v);
}

bool simulate(int x, int y, int dx, int dy, int dir)
{
	vector< pair<frac, frac> > &v = cross[dx][dy];
	if (v.empty())
		fill_cross_vector(v, dx, dy);

	int ix = x, iy = y;
	int cx = x, cy = y;
	int cdir = 0;
	bool killed = false;
	for (size_t k = 0; k < v.size(); k++) {
		if (v[k].first.is_integer()) {
			if (v[k].second.is_integer()) {
				int nx = cx + delta_x[cdir];
				int ny = cy + delta_y[cdir];
				if (rotated_mirror(ix, iy, nx, ny, dir)) {
					if (rotated_mirror(ix, iy, nx, cy, dir)) {
						if (rotated_mirror(ix, iy, cx, ny, dir)) {
							cdir ^= 2;
						} else {
							cdir ^= 1;
							cy = ny;
						}
					} else {
						if (rotated_mirror(ix, iy, cx, ny, dir)) {
							cdir ^= 3;
							cx = nx;
						} else {
							killed = true;
							break;
						}
					}
				} else {
					cx = nx;
					cy = ny;
				}
			} else {
				int nx = cx + delta_x[cdir];
				if (rotated_mirror(ix, iy, nx, cy, dir)) {
					cdir ^= 1;
				} else {
					cx = nx;
				}
			}
		} else {
			int ny = cy + delta_y[cdir];
			if (rotated_mirror(ix, iy, cx, ny, dir)) {
				cdir ^= 3;
			} else {
				cy = ny;
			}
		}
	}

	return (!killed && cx == x && cy == y);
}

int solve_problem(int num_case)
{
	vector< pair<int, int> > v;
	map< pair<int, int>, set< pair<int, int> > > reduced;
	int h, w, d;
	int x, y;

	if (scanf("%d %d %d", &h, &w, &d) != 3)
		return 1;

	for (int i = 0; i < h; i++)
		if (scanf("%s", grid[i]) != 1)
			return 1;

	x = y = -1;
	for (int i = 0; i < h && x < 0; i++)
		for (int j = 0; j < w; j++)
			if (grid[i][j] == 'X') {
				x = i;
				y = j;
				break;
			}

	for (int i = 1; i <= d; i++)
		for (int j = 0; j <= d; j++) {
			int dist = i * i + j * j;
			if (dist > 0 && dist <= d * d)
				v.push_back(make_pair(i, j));
		}

	for (size_t k = 0; k < v.size(); k++) {
		int g = gcd(v[k].first, v[k].second);
		int s = v[k].first / g;
		int t = v[k].second / g;
		reduced[make_pair(s, t)].insert(v[k]);
	}

	int result = 0;

	for (map< pair<int, int>, set< pair<int, int> > >::iterator it = reduced.begin(); it != reduced.end(); ++it) {
		const set< pair<int, int> > &s = it->second;
		for (int d = 0; d < 4; d++)
			for (set< pair<int, int> >::const_iterator s_it = s.begin(); s_it != s.end(); ++s_it) {
				bool r = simulate(x, y, s_it->first, s_it->second, d);
				if (r) {
					++result;
					break;
				}
			}
	}

	printf("Case #%d: %d\n", num_case, result);

	return 0;
}

int main(int argc, char *argv[])
{
	const char *filenames[] = {
		"hall-mirrors.in",
		"hall-mirrors.out",
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
