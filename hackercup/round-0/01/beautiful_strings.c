#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_M 50
#define MAX_LENGTH 501
#define NUM_LETTERS 26

int int_compare(const void *a, const void *b)
{
	const int *x = a;
	const int *y = b;

	if (*x < *y)
		return -1;
	else
		return *x > *y;
}

int main()
{
	int s[NUM_LETTERS];
	char line[MAX_LENGTH + 1];
	size_t i, len;
	int m;
	int sum;
	int num_cases = 0;

	if (fgets(line, sizeof(line), stdin) == NULL)
		return 1;

	m = atoi(line);

	while (fgets(line, sizeof(line), stdin) != NULL && num_cases < m) {
		++num_cases;
		len = strlen(line);
		memset(s, 0, sizeof(s));
		for (i = 0; i < len; i++)
			if (isalpha(line[i]))
				s[tolower(line[i]) - 'a']++;
		qsort(s, sizeof(s) / sizeof(*s), sizeof(*s), int_compare);
		sum = 0;
		for (i = 0; i < 26; i++)
			sum += (i + 1) * s[i];
		printf("Case #%d: %d\n", num_cases, sum);
	}

	return 0;
}
