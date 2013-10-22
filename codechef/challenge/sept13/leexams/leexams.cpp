#include <algorithm>
#include <cstdio>

#define MAX_N 50
#define MAX_VALUE 16
#define pow2i(x) (1 << (x))

using namespace std;

int a[MAX_N];
int b[MAX_N];
int p[MAX_N];
double dp[MAX_N][1 << MAX_VALUE];

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

int solve_problem()
{
	int n;

	n = read_unsigned_integer<int>();
	for (int i = 0; i < n; i++) {
		p[i] = read_unsigned_integer<int>();
		a[i] = read_unsigned_integer<int>() - 1;
		b[i] = read_unsigned_integer<int>() - 1;
	}

	double result = 0.0;
	if (n <= 16) {
		int max_mask = pow2i(MAX_VALUE);
		fill(dp[0], dp[0] + max_mask, 0.0);
		dp[0][pow2i(a[0])] += (double) p[0] / 100;
		dp[0][pow2i(b[0])] += (double) (100 - p[0]) / 100;

		for (int i = 1; i < n; i++) {
			fill(dp[i], dp[i] + max_mask, 0.0);
			double pos = (double) p[i] / 100;
			double neg = (double) (100 - p[i]) / 100;
			int pos_mask = pow2i(a[i]);
			int neg_mask = pow2i(b[i]);
			for (int j = 0; j < max_mask; j++)
				if (dp[i - 1][j] > 0.0) {
					if ((j & pos_mask) == 0)
						dp[i][j | pow2i(a[i])] += dp[i - 1][j] * pos;
					if ((j & neg_mask) == 0)
						dp[i][j | pow2i(b[i])] += dp[i - 1][j] * neg;
				}
		}
		for (int j = 0; j < max_mask; j++)
			result += dp[n - 1][j];
	}

	printf("%lf\n", result);

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
