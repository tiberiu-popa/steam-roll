#include <algorithm>
#include <limits>
#include <vector>
#include <cstdio>
#include <cstring>

#define MAX_N 100000

using namespace std;

vector<int> neighbours[MAX_N];
int c[MAX_N];
int visited[MAX_N];
int num_components = 0;
int scores[MAX_N];

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

void dfs(int root)
{
	visited[root] = num_components;
	for (vector<int>::const_iterator it = neighbours[root].begin(); it != neighbours[root].end(); ++it) {
		int node = *it;
		if (visited[node] == 0)
			dfs(node);
	}
}

int solve_problem()
{
	int n, m;

	n = read_unsigned_integer<int>();
	m = read_unsigned_integer<int>();

	for (int i = 0; i < m; i++) {
		int x, y;
		x = read_unsigned_integer<int>() - 1;
		y = read_unsigned_integer<int>() - 1;
		neighbours[x].push_back(y);
		neighbours[y].push_back(x);
	}

	for (int i = 0; i < n; i++)
		c[i] = read_signed_integer<int>();

	for (int i = 0; i < n; i++)
		if (visited[i] == 0) {
			++num_components;
			dfs(i);
		}

	if (num_components == 1) {
		printf("%d\n", 0);
		return 0;
	}

	fill(scores, scores + num_components, numeric_limits<int>::max());
	for (int i = 0; i < n; i++) {
		int comp = visited[i] - 1;
		if (c[i] >= 0 && c[i] < scores[comp])
			scores[comp] = c[i];
	}

	for (int i = 0; i < num_components; i++)
		if (scores[i] == numeric_limits<int>::max()) {
			printf("%d\n", -1);
			return 0;
		}

	sort(scores, scores + num_components);

	long long result = scores[0];
	result *= num_components - 1;
	for (int i = 1; i < num_components; i++)
		result += scores[i];
	printf("%lld\n", result);

	return 0;
}

int main()
{
	solve_problem();
	return 0;
}
