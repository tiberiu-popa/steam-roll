#include <stdio.h>

int min(int x, int y)
{
	return x < y ? x : y;
}

int get_direction(int x, int y)
{
	if (x < y)
		return -1;
	else
		return x > y;
}

long long compute_sum(int x)
{
	long y = x;
	return y * (y + 1) / 2;
}

long long update_sum(int direction, int run_length, int prev_run)
{
	long long value = 0;

	if (direction != 0) {
		value += compute_sum(run_length);
		if (direction == -1)
			value -= min(prev_run, run_length);
	}

	return value;
}

int solve_problem()
{
	long long sum;
	int i, n;
	int x, prev_x;
	int direction, new_direction;
	int run_length, prev_run;
	int num_elems_read = 0;

	prev_x = -1;
	direction = 0;
	run_length = 0;
	prev_run = 0;
	sum = 0;

	num_elems_read = scanf("%d", &n);
	if (num_elems_read < 1)
		return 1;

	if (n <= 1) {
		printf("%d\n", n);
		return 0;
	}

	for (i = 0; i < n; i++) {
		num_elems_read = scanf("%d", &x);
		if (num_elems_read < 1)
			return 1;
		if (prev_x >= 0) {
			new_direction = get_direction(x, prev_x);
			if (direction == new_direction) {
				if (direction != 0)
					run_length++;
			} else {
				sum += update_sum(direction, run_length, prev_run);
				direction = new_direction;
				prev_run = run_length;
				run_length = (direction != 0);
			}
		}
		if (i == n - 1) {
			sum += update_sum(direction, run_length, prev_run);
		}
		prev_x = x;
	}
	printf("%lld\n", sum + n);

	return 0;
}

int main()
{
	solve_problem();
	return 0;
}
