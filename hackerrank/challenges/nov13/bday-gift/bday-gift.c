#include <stdio.h>

int solve_problem()
{
	long long sum = 0;
	int i, n;
	int elem;

	if (scanf("%d", &n) != 1)
		return 1;

	for (i = 0; i < n; i++) {
		if (scanf("%d", &elem) != 1)
			return 1;
		sum += elem;
	}

	printf("%lld.%lld\n", sum / 2, 5 * (sum % 2));

	return 0;
}

int main()
{
	solve_problem();
	return 0;
}
