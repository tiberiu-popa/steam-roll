#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#define MAX_N 1000000
#define MAX_Q 500000

using namespace std;

char str[2][MAX_N + 1];
char merge_fw_str[2 * MAX_N + 1];
char merge_bw_str[2 * MAX_N + 1];
int prefix[MAX_N];
int z_fw[2 * MAX_N];
int z_bw[2 * MAX_N];
int queries[MAX_Q][2];

void read_input(const char *filename, int &n, int &m, int &q)
{
	FILE *f;

	f = fopen(filename, "rt");
	if (f == NULL) {
		fprintf(stderr, "Could not open file: %s\n", filename);
		exit(EXIT_FAILURE);
	}

	if (fscanf(f, "%d %d %d", &n, &m, &q) != 3)
		exit(EXIT_FAILURE);
	if (fscanf(f, "%s %s", str[0], str[1]) != 2)
		exit(EXIT_FAILURE);

	memcpy(merge_fw_str, str[1], ((size_t) m) * sizeof(*str[1]));
	memcpy(merge_fw_str + m, str[0], ((size_t) n) * sizeof(*str[0]));
	merge_fw_str[m + n] = 0;

	memcpy(merge_bw_str, str[1], ((size_t) m) * sizeof(*str[1]));
	for (int i = n, j = m; i > 0; j++) {
		--i;
		merge_bw_str[j] = str[0][i];
	}
	merge_bw_str[m + n] = 0;

	for (int i = 0; i < q; i++) {
		int x, y;
		if (fscanf(f, "%d %d", &x, &y) != 2)
			exit(EXIT_FAILURE);
		queries[i][0] = --x;
		queries[i][1] = --y;
	}

	fclose(f);
}

void write_output(const char *filename, int n, int m, int q)
{
	FILE *f;

	f = fopen(filename, "wt");
	if (f == NULL) {
		fprintf(stderr, "Could not open file: %s\n", filename);
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < q; i++) {
		int x = queries[i][0];
		int y = queries[i][1];
		if (x <= 0 || y >= n - 1) {
			fprintf(f, "%d\n", 0);
		} else {
			int p = z_bw[m + n - 1 - (x - 1)];
			int q = z_fw[m + y + 1];
			fprintf(f, "%d\n", min(p, q));
		}
	}
	fclose(f);
}

void z_algo(int *z, char *array, int n) {
	int l, r;

	z[0] = n;
	l = -1;
	r = -1;
	for (int i = 1; i < n; i++) {
		if (i > r) {
			int j;
			for (j = 0; j < n - i; j++)
				if (array[j] != array[i + j])
					break;
			if (j == 0) {
				l = r = -1;
				z[i] = 0;
			} else {
				l = i;
				r = i + j - 1;
				z[i] = r - l + 1;
			}
		} else {
			int k = i - l;
			if (z[k] < r - i + 1) {
				z[i] = z[k];
			} else {
				l = i;
				int j;
				for (j = r - i + 1; j < n - i; j++)
					if (array[j] != array[i + j])
						break;
				r = i + j - 1;
				z[i] = r - l + 1;
			}
		}
	}
}

void solve_problem(int n, int m)
{
	z_algo(z_fw, merge_fw_str, n + m);
	z_algo(z_bw, merge_bw_str, n + m);
}

int main()
{
	int n, m, q;

	read_input("x.in", n, m, q);
	solve_problem(n, m);
	write_output("x.out", n, m, q);

	return 0;
}
