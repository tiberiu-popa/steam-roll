#include <algorithm>
#include <map>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#define MAX_N 20000
#define MAX_L 1000
#define MOD_PRIME 101267

using namespace std;

vector<int> parents[MAX_L + 1];
int dp[2][MAX_L + 1];
int prefix[MAX_L];
char str[MAX_L + 1];

void read_input(const char *filename, int &n, int &l)
{
	FILE *f;

	f = fopen(filename, "rt");
	if (f == NULL) {
		fprintf(stderr, "Could not open file: %s\n", filename);
		exit(EXIT_FAILURE);
	}

	if (fscanf(f, "%d %d", &n, &l) != 2)
		exit(EXIT_FAILURE);
	if (fscanf(f, "%s", str) != 1)
		exit(EXIT_FAILURE);

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

int powmod(int x, int n, int mod)
{
	long long result = 1;
	long long base = x;
	while (n > 0) {
		if (n & 1)
			result = (result * base) % mod;
		base = (base * base) % mod;
		n >>= 1;
	}
	return (int) result;
}

int advance_char(int l, int k, char ch)
{
	while (k >= 1 && (k == l || str[k] != ch))
		k = prefix[k - 1];
	if (k < l && str[k] == ch)
		k++;

	return k;
}

void compute_prefix(int l)
{
	int k = 0;
	prefix[0] = k;
	for (int i = 1; i < l; i++) {
		k = advance_char(l, k, str[i]);
		prefix[i] = k;
	}
}

int solve_problem(int n, int l)
{
	int result = powmod(2, n, MOD_PRIME);

	if (l == 0 || l > n)
		return result;

	compute_prefix(l);

	for (int i = 0; i < l; i++) {
		parents[i + 1].push_back(i);
		char ch = 'a' + 'b' - str[i];
		int next = advance_char(l, i, ch);
		parents[next].push_back(i);
	}

	dp[0][0] = 1;
	for (int i = 1; i <= n; i++) {
		int next = i & 1;
		int prev = next ^ 1;
		for (int j = 0; j <= l; j++) {
			int value = 0;
			if (j == l) {
				value = (value + 2 * dp[prev][l]) % MOD_PRIME;
			}
			for (vector<int>::iterator it = parents[j].begin(); it != parents[j].end(); ++it) {
				int parent = *it;
				value = (value + dp[prev][parent]) % MOD_PRIME;
			}
			dp[next][j] = value % MOD_PRIME;
		}
	}

	result -= dp[n & 1][l];
	if (result < 0)
		result += MOD_PRIME;

	return result;
}

int main()
{
	int n, l;
	int result;

	read_input("interzis.in", n, l);
	result = solve_problem(n, l);
	write_output("interzis.out", result);

	return 0;
}
