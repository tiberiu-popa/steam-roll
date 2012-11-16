#include <algorithm>
#include <iostream>
#include <limits>
#include <map>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>

using namespace std;

#define MAX_N 2000

long long dp[MAX_N][2];

int x[MAX_N + 1];
long long a[MAX_N];
long long b[MAX_N];

long long s[MAX_N + 1];
long long t[MAX_N + 1];
long long u[MAX_N + 1];
long long v[MAX_N + 1];

static long long generic_to_lower(long long *s, long long *u, int lo, int hi)
{
	return (u[hi + 1] - u[lo]) - x[lo] * (s[hi + 1] - s[lo]);
}

static long long generic_to_upper(long long *s, long long *u, int lo, int hi)
{
	return x[hi] * (s[hi + 1] - s[lo]) - (u[hi + 1] - u[lo]);
}

static long long in_to_lower(int lo, int hi)
{
	return generic_to_lower(s, u, lo, hi);
}

static long long in_to_upper(int lo, int hi)
{
	return generic_to_upper(s, u, lo, hi);
}

static long long out_to_lower(int lo, int hi)
{
	return generic_to_lower(t, v, lo, hi);
}

static long long out_to_upper(int lo, int hi)
{
	return generic_to_upper(t, v, lo, hi);
}

void read_input(const char *filename, int &n)
{
	FILE *f;

	f = fopen(filename, "rt");
	if (f == NULL) {
		fprintf(stderr, "Could not open file: %s\n", filename);
		exit(EXIT_FAILURE);
	}

	if (fscanf(f, "%d", &n) != 1)
		exit(EXIT_FAILURE);
	for (int i = 0; i < n; i++) {
		int elem;
		if (fscanf(f, "%d", &elem) != 1)
			exit(EXIT_FAILURE);
		x[i] = elem;
	}
	x[n] = numeric_limits<int>::max();
	for (int i = 0; i < n; i++) {
		long long elem;
		if (fscanf(f, "%lld", &elem) != 1)
			exit(EXIT_FAILURE);
		a[i] = elem;
		s[i + 1] = s[i] + elem;
		u[i + 1] = u[i] + x[i] * elem;
	}
	for (int i = 0; i < n; i++) {
		long long elem;
		if (fscanf(f, "%lld", &elem) != 1)
			exit(EXIT_FAILURE);
		b[i] = elem;
		t[i + 1] = t[i] + elem;
		v[i + 1] = v[i] + x[i] * elem;
	}
	fclose(f);
}

void write_output(const char *filename, int result)
{
	FILE *f;

	f = fopen(filename, "wt");
	if (f == NULL) {
		fprintf(stderr, "Could not open file: %s\n", filename);
		exit(EXIT_FAILURE);
	}

	fprintf(f, "%d\n", result);
	fclose(f);
}

static long long standardize(long long value)
{
	if (value == numeric_limits<long long>::max())
		return 0;
	else
		return value;
}

int solve_problem(int n)
{
	for (int i = 0; i < n; i++) {
		long long in_min_value = numeric_limits<long long>::max();
		long long out_min_value = numeric_limits<long long>::max();
		int k = 0;
		for (int j = (i + 1 == n); j <= i; j++) {
			if (k < j)
				++k;
			if (j > 0) {
				while (k <= i && x[k] - x[j - 1] <= x[i + 1] - x[k])
					k++;
			}

			long long in_cand = 0;
			long long out_cand = 0;
			if (j > 0) {
				in_cand += dp[j - 1][1];
				in_cand += out_to_lower(j - 1, k - 1);
				out_cand += dp[j - 1][0];
				out_cand += in_to_lower(j - 1, k - 1);
			}
			if (i + 1 < n) {
				in_cand += out_to_upper(k, i + 1);
				out_cand += in_to_upper(k, i + 1);
			}
			if (in_cand < in_min_value)
				in_min_value = in_cand;
			if (out_cand < out_min_value)
				out_min_value = out_cand;
		}

		dp[i][0] = standardize(in_min_value);
		dp[i][1] = standardize(out_min_value);
	}

	return min(dp[n - 1][0], dp[n - 1][1]);
}

int main()
{
	int n, result;

	read_input("metrou.in", n);
	result = solve_problem(n);
	write_output("metrou.out", result);

	return 0;
}
