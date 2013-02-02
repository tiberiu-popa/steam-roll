#include <stdio.h>
#include <stdlib.h>

#define RING_SIZE 1000

int compare_int(const void *a, const void *b)
{
	const int *x = a;
	const int *y = b;

	if (*x < *y)
		return -1;
	else if (*x > *y)
		return 1;
	else
		return 0;
}

int main()
{
	int *v, *u;
	int i, j, n;
	long long answer;

	scanf("%d", &n);

	v = malloc(n * sizeof(*v));
	if (v == NULL)
		return 1;

	u = malloc(n * sizeof(*v));
	if (u == NULL)
		return 1;

	for (i = 0; i < n; i++)
		scanf("%d", &v[i]);

	qsort(v, n, sizeof(*v), compare_int);
	for (i = 0; i < n; i++)
		if (v[i] != i)
			break;
	for (j = 0; j < n; j++)
		u[j] = v[(j + i) % n];

	answer = 2 * 100000;
	answer *= n / 2;
	answer *= (n + 1) / 2;
	for (j = 0; j < n - 1; j++)
		if ((u[j] + 1) % RING_SIZE == u[j + 1]) {
			answer++;
			j++;
		}
	answer *= 2;

	printf("%lld\n", answer);

	free(u);
	free(v);

	return 0;
}
