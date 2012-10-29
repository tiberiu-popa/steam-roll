#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_N 15
#define NUM_BITS 31

#define last_bit(n) ((n) & 1)
#define is_even(n) (!last_bit(n))
#define is_odd(n) (last_bit(n))
#define is_clear_bit(n, k) (is_even((n) >> (k)))
#define is_set_bit(n, k) (is_odd((n) >> (k)))

static long long min(long long x, long long y)
{
	return (x < y) ? x : y;
}

int compare_int_gt(const void *a, const void *b)
{
	int x, y;

	x = * (const int *) a;
	y = * (const int *) b;
	if (x > y)
		return -1;
	else
		return x < y;
}

long long bkt(int *prev, unsigned int n, int bit, long long sum, long long min_sum)
{
	int next[MAX_N];
	unsigned int k;
	int mask;

	mask = 1 << bit;

	memcpy(next, prev, n * sizeof(*prev));
	qsort(next, n, sizeof(*next), compare_int_gt);

	for (k = 0; k < n; k++)
		if (is_set_bit(next[k], bit))
			next[k] &= ~mask;
		else
			break;

	if (k & 1) {
		if (k < n) {
			sum += mask - next[k];
			next[k++] = 0;
		} else {
			return min_sum;
		}
	}

	if (bit == 0) {
		return min(sum, min_sum);
	} else {
		while (sum <= min_sum) {
			min_sum = bkt(next, n, bit - 1, sum, min_sum);
			if (k + 1 < n) {
				sum += mask - next[k];
				next[k++] = 0;
				sum += mask - next[k];
				next[k++] = 0;
			} else {
				break;
			}
		}
	}

	return min_sum;
}

void solve_unfair_game(int *v, unsigned int n)
{
	long long answer;
	int num_bits;

	qsort(v, n, sizeof(*v), compare_int_gt);

	for (num_bits = NUM_BITS; num_bits >= 0; num_bits--)
		if (is_set_bit(v[0], num_bits))
			break;

	answer = bkt(v, n, num_bits + 1, 0LL, LLONG_MAX);
	printf("%lld\n", answer);
}

void solve_problem()
{
	int v[MAX_N];
	int i, n;

	if (scanf("%d", &n) != 1)
		exit(EXIT_FAILURE);

	for (i = 0; i < n; i++) {
		if (scanf("%d", &v[i]) != 1)
			exit(EXIT_FAILURE);
	}

	solve_unfair_game(v, (unsigned int) n);
}

int main()
{
	int num_tests;
	int i;

	if (scanf("%d", &num_tests) != 1)
		return EXIT_FAILURE;

	for (i = 0; i < num_tests; i++)
		solve_problem();

	return 0;
}
