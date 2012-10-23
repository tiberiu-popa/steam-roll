#include <stdio.h>

#define MAX_N 1000

int count[MAX_N];

int main()
{
	FILE *f;
	int result;
	int elem, candidate;
	int n, m;
	int i, j;

	f = fopen("submat.in", "rt");
	if (f == NULL)
		return 1;

	if (fscanf(f, "%d %d", &n, &m) != 2)
		return 2;
	for (i = 0; i < n; i++)
		for (j = 0; j < m; j++) {
			if (fscanf(f, "%d", &elem) != 1)
				return 2;
			if (elem == 0)
				count[j]++;
		}

	fclose(f);

	result = 0;
	for (j = 0; j < m; j++) {
		candidate = (j + 1) * count[j];
		if (candidate > result)
			result = candidate;
	}

	f = fopen("submat.out", "wt");
	if (f == NULL)
		return 1;

	fprintf(f, "%d\n", result);
	fclose(f);

	return 0;
}
