#include <algorithm>
#include <vector>
#include <cmath>
#include <cstdio>
#include <cstring>

#define MAX_N 20
#define MAX_M (MAX_N * MAX_N)
#define BYTE_SIZE 8
#define NUM_BYTES ((MAX_M + BYTE_SIZE - 1) / BYTE_SIZE)
#define ULL_SIZE 8
#define NUM_ULLS ((NUM_BYTES + ULL_SIZE - 1) / ULL_SIZE)

using namespace std;

int grid[MAX_N][MAX_N];
unsigned long long t[MAX_M][NUM_ULLS];
unsigned long long r[NUM_ULLS];
unsigned long long s[NUM_ULLS];
unsigned long long p[NUM_ULLS];
bool used[MAX_M];

int get_bit(unsigned long long *v, int idx)
{
	return (v[idx >> 6] & (1ULL << (idx & 63))) != 0;
}

void set_bit(unsigned long long *v, int idx)
{
	v[idx >> 6] |= (1ULL << (idx & 63));
}

void clear_bit(unsigned long long *v, int idx)
{
	v[idx >> 6] &= ~(1ULL << (idx & 63));
}

void gauss_jordan(int n, int m, int row, int col)
{
	for (int i = 0; i < n; i++)
		if (i != row) {
			int factor = get_bit(t[i], col);
			if (factor != 0) {
				for (int k = 0; k < m; k++)
					if (get_bit(t[row], k)) {
						int bit = get_bit(t[i], k);
						if (bit)
							clear_bit(t[i], k);
						else
							set_bit(t[i], k);
					}
				if (get_bit(r, row)) {
					int bit = get_bit(r, i);
					if (bit)
						clear_bit(r, i);
					else
						set_bit(r, i);
				}
			}
		}
}

void search_solution(int n, int m)
{
	for (int col = 0; col < m; col++) {
		int row;
		for (row = 0; row < n; row++)
			if (!used[row] && get_bit(t[row], col))
				break;
		if (row < n) {
			if (row != col) {
				unsigned long long tmp[NUM_ULLS];
				memcpy(tmp, t[row], sizeof(tmp));
				memcpy(t[row], t[col], sizeof(tmp));
				memcpy(t[col], tmp, sizeof(tmp));
				int a = get_bit(r, row);
				int b = get_bit(r, col);
				if (a)
					set_bit(r, col);
				else
					clear_bit(r, col);
				if (b)
					set_bit(r, row);
				else
					clear_bit(r, row);
			}
			used[col] = true;
			gauss_jordan(n, m, col, col);
		}
	}
}

int solve_problem()
{
	int n, d;

	if (scanf("%d %d", &n, &d) != 2)
		return 1;

	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			if (scanf("%d", &grid[i][j]) != 1)
				return 1;

	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			if (grid[i][j])
				set_bit(r, i * n + j);

	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++) {
			int x = i * n + j;
			for (int k = 0; k < n; k++)
				for (int l = 0; l < n; l++) {
					int dist = abs(k - i) + abs(l - j);
					if (dist <= d) {
						int y = k * n + l;
						set_bit(t[y], x);
					}
				}
		}

	int m = n * n;
	
	search_solution(m, m);

	for (int i = 0; i < m; i++) {
		if (get_bit(r, i) != get_bit(s, i)) {
			if (used[i]) {
				for (int j = 0; j < m; j++) {
					int flip = get_bit(t[j], i);
					if (flip) {
						int bit = get_bit(s, j);
						if (bit)
							clear_bit(s, j);
						else
							set_bit(s, j);
					}
				}
				set_bit(p, i);
			}
		}
	}

	bool found = true;
	for (int i = 0; i < m; i++)
		if (get_bit(r, i) != get_bit(s, i)) {
			found = false;
			break;
		}

	if (found) {
		vector< pair<int, int> > points;
		for (int j = 0; j < m; j++)
			if (get_bit(p, j))
				points.push_back(make_pair(j / n, j % n));
		printf("Possible\n");
		printf("%zu\n", points.size());
		for (size_t i = 0; i < points.size(); i++)
			printf("%d %d\n", points[i].first, points[i].second);
	} else {
		printf("Impossible\n");
	}

	return 0;
}

int main()
{
	solve_problem();
	return 0;
}
