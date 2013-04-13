#include <algorithm>
#include <complex>
#include <map>
#include <set>
#include <vector>
#include <cmath>
#include <cstdio>
#include <cstring>

using namespace std;

void trim_unique(double *v, size_t &num_elems)
{
	double *it = unique(v, v + num_elems);
	ptrdiff_t diff = it - v;
	num_elems = (size_t) diff;
}

void filter_vector(double *v, size_t &num_elems, double inf, double sup)
{
	for (size_t i = 0; i < num_elems; i++) {
		while (i < num_elems) {
			double elem = v[i];
			if (elem < inf || elem > sup) {
				v[i] = v[--num_elems];
			} else {
				break;
			}
		}
	}
}

pair<double, double> solve_quadratic(double a, double b, double c)
{
	c /= a;
	b /= a;
	double delta = b * b - 4 * c;
	if (delta >= 0) {
		delta = sqrt(delta);
		return pair<double, double>((-b + delta) / 2.0, (-b - delta) / 2.0);
	} else {
		return pair<double, double>(0.0, 0.0);
	}
}

void solve_cubic(const double *coef, double *roots, size_t &num_roots)
{
	double d = coef[0];
	double c = coef[1];
	double b = coef[2];
	double a = coef[3];

	d /= a;
	c /= a;
	b /= a;
	a /= a;

	double p = (3 * a * c - b * b) / (3 * a * a);
	double q = (2 * b * b * b - 9 * a * b * c + 27 * a * a * d) / (27 * a * a * a);

	double delta = q * q + 4 * p * p * p / 27;

	complex<double> c_delta(delta);
	complex<double> s_delta = sqrt(c_delta);

	complex<double> cube_w = (-q + s_delta) / 2.0;
	complex<double> w = pow(cube_w, 1.0 / 3);
	complex<double> omega = exp(2 * M_PI / 3 * complex<double>(0.0, 1.0));
	for (int i = 0; i < 3; i++) {
		complex<double> root = w - p / (3.0 * w) - b / 3.0;
		if (fabs(root.imag()) < 1e-11) {
			printf("root: %lf %lf\n", root.real(), root.imag());
			roots[num_roots++] = root.real();
		}
		w *= omega;
	}
}

double compute_max_volume(int w, int h)
{
	double points[8];
	size_t num_points;
	double max_volume = 0.0;

	num_points = 0;
	points[num_points++] = 0.0;
	points[num_points++] = w / 2.0;
	points[num_points++] = h;
	points[num_points++] = w / M_PI;
	points[num_points++] = h - w;
	points[num_points++] = h / (1 + M_PI);

	filter_vector(points, num_points, 0.0, min(w / 2.0, (double) h));
	sort(points, points + num_points);
	trim_unique(points, num_points);

	double max_point = 2.0 * h / 3;
	for (size_t i = 1; i < num_points; i++) {
		const double left = points[i - 1];
		const double right = points[i];
		const double mid = left + (right - left) / 2;
		const double r = mid / 2;
		const double length = 2 * M_PI * r;
		const double diff = h - mid;
		if (w >= length) {
			if (diff >= length && w >= diff) {
				double cand = M_PI * right * right * w / 4;
				max_volume = max(max_volume, cand);
			} else {
				if (max_point >= left && max_point <= right) {
					double cand = M_PI * max_point * max_point * (h - max_point) / 4;
					max_volume = max(max_volume, cand);
				}

				double lcand = M_PI * left * left * (h - left) / 4;
				max_volume = max(max_volume, lcand);

				double rcand = M_PI * right * right * (h - right) / 4;
				max_volume = max(max_volume, rcand);
			}
		} else {
			if (diff >= length) {
				double cand = M_PI * right * right * w / 4;
				max_volume = max(max_volume, cand);
			}
		}
	}

	num_points = 0;
	points[num_points++] = w * 2.0;
	points[num_points++] = h - w;
	points[num_points++] = h;

	filter_vector(points, num_points, w * 2.0, h);
	sort(points, points + num_points);
	trim_unique(points, num_points);

	if (num_points == 1)
		points[num_points++] = points[0];

	for (size_t i = 1; i < num_points; i++) {
		double left = points[i - 1];
		double right = points[i];
		double mid = left + (right - left) / 2;
		double length = M_PI * w;
		if (h - mid >= length) {
			double cand = M_PI * w * w * w / 4;
			max_volume = max(max_volume, cand);
		}
	}

	num_points = 0;
	points[num_points++] = w / 2.0;
	points[num_points++] = w * 2.0;
	points[num_points++] = h;
	points[num_points++] = h - w;
	pair<double, double> pr = solve_quadratic(M_PI + 1, -sqrt(2 * w) * M_PI, M_PI * w - h);
	points[num_points++] = pr.first * pr.first;
	points[num_points++] = pr.second * pr.second;

	filter_vector(points, num_points, w / 2.0, min(w * 2.0, (double) h));
	sort(points, points + num_points);
	trim_unique(points, num_points);

	if (num_points == 1)
		points[num_points++] = points[0];

	for (size_t i = 1; i < num_points; i++) {
		double left = points[i - 1];
		double right = points[i];
		double mid = left + (right - left) / 2;
		double r = (w + mid - sqrt(2 * w * mid)) / 2;
		double length = 2 * M_PI * r;
		// Note that r >= w / 4, so length >= M_PI * w / 2 > w
		if (h - mid >= length) {
			double right_r = (w + right - sqrt(2 * w * right)) / 2;
			double cand = M_PI * right_r * right_r * w;
			max_volume = max(max_volume, cand);
		}
	}

	return max_volume;
}

void solve_problem()
{
	int w, h;

	if (scanf("%d %d", &w, &h) != 2)
		return;

	double fm = compute_max_volume(w, h);
	double sm = compute_max_volume(h, w);
	printf("%.23lf\n", max(fm, sm));
}

int main()
{
	int num_tests;

	if (scanf("%d", &num_tests) != 1)
		return 1;
	for (int i = 0; i < num_tests; i++)
		solve_problem();

	return 0;
}
