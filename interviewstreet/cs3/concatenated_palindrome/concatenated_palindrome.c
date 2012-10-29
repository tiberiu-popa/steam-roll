#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NUM_STRINGS 100
#define MAX_LENGTH 100000

size_t v[2 * MAX_LENGTH + 1];
size_t u[MAX_NUM_STRINGS][MAX_LENGTH];
size_t w[MAX_NUM_STRINGS][MAX_LENGTH];
char str[MAX_NUM_STRINGS][MAX_LENGTH + 1];

static size_t min(size_t x, size_t y)
{
	return (x < y) ? x : y;
}

static size_t max(size_t x, size_t y)
{
	return (x > y) ? x : y;
}

void process_palindrome(const char *str, size_t *u, size_t *w)
{
	size_t palindrome_length;
	size_t num_elems;
	size_t length;
	size_t i, j, k;
	int found;

	length = strlen(str);
	palindrome_length = 0;
	num_elems = 0;
	i = 0;
	while (i <= length) {
		while (i > palindrome_length && str[i] == str[i - palindrome_length - 1]) {
			palindrome_length += 2;
			i++;
		}
		v[num_elems++] = palindrome_length;
		found = 0;
		for (j = palindrome_length; j > 0;) {
			j--;
			k = 2 * (i - palindrome_length) + j;
			if (v[k] == j) {
				palindrome_length = j;
				found = 1;
				break;
			}
			v[num_elems++] = min(j, v[k]);
		}
		if (!found) {
			palindrome_length = 1;
			i++;
		}
	}

	memset(u, 0, length * sizeof(*u));
	memset(w, 0, length * sizeof(*w));

	for (i = 0; i < num_elems; i++)
		if (v[i] > 0) {
			j = (i - v[i]) / 2;
			k = j + v[i] - 1;
			if (v[i] > u[j])
				u[j] = v[i];
			if (v[i] > w[k])
				w[k] = v[i];
		}
}

void solve_problem()
{
	size_t answer, candidate;
	int n, m;
	int i, j, k, l;

	if (scanf("%d %d", &n, &m) != 2)
		exit(EXIT_FAILURE);

	for (i = 0; i < n; i++) {
		if (scanf("%s", str[i]) != 1)
			exit(EXIT_FAILURE);
		process_palindrome(str[i], u[i], w[i]);
	}

	answer = 0;
	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
			if (j != i) {
				for (k = 0; k < m; k++) {
					l = m - 1 - k;
					candidate = 2 * k + max(u[i][k], w[j][l]);
					if (candidate > answer)
						answer = candidate;
					if (str[i][k] != str[j][l])
						break;
				}
				if (k == m) {
					candidate = 2 * k;
					if (candidate > answer)
						answer = candidate;
				}
			}
	printf("%lu\n", (unsigned long) answer);
}

int main()
{
	solve_problem();
	return 0;
}
