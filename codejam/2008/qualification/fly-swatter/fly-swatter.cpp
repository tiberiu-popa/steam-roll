#include <algorithm>
#include <limits>
#include <set>
#include <cmath>
#include <cstdio>
#include <cstring>

using namespace std;

double solve_quadratic(double a, double b, double c)
{
	double delta = b * b - 4 * a * c;
	if (delta >= 0)
		return (-b + sqrt(delta)) / (2 * a);
	else
		return -numeric_limits<double>::max();
}

bool inside_circle(double x, double y, double r)
{
	return x * x + y * y <= r * r;
}

double circle_segment(double rad, double th)
{
	return rad * rad * (th - sin(th)) / 2;
}

int solve_problem(int num_case)
{
	double f, R, t, r, g;
	double area = 0.0;

	if (scanf("%lf %lf %lf %lf %lf", &f, &R, &t, &r, &g) != 5)
		return 1;

	r += f;
	t += f;
	g -= 2 * f;

	double inner_radius = R - t;
	double step = g + 2 * r;
	for (double x = r; g >= 0; x += step) {
		double diff = x * x + r * r - inner_radius * inner_radius;
		if (diff > 0)
			break;
		double low_root = solve_quadratic(step * step, 2 * r * step, diff);
		int num_squares = (int) ceil(low_root);
		double up_root = solve_quadratic(step * step, 2 * (r + g) * step, diff + 2 * g * (r + x + g));
		int threshold = -1;
		if (up_root >= 0)
			threshold = (int) floor(up_root);
		area += (threshold + 1) * g * g;
		for (int i = threshold + 1; i < num_squares; i++) {
			double y = r + i * step;
			if (inside_circle(x + g, y + g, inner_radius)) {
				area += g * g;
			} else {
				double circular_area = 0.0;
				double angle = 0.0;
				if (inside_circle(x + g, y, inner_radius)) {
					double yr = sqrt(inner_radius * inner_radius - (x + g) * (x + g));
					if (inside_circle(x, y + g, inner_radius)) {
						double xl = sqrt(inner_radius * inner_radius - (y + g) * (y + g));
						circular_area += g * g - (x + g - xl) * (y + g - yr) / 2;
						angle = atan2(y + g, xl) - atan2(yr, x + g);
					} else {
						double yl = sqrt(inner_radius * inner_radius - x * x);
						circular_area += g * (yr - y) + g * (yl - yr) / 2;
						angle = atan2(yl, x) - atan2(yr, x + g);
					}
				} else {
					double xr = sqrt(inner_radius * inner_radius - y * y);
					if (inside_circle(x, y + g, inner_radius)) {
						double xl = sqrt(inner_radius * inner_radius - (y + g) * (y + g));
						circular_area += g * (xl - x) + g * (xr - xl) / 2;
						angle = atan2(y + g, xl) - atan2(y, xr);
					} else {
						double yl = sqrt(inner_radius * inner_radius - x * x);
						circular_area += (xr - x) * (yl - y) / 2;
						angle = atan2(yl, x) - atan2(y, xr);
					}
				}
				circular_area += inner_radius * inner_radius * (angle - sin(angle)) / 2;
				area += circular_area;
			}
		}
	}
	area *= 4;

	printf("Case #%d: %lf\n", num_case, 1 - area / (M_PI * R * R));
	return 0;
}

int main(int argc, char *argv[])
{
	const char *filenames[] = {
		"fly-swatter.in",
		"fly-swatter.out",
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
