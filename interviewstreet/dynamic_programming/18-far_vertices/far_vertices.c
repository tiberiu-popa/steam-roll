#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_N 100

struct fixed_array {
	int *arr;
	size_t size, capacity;
};

int fa_init(struct fixed_array *self, size_t capacity);
void fa_destroy(struct fixed_array *self);
int fa_empty(const struct fixed_array *self);
void fa_append(struct fixed_array *self, int x);
int fa_pop_back(struct fixed_array *self);
void fa_clear(struct fixed_array *self);

int graph[MAX_N][MAX_N];
struct fixed_array neighbours[MAX_N];

static size_t max(size_t x, size_t y)
{
	return (x > y) ? x : y;
}

void read_input(int *ref_n, int *ref_k)
{
	int n, k;
	int x, y;
	int i;

	if (scanf("%d %d", &n, &k) != 2)
		exit(EXIT_FAILURE);
	for (i = 1; i < n; i++) {
		if (scanf("%d %d", &x, &y) != 2)
			exit(EXIT_FAILURE);
		--x;
		--y;
		graph[x][y] = 1;
		graph[y][x] = 1;
	}

	for (i = 0; i < n; i++)
		fa_init(&neighbours[i], (size_t) n);

	*ref_n = n;
	*ref_k = k;
}

void write_output(int n)
{
	int i;

	for (i = 0; i < n; i++)
		fa_destroy(&neighbours[i]);
}

void intersect_sorted(struct fixed_array *res, const struct fixed_array *x,
	const struct fixed_array *y)
{
	size_t i, j;
	int ex, ey;

	fa_clear(res);
	for (i = 0, j = 0; i < x->size && j < y->size;) {
		ex = x->arr[i];
		ey = y->arr[j];
		if (ex == ey) {
			fa_append(res, ex);
			i++;
			j++;
		} else if (ex < ey) {
			i++;
		} else {
			j++;
		}
	}
}

size_t clique(struct fixed_array *array, size_t size, size_t max_size)
{
	struct fixed_array next_array;
	int node;

	if (fa_empty(array))
		return max(size, max_size);

	fa_init(&next_array, array->size - 1);

	while (!fa_empty(array)) {
		if (size + array->size <= max_size)
			break;

		node = fa_pop_back(array);
		intersect_sorted(&next_array, array, &neighbours[node]);
		max_size = clique(&next_array, size + 1, max_size);
	}

	fa_destroy(&next_array);

	return max_size;
}

void solve_problem(int n, int k)
{
	struct fixed_array nodes;
	size_t max_clique;
	int cand;
	int i, j, t;

	for (t = 0; t < n; t++)
		for (i = 0; i < n; i++)
			if (graph[i][t] > 0) {
				for (j = 0; j < n; j++)
					if (graph[t][j] > 0) {
						cand = graph[i][t] + graph[t][j];
						if (graph[i][j] == 0 || cand < graph[i][j])
							graph[i][j] = cand;
					}
			}

	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
			graph[i][j] = (graph[i][j] >= 1 && graph[i][j] <= k);

	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
			if (j != i && graph[i][j] != 0)
				fa_append(&neighbours[i], j);

	fa_init(&nodes, (size_t) n);
	for (i = 0; i < n; i++)
		fa_append(&nodes, i);

	max_clique = clique(&nodes, 0, 0);
	printf("%d\n", n - (int) max_clique);

	fa_destroy(&nodes);
#if 0
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++)
			printf("%d ", graph[i][j]);
		printf("\n");
	}
#endif
}

int main()
{
	int n, k;

	read_input(&n, &k);
	solve_problem(n, k);
	write_output(n);

	return 0;
}

int fa_init(struct fixed_array *self, size_t capacity)
{
	self->arr = malloc(capacity * sizeof(*self->arr));
	if (self->arr != NULL) {
		self->size = 0;
		self->capacity = capacity;
		return 0;
	} else {
		return 1;
	}
}

void fa_destroy(struct fixed_array *self)
{
	free(self->arr);
	memset(self, 0, sizeof(*self));
}

int fa_empty(const struct fixed_array *self)
{
	return self->size == 0;
}

void fa_append(struct fixed_array *self, int x)
{
	self->arr[self->size++] = x;
}

int fa_pop_back(struct fixed_array *self)
{
	if (fa_empty(self)) {
		exit(EXIT_FAILURE);
	} else {
		return self->arr[--self->size];
	}
}

void fa_clear(struct fixed_array *self)
{
	self->size = 0;
}
