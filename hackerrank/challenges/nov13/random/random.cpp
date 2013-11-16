#include <algorithm>
#include <cstdio>

#define MAX_N 1000

using namespace std;

float g[MAX_N][MAX_N];
float h[2][MAX_N];
int kavinsky[MAX_N];
int d[MAX_N];

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

pair<float, float> mulcoef(pair<float, float> x, pair<float, float> y)
{
	float q = x.second * y.second;
	float p = (x.first + x.second) * (y.first + y.second) - q;
	return make_pair(p, q);
}

pair<float, float> powcoef(pair<float, float> x, int e)
{
	pair<float, float> result(0.0f, 1.0f);
	pair<float, float> base = x;

	if (e > 0) {
		while (true) {
			if (e & 1) {
				result = mulcoef(result, base);
			}
			e >>= 1;
			if (e <= 0)
				break;
			base = mulcoef(base, base);
		}
	}

	return result;
}

int solve_problem()
{
	int n, a, b;

	n = read_unsigned_integer<int>();
	a = read_unsigned_integer<int>();
	b = read_unsigned_integer<int>();

	for (int i = 0; i < n; i++)
		d[i] = read_signed_integer<int>();

	float s = 2.0f / (n - 1) , t = (float) (n - 3) / (n - 1);
	pair<float, float> coef = powcoef(make_pair(s, t), a);
	coef.first /= n;

	int m = n * (n - 1) / 2;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++) {
			int l = min(i, j) + 1;
			int r = n - max(i, j);
			int x = min(l, r) - (i == j);
			kavinsky[j] += x;
			if (i == j)
				x += i * (i - 1) / 2 + (n - 1 - i) * (n - 1 - i - 1) / 2;
			g[i][j] = (float) x / m;
		}

	long long sum = 0;
	for (int i = 0; i < n; i++) {
		sum += d[i];
		h[0][i] = d[i];
	}

	int p = 0;
	for (int k = 0; k < b; k++) {
		float *alpha = h[p];
		p ^= 1;
		float *beta = h[p];
		for (int i = 0; i < n; i++) {
			beta[i] = 0.0;
			for (int j = 0; j < n; j++)
				beta[i] += g[i][j] * alpha[j];
		}
	}

	for (int i = 0; i < n; i++)
		h[p][i] = coef.first * sum + coef.second * h[p][i];

	float result = 0.0;
	for (int i = 0; i < n; i++)
		result += kavinsky[i] * h[p][i];
	result /= m;
	printf("%f\n", result);

	return 0;
}

int main()
{
	solve_problem();
	return 0;
}
