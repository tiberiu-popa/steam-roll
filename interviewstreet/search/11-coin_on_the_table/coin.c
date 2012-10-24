#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define U_IDX 0
#define L_IDX 1
#define D_IDX 2
#define R_IDX 3
#define STAR_IDX 4

#define HIGH_PAYOFF 5000

int dx[] = { -1, 0, 1, 0 };
int dy[] = { 0, -1, 0, 1 };

struct point {
	int x, y;
};

struct vector {
	struct point *arr;
	size_t size, capacity;
};

void vector_init(struct vector *self, size_t capacity);
void vector_clear(struct vector *self);
void vector_destroy(struct vector *self);
void vector_add(struct vector *self, int x, int y);

int **alloc_matrix(int n, int m);
void free_matrix(int **matrix, int n);
void fill_matrix(int **matrix, int n, int m, int value);

int char_to_idx(char ch)
{
	switch (ch) {
	case 'U':
		return U_IDX;

	case 'L':
		return L_IDX;

	case 'D':
		return D_IDX;

	case 'R':
		return R_IDX;

	default:
		return STAR_IDX;
	}
}

int read_matrix(int **matrix, int n, int m)
{
	char *line;
	int i, j;

	line = malloc((m + 1) * sizeof(*line));
	if (line == NULL)
		return 1;

	for (i = 0; i < n; i++) {
		scanf("%s", line);
		for (j = 0; j < m; j++)
			matrix[i][j] = char_to_idx(line[j]);
	}

	free(line);
	return 0;
}

int main()
{
	struct vector open, frontier, tmp;
	size_t capacity;
	int **matrix, **dp;
	size_t t;
	int n, m, k;
	int x, y, nx, ny;
	int dist, cand;
	int i, j;

	scanf("%d", &n);
	scanf("%d", &m);
	scanf("%d", &k);

	matrix = alloc_matrix(n, m);
	if (matrix == NULL)
		return 1;

	read_matrix(matrix, n, m);

	dp = alloc_matrix(n, m);
	if (dp == NULL)
		return 1;

	fill_matrix(dp, n, m, INT_MAX);

	capacity = n;
	capacity *= m;
	vector_init(&open, capacity);
	vector_init(&frontier, capacity);

	dp[0][0] = 0;
	vector_add(&open, 0, 0);
	for (i = 0; i < k; i++) {
		for (t = 0; t < open.size; t++) {
			x = open.arr[t].x;
			y = open.arr[t].y;
			for (j = 0; j < 4; j++) {
				nx = x + dx[j];
				ny = y + dy[j];
				if (nx >= 0 && nx < n && ny >= 0 && ny < m) {
					dist = (j != matrix[x][y]);
					cand = dp[x][y] + dist;
					if (cand < dp[nx][ny]) {
						dp[nx][ny] = cand;
						vector_add(&frontier, nx, ny);
					}
				}
			}
		}
		memcpy(&tmp, &open, sizeof(tmp));
		memcpy(&open, &frontier, sizeof(tmp));
		memcpy(&frontier, &tmp, sizeof(tmp));
		vector_clear(&frontier);
	}

	vector_destroy(&open);
	vector_destroy(&frontier);

	for (i = 0; i < n; i++)
		for (j = 0; j < m; j++)
			if (matrix[i][j] == STAR_IDX) {
				if (dp[i][j] < INT_MAX)
					printf("%d\n", dp[i][j]);
				else
					printf("-1\n");
				break;
			}

	free_matrix(dp, n);
	free_matrix(matrix, n);

	return 0;
}

void vector_init(struct vector *self, size_t capacity)
{
	self->arr = malloc(capacity * sizeof(*self->arr));
	if (self->arr != NULL) {
		self->size = 0;
		self->capacity = capacity;
	}
}

void vector_clear(struct vector *self)
{
	self->size = 0;
}

void vector_destroy(struct vector *self)
{
	vector_clear(self);
	free(self->arr);
	self->capacity = 0;
}

void vector_add(struct vector *self, int x, int y)
{
	struct point p;

	p.x = x;
	p.y = y;
	self->arr[self->size++] = p;
}

int **alloc_matrix(int n, int m)
{
	int **matrix;
	int i, j;

	matrix = malloc(n * sizeof(*matrix));
	if (matrix == NULL)
		goto out_err;

	for (i = 0; i < n; i++) {
		matrix[i] = malloc(m * sizeof(*matrix[i]));
		if (matrix[i] == NULL)
			goto out_vec;
	}

	return matrix;

out_vec:
	for (j = 0; j < i; j++)
		free(matrix[j]);
	free(matrix);
out_err:
	return NULL;
}

void free_matrix(int **matrix, int n)
{
	int i;

	for (i = 0; i < n; i++)
		free(matrix[i]);
	free(matrix);
}

void fill_matrix(int **matrix, int n, int m, int value)
{
	int i, j;

	for (i = 0; i < n; i++)
		for (j = 0; j < m; j++)
			matrix[i][j] = value;
}
