#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VARIABLES 13
#define MAX_VALUE 10

#define MOD_NUMBER 1007

void debug_print();

struct fixed_array {
	int *arr;
	size_t size, capacity;
};

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

void fa_append(struct fixed_array *self, int x)
{
	self->arr[self->size++] = x;
}

void fill_array(int *v, int n, int value)
{
	int i;

	for (i = 0; i < n; i++)
		v[i] = value;
}

int graph[MAX_VARIABLES][MAX_VARIABLES];
int visited[MAX_VARIABLES];
int parent[MAX_VARIABLES];
int invalid[MAX_VARIABLES];
int iter = 0;

struct fixed_array queue;
struct fixed_array masks;

int dp[1 << MAX_VARIABLES][MAX_VALUE];

struct fixed_array bit_graph[1 << MAX_VARIABLES];

void mark_cycle(int start, int end)
{
	while (end != start) {
		invalid[end] = 1;
		end = parent[end];
	}
}

void build_masks()
{
	size_t old_masks_size;
	size_t i, j;
	int prev_mask, mask;

	fa_init(&masks, 1 << MAX_VARIABLES);
	fa_append(&masks, 0);
	for (i = 0; i < queue.size; i++) {
		prev_mask = 0;
		for (j = 0; j < queue.size; j++)
			if (graph[queue.arr[i]][queue.arr[j]])
				prev_mask |= (1 << j);
		old_masks_size = masks.size;
		for (j = 0; j < old_masks_size; j++) {
			mask = masks.arr[j];
			if ((mask & prev_mask) == prev_mask)
				fa_append(&masks, mask | (1 << i));
		}
	}
}

void visit(int node, int n)
{
	int i;

	visited[node] = iter;
	for (i = 0; i < n; i++)
		if (graph[node][i]) {
			if (visited[i] == 0) {
				parent[i] = node;
				visit(i, n);
			} else if (visited[i] == iter) {
				mark_cycle(i, node);
			}
		}
	if (!invalid[node])
		fa_append(&queue, node);
}

void dfs(int n)
{
	int i;

	fill_array(parent, n, -1);
	fa_init(&queue, MAX_VARIABLES);
	for (i = 0; i < n; i++)
		if (!visited[i]) {
			++iter;
			visit(i, n);
		}
}

void run_dp()
{
	size_t i, k;
	int j;
	int elem;

	for (i = 0; i < masks.size; i++)
		dp[i][0] = 1;

	for (j = 1; j < MAX_VALUE; j++)
		dp[0][j] = 1;

	for (i = 1; i < masks.size; i++) {
		for (j = 1; j < MAX_VALUE; j++) {
			dp[i][j] = dp[i][j - 1];
			for (k = 0; k < bit_graph[i].size; k++) {
				elem = bit_graph[i].arr[k];
				dp[i][j] += dp[elem][j - 1];
				dp[i][j] %= MOD_NUMBER;
			}
		}
	}
}

int main()
{
	int n, m;
	int x, y;
	size_t i, j;
	int k;

	scanf("%d", &n);
	scanf("%d", &m);

	for (k = 0; k < m; k++) {
		scanf("%d", &x);
		scanf("%d", &y);
		if (x != y)
			graph[y][x] = 1;
	}

	dfs(n);
	build_masks();

	for (i = 0; i < masks.size; i++)
		fa_init(&bit_graph[i], masks.size);
	for (i = 0; i < masks.size; i++) {
		for (j = i + 1; j < masks.size; j++)
			if ((masks.arr[i] & masks.arr[j]) == masks.arr[i])
				fa_append(&bit_graph[j], i);
	}

	run_dp();

	//debug_print();
	printf("%d\n", dp[masks.size - 1][MAX_VALUE - 1]);

	for (i = 0; i < masks.size; i++)
		fa_destroy(&bit_graph[i]);

	fa_destroy(&masks);
	fa_destroy(&queue);

	return 0;
}

void debug_print()
{
	size_t i, j;

	printf("queue: ");
	for (i = 0; i < queue.size; i++)
		printf("%d ", queue.arr[i]);
	printf("\n");

	printf("masks: ");
	for (i = 0; i < masks.size; i++)
		printf("%d ", masks.arr[i]);
	printf("\n");

	printf("bit_graph:\n");
	for (i = 0; i < masks.size; i++) {
		printf("(%lu)", i);
		for (j = 0; j < bit_graph[i].size; j++)
			printf(" %d", bit_graph[i].arr[j]);
		printf("\n");
	}

	printf("\n");
	printf("DP:\n");
	for (i = 0; i < masks.size; i++) {
		for (j = 0; j < MAX_VALUE; j++)
			printf("%s%6d", (j == 0) ? "" : " ", dp[i][j]);
		printf("\n");
	}
	printf("\n");
}
