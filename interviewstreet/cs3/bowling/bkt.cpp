#include <cstdio>
#include <cstdlib>

#define MAX_N 100
#define MOD_PRIME 1000000007

int st_x[MAX_N];
int st_y[MAX_N];
int n, m;
int soln;

void increment(int &x)
{
	++x;
	if (x >= MOD_PRIME)
		x -= MOD_PRIME;
}

void bkt(int k, int score)
{
	if (score > m)
		return;
	if (k == n) {
		if (st_x[k - 1] + st_y[k - 1] == 10) {
			if (st_x[k - 1] == 10) {
				for (int i = 0; i <= 10; i++)
					for (int j = 0; j <= 10; j++) {
						if (i < 10 && i + j > 10)
							break;
						int diff = i + j;
						if (k >= 2 && st_x[k - 2] == 10)
							diff += i;
						if (score + diff == m)
							increment(soln);
					}
			} else {
				for (int i = 0; i <= 10; i++)
					if (score + i == m)
						increment(soln);
			}
		} else {
			if (score == m)
				increment(soln);
		}
		return;
	}
	for (int i = 0; i <= 10; i++)
		for (int j = 0; j <= i; j++) {
			st_x[k] = j;
			st_y[k] = i - j;
			int diff = i;
			if (k >= 1) {
				if (st_x[k - 1] + st_y[k - 1] == 10)
					diff += j;
				if (st_x[k - 1] == 10) {
					diff += i - j;
					if (k >= 2) {
						if (st_x[k - 2] == 10)
							diff += j;
					}
				}
			}
			bkt(k + 1, score + diff);
		}
}

int main()
{
	int num_tests = 0;

	if (scanf("%d", &num_tests) != 1)
		exit(EXIT_FAILURE);
	for (int i = 0; i < num_tests; i++) {
		if (scanf("%d %d", &n, &m) != 2)
			exit(EXIT_FAILURE);
		soln = 0;
		bkt(0, 0);
		printf("soln=%d\n", soln);
	}

	return 0;
}
