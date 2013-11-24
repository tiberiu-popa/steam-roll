#include <cstdio>
#include <cstdlib>
#include <cstring>

#define BUFFER_SIZE 200000

using namespace std;

char str[BUFFER_SIZE + 10];

char *trim_line(char *str)
{
	return strtok(str, "\r\n");
}

int solve_problem()
{
	if (fgets(str, sizeof(str), stdin) == NULL)
		return 1;
	char *p = trim_line(str);
	size_t n = strlen(str);

	size_t m = 0;
	int run = 0;
	int superrun = 0;
	for (size_t i = 0; i < n; i++) {
		if (i > 0) {
			if (p[i] == p[i - 1]) {
				run++;
			} else {
				if (run >= 2)
					superrun++;
				else
					superrun = 0;
				run = 1;
			}
		} else {
			run++;
		}
		if (run <= 2 - (superrun & 1))
			p[m++] = p[i];
	}

	p[m] = '\0';
	printf("%s\n", p);

	return 0;
}

int main()
{
	solve_problem();
	return 0;
}
