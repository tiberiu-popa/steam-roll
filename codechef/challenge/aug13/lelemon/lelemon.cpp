#include <algorithm>
#include <functional>
#include <vector>
#include <cstdio>

#define MAX_N 100
#define MAX_M 10000

using namespace std;

int p[MAX_M];
vector<int> v[MAX_N];
size_t q[MAX_N];

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
	int n, m, k;

	n = read_unsigned_integer<int>();
	m = read_unsigned_integer<int>();

	for (int i = 0; i < n; i++)
		v[i].clear();

	for (int i = 0; i < m; i++)
		p[i] = read_unsigned_integer<int>();

	for (int i = 0; i < n; i++) {
		k = read_unsigned_integer<int>();
		for (int j = 0; j < k; j++)
			v[i].push_back(read_unsigned_integer<int>());
		sort(v[i].begin(), v[i].end(), greater<int>());
	}

	int result = 0;
	fill(q, q + n, 0);
	for (int i = 0; i < m; i++) {
		int j = p[i];
		if (q[j] < v[j].size()) {
			int elem = v[j][q[j]];
			result += elem;
			q[j]++;
		}
	}
	printf("%d\n", result);

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
