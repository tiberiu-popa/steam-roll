#include <algorithm>
#include <cstdio>

#define MAX_N 100
#define MAX_VALUE 10000

using namespace std;

int a[MAX_N];
int p[MAX_N];
float dp[2][MAX_VALUE + 1];

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

	for (int i = 0; i < n; i++)
		a[i] = read_unsigned_integer<int>();

	for (int i = 0; i < n; i++)
		p[i] = read_unsigned_integer<int>();

	int sum = 0;
	for (int i = 0; i < n; i++)
		sum += a[i];

	int crt_pos = 0;
	fill(dp[crt_pos], dp[crt_pos] + sum + 1, 0.0f);
	dp[crt_pos][0] = 1.0f;
	for (int i = 0; i < n; i++) {
		float prob = (float) p[i] / 100;
		int next_pos = crt_pos ^ 1;
		for (int j = 0; j <= sum; j++)
			dp[next_pos][j] = dp[crt_pos][j] * (1.0f - prob);
		for (int j = 0; j <= sum; j++)
			dp[next_pos][j + a[i]] += dp[crt_pos][j] * prob;
		crt_pos = next_pos;
	}

	float result = 0.0f;
	for (int i = (sum + 1) / 2; i <= sum; i++)
		result += dp[crt_pos][i];
	printf("%f\n", result);

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
