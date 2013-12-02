#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#define BUFFER_SIZE 100000

using namespace std;

char str[BUFFER_SIZE + 10];
int x[BUFFER_SIZE + 1];
int y[BUFFER_SIZE + 1];
int z[BUFFER_SIZE + 1];

char *trim_line(char *str)
{
	return strtok(str, "\r\n");
}

int check_large(int left, int right)
{
	int dx = x[right + 1] - x[left];
	int dy = y[right + 1] - y[left];
	int dz = z[right + 1] - z[left];
	return abs(dx - dy) <= 1 && abs(dy - dz) <= 1 && abs(dz - dx) <= 1;
}

int solve_problem()
{
	char *p;
	size_t n;
	int m;

	if (fgets(str, sizeof(str), stdin) == NULL)
		return 1;
	p = trim_line(str);
	n = strlen(p);

	for (size_t i = 0; i < n; i++) {
		x[i + 1] = x[i] + (p[i] == 'x');
		y[i + 1] = y[i] + (p[i] == 'y');
		z[i + 1] = z[i] + (p[i] == 'z');
	}

	if (scanf("%d", &m) != 1)
		return 1;

	for (int i = 0; i < m; i++) {
		int left, right;
		if (scanf("%d %d", &left, &right) != 2)
			return 1;
		--left;
		--right;
		if (right - left <= 1 || check_large(left, right))
			printf("YES\n");
		else
			printf("NO\n");
	}

	return 0;
}

int main()
{
	solve_problem();
	return 0;
}
