#include <vector>
#include <cstdio>

struct line {
	int a, b, c;
};

using namespace std;

vector<int> x, y;

void compute_pbis(line &pbis, size_t i, size_t j)
{
	int mx = (x[i] + x[j]) / 2;
	int my = (y[i] + y[j]) / 2;
	pbis.a = x[j] - x[i];
	pbis.b = y[j] - y[i];
	pbis.c = -(pbis.a * mx + pbis.b * my);
}

long long sqr(long long x)
{
	return x * x;
}

double sqr(double x)
{
	return x * x;
}

int solve_problem()
{
	int n;

	if (scanf("%d", &n) != 1)
		return 1;

	x.clear();
	y.clear();

	for (int i = 0; i < n; i++) {
		int p, q;
		if (scanf("%d %d", &p, &q) != 2)
			return 1;
		x.push_back(2 * p);
		y.push_back(2 * q);
	}

	int s = 0, t = 0;
	for (size_t i = 0; i < x.size(); i++)
		for (size_t j = i + 1; j < x.size(); j++)
			for (size_t k = j + 1; k < x.size(); k++) {
				line m1, m2;
				compute_pbis(m1, i, j);
				compute_pbis(m2, j, k);
				long long det = m1.b * m2.a - m1.a * m2.b;
				if (det != 0) {
					int nx = m1.c * m2.b - m1.b * m2.c;
					int ny = m1.a * m2.c - m1.c * m2.a;
					long long ir = sqr(nx - x[i] * det) + sqr(ny - y[i] * det);
					for (size_t l = 0; l < x.size(); l++)
						if (l != i && l != j && l != k) {
							long long id = sqr(nx - x[l] * det) + sqr(ny - y[l] * det);
							if (id <= ir)
								++s;
						}
				}
				t += n - 3;
			}

	printf("%lf\n", (double) s / t);

	return 0;
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
