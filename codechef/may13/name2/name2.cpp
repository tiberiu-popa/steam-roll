#include <algorithm>
#include <map>
#include <cmath>
#include <cstdio>
#include <cstring>

#define BUFFER_SIZE 25000

using namespace std;

char m[BUFFER_SIZE + 1];
char w[BUFFER_SIZE + 1];

void solve_problem()
{
	char *p = m;
	char *q = w;

	if (scanf("%s %s", p, q) != 2)
		return;

	size_t n = strlen(p);
	size_t m = strlen(q);
	if (n < m) {
		swap(p, q);
		swap(n, m);
	}

	size_t pos = 0;
	for (size_t i = 0; i < n && pos < m; i++)
		if (p[i] == q[pos])
			++pos;

	printf("%s\n", (pos == m) ? "YES" : "NO");
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
