#include <algorithm>
#include <map>
#include <set>
#include <vector>
#include <cstdio>

#define MAX_N 50
#define MAX_M 10000

using namespace std;

typedef pair<long long, long long> point_ll;
typedef pair<point_ll, point_ll> pair_point_ll;

int poly[MAX_N][2];
int lines[MAX_M][2][2];

int skip_whitespace()
{
	int ch;
	while (true) {
		ch = getchar_unlocked();
		if (ch != ' ' && ch != '\n')
			break;
	}
	return ch;
}

template<typename T>
T read_unsigned_integer()
{
	T result = (T) 0;
	int ch = skip_whitespace();
	while (ch >= '0' && ch <= '9') {
		result = 10 * result + (ch - '0');
		ch = getchar_unlocked();
	}
	return result;
}

template<typename T>
T read_signed_integer()
{
	T result = (T) 0;
	bool flip = false;
	int ch = skip_whitespace();
	if (ch == '-') {
		flip = true;
		ch = skip_whitespace();
	}
	while (ch >= '0' && ch <= '9') {
		result = 10 * result + (ch - '0');
		ch = getchar_unlocked();
	}
	return flip ? -result : result;
}

template<class T>
void trim_unique(vector<T> &v)
{
	typename vector<T>::iterator it = unique(v.begin(), v.end());
	typename vector<T>::difference_type diff = it - v.begin();
	v.resize((typename vector<T>::size_type) diff);
}

template<typename S, typename T>
pair<S, T> pairsub(pair<S, T> x, pair<S, T> y)
{
	return make_pair(x.first - y.first, x.second - y.second);
}

template<typename T, typename K>
bool contains(T coll, K key)
{
	return coll.find(key) != coll.end();
}

long long compute_det(const int p[2], const int q[2])
{
	long long x = p[0];
	x *= q[1];
	long long y = p[1];
	y *= q[0];
	return x - y;
}

long long compute_det(long long a, long long b, long long c, long long d)
{
	return a * d - b * c;
}

long long compute_det(pair<long long, long long> x, pair<long long, long long> y)
{
	return compute_det(x.first, x.second, y.first, y.second);
}

template<typename T>
T gcd(T x, T y)
{
	if (x < 0)
		x = -x;
	if (y < 0)
		y = -y;
	if (x < y)
		swap(x, y);
	while (y > 0) {
		T r = x % y;
		x = y;
		y = r;
	}
	return x;
}

struct frac {

	long long x, y;

	frac(long long x, long long y)
	{
		long long g = gcd(x, y);
		this->x = x / g;
		this->y = y / g;
		if (this->y < 0) {
			this->y = -this->y;
			this->x = -this->x;
		}
	}

	frac(long long x) : x(x), y(1LL) { }

	frac() : x(0LL), y(1LL) { }

	bool operator== (const frac &rhs) const
	{
		return x == rhs.x && y == rhs.y;
	}

	bool operator< (const frac &rhs) const
	{
		double p = (double) x / y;
		double q = (double) rhs.x / rhs.y;
		return p < q;
	}

	frac operator* (long long p) const
	{
		frac result;
		long long g = gcd(p, y);
		result.x = (p / g) * x;
		result.y = y / g;
		return result;
	}

};

struct point_frac {

	frac x, y;

	point_frac() : x(), y() { }

	point_frac(frac x, frac y) : x(x), y(y) { }

	bool operator== (const point_frac &rhs) const
	{
		return x == rhs.x && y == rhs.y;
	}

	bool operator< (const point_frac &rhs) const
	{
		if (x == rhs.x)
			return y < rhs.y;
		else
			return x < rhs.x;
	}

};

bool same_sign(long long x, long long y)
{
	if (x == 0)
		return true;
	else
		return (x > 0) ? (y >= 0) : (y <= 0);
}

bool between_01(long long x, long long y)
{
	if (same_sign(x, y)) {
		return (y > 0) ? (x <= y) : (y <= x);
	} else {
		return false;
	}
}

bool inside_polygon(int x, int y, int n)
{
	for (int i = 0; i < n; i++) {
		int j = (i + 1) % n;
		long long area = compute_det(poly[i][0] - x, poly[i][1] - y, poly[j][0] - x, poly[j][1] - y);
		if (area < 0)
			return false;
	}
	return true;
}

bool inside_polygon(int p[2], int n)
{
	return inside_polygon(p[0], p[1], n);
}

pair<bool, point_frac> intersects(point_ll a, point_ll b, point_ll c, point_ll d)
{
	point_ll x = pairsub(a, b);
	long long det = compute_det(a, b);
	point_ll px = pairsub(c, d);
	long long pdet = compute_det(c, d);
	long long down = compute_det(x, px);
	point_ll diff = pairsub(c, a);
	long long col = compute_det(x, diff);
	if (down != 0) {
		long long rcol = compute_det(px, diff);
		if (between_01(col, down) && between_01(rcol, down)) {
			frac ix(compute_det(det, x.first, pdet, px.first), down);
			frac iy(compute_det(det, x.second, pdet, px.second), down);
			return make_pair(true, point_frac(ix, iy));
		}
	}
	return make_pair(false, point_frac());
}

pair<bool, point_frac> intersects(pair_point_ll x, pair_point_ll y)
{
	return intersects(x.first, x.second, y.first, y.second);
}

bool on_segment(point_ll p, point_ll a, point_ll b)
{
	point_ll x = pairsub(b, a);
	point_ll y = pairsub(p, a);
	long long det = compute_det(x, y);
	return det == 0 && ((x.first != 0) ? between_01(y.first, x.first) : between_01(y.second, x.second));
}

int solve_problem()
{
	int n, m;

	n = read_unsigned_integer<int>();
	m = read_unsigned_integer<int>();

	for (int i = 0; i < n; i++) {
		poly[i][0] = read_signed_integer<int>();
		poly[i][1] = read_signed_integer<int>();
	}

	for (int i = 0; i < m; i++) {
		lines[i][0][0] = read_signed_integer<int>();
		lines[i][0][1] = read_signed_integer<int>();
		lines[i][1][0] = read_signed_integer<int>();
		lines[i][1][1] = read_signed_integer<int>();
	}

	long long area = 0LL;
	long long boundary = 0LL;
	for (int i = 1; i < n; i++) {
		area += compute_det(poly[i - 1], poly[i]);
		boundary += gcd(poly[i][0] - poly[i - 1][0], poly[i][1] - poly[i - 1][1]);
	}
	area += compute_det(poly[n - 1], poly[0]);
	boundary += gcd(poly[0][0] - poly[n - 1][0], poly[0][1] - poly[n - 1][1]);
	long long inside = (area + 2 - boundary) / 2;

	vector<point_ll> left;
	vector<point_ll> right;
	for (int i = 0; i < m; i++) {
		const int *a = lines[i][0];
		const int *b = lines[i][1];
		int x = a[0] - b[0];
		int y = a[1] - b[1];
		long long z = compute_det(a, b);
		int g = gcd(x, y);
		bool collinear = false;
		map<point_frac, frac> intersections;
		for (int j = 0; j < n; j++) {
			int nj = (j + 1 == n) ? 0 : (j + 1);
			int *c = poly[j];
			int *d = poly[nj];
			int px = c[0] - d[0];
			int py = c[1] - d[1];
			long long pz = compute_det(c, poly[(j + 1) % n]);
			long long down = compute_det(x, y, px, py);
			int diff[] = { c[0] - a[0], c[1] - a[1] };
			long long col = compute_det(x, y, diff[0], diff[1]);
			if (down != 0) {
				long long rcol = compute_det(px, py, diff[0], diff[1]);
				if (between_01(col, down) && between_01(rcol, down)) {
					frac ix(compute_det(z, x, pz, px), down);
					frac iy(compute_det(z, y, pz, py), down);
					intersections[point_frac(ix, iy)] = frac(rcol, down);
				}
			} else {
				if (col == 0) {
					collinear = true;
					break;
				}
			}
		}
		if (!collinear) {
			set<point_frac> strictly_inside;
			for (int j = 0; j < 2; j++)
				if (inside_polygon(lines[i][j], n)) {
					point_frac p(frac((long long) lines[i][j][0]), frac((long long) lines[i][j][1]));
					if (intersections.find(p) == intersections.end()) {
						intersections[p] = frac(j);
						strictly_inside.insert(p);
					}
				}
			if (intersections.size() >= 2) {
				vector< pair<frac, point_frac> > ratios;
				for (map<point_frac, frac>::iterator it = intersections.begin(); it != intersections.end(); ++it) {
					point_frac p = it->first;
					frac q = it->second * g;
					ratios.push_back(make_pair(q, p));
				}
				sort(ratios.begin(), ratios.end());
				vector<frac> rs;
				for (size_t j = 0; j < ratios.size(); j++)
					rs.push_back(ratios[j].first);
				long long endpoints[2];
				if (rs[0].y > 1) {
					endpoints[0] = (rs[0].x + rs[0].y - 1) / rs[0].y;
				} else {
					endpoints[0] = rs[0].x + !contains(strictly_inside, ratios[0].second);
				}
				if (rs[1].y > 1) {
					endpoints[1] = rs[1].x / rs[1].y;
				} else {
					endpoints[1] = rs[1].x - !contains(strictly_inside, ratios[1].second);
				}
				if (endpoints[0] <= endpoints[1]) {
					left.push_back(make_pair(a[0] - x / g * endpoints[0], a[1] - y / g * endpoints[0]));
					right.push_back(make_pair(a[0] - x / g * endpoints[1], a[1] - y / g * endpoints[1]));
					if (left.back() > right.back())
						swap(left.back(), right.back());
				}
			}
		}
	}

	vector<pair_point_ll> segments;
	vector<point_ll> single_points;

	map< frac, map< frac, set<pair_point_ll> > > merge_map;
	for (size_t i = 0; i < left.size(); i++) {
		if (left[i] != right[i]) {
			long long dy = right[i].second - left[i].second;
			frac dir(right[i].first - left[i].first, dy);
			frac base;
			if (dir.y == 0) {
				if (dir.x < 0)
					dir.x = -dir.x;
				base = frac(left[i].second);
			} else {
				base = frac(left[i].first * right[i].second - left[i].second * right[i].first, dy);
			}
			merge_map[dir][base].insert(make_pair(left[i], right[i]));
		} else {
			single_points.push_back(left[i]);
		}
	}

	for (map< frac, map< frac, set<pair_point_ll> > >::iterator it = merge_map.begin(); it != merge_map.end(); ++it) {
		map< frac, set<pair_point_ll> > &by_base = it->second;
		for (map< frac, set<pair_point_ll> >::iterator b_it = by_base.begin(); b_it != by_base.end(); ++b_it) {
			set<pair_point_ll> &s = b_it->second;
			set<pair_point_ll> ns;
			pair_point_ll last;
			bool first = true;
			for (set<pair_point_ll>::iterator s_it = s.begin(); s_it != s.end(); ++s_it) {
				if (first) {
					last = *s_it;
					first = false;
				} else {
					if (s_it->first <= last.second) {
						last.second = max(last.second, s_it->second);
					} else {
						ns.insert(last);
						last = *s_it;
					}
				}
			}
			if (!first)
				ns.insert(last);
			s.clear();
			s.insert(ns.begin(), ns.end());
		}
	}

	for (map< frac, map< frac, set<pair_point_ll> > >::iterator it = merge_map.begin(); it != merge_map.end(); ++it) {
		map< frac, set<pair_point_ll> > &by_base = it->second;
		for (map< frac, set<pair_point_ll> >::const_iterator b_it = by_base.begin(); b_it != by_base.end(); ++b_it) {
			const set<pair_point_ll> &s = b_it->second;
			segments.insert(segments.end(), s.begin(), s.end());
		}
	}

	sort(segments.begin(), segments.end());
	sort(single_points.begin(), single_points.end());
	trim_unique(single_points);

	long long count = (long long) single_points.size();
	for (size_t i = 0; i < segments.size(); i++) {
		point_ll diff = pairsub(segments[i].first, segments[i].second);
		count += gcd(diff.first, diff.second) + 1;
	}

	set<size_t> used_indices;
	set<point_ll> visited;
	size_t s_idx = 0;
	for (size_t i = 0; i < segments.size(); i++) {
		long long left = segments[i].first.first;
		long long right = segments[i].second.first;
		visited.clear();
		while (s_idx < single_points.size() && single_points[s_idx].first < left)
			++s_idx;
		for (size_t j = s_idx; j < single_points.size(); j++)
			if (on_segment(single_points[j], segments[i].first, segments[i].second))
				used_indices.insert(j);
		for (size_t j = i + 1; j < segments.size() && segments[j].first.first <= right; j++) {
			pair<bool, point_frac> bp = intersects(segments[i], segments[j]);
			if (bp.first) {
				point_frac p = bp.second;
				if (p.x.y == 1 && p.y.y == 1) {
					point_ll q(p.x.x, p.y.x);
					if (visited.insert(q).second)
						--count;
				}
			}
		}
	}

	count -= (long long) used_indices.size();
	printf("%lld %lld\n", boundary + count, inside - count);

	return 0;
}

int main()
{
	int num_tests;

	num_tests = read_unsigned_integer<int>();
	for (int i = 0; i < num_tests; i++)
		solve_problem();

	return 0;
}
