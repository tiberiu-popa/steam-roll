#include <stdio.h>
#include <stdlib.h>

struct fixed_array {
	unsigned int *v;
	size_t size;
};

void read_fixed_array(struct fixed_array *self)
{
	size_t n, i;

	scanf("%d", &n);
	self->v = malloc(n * sizeof(*self->v));
	if (self->v != NULL) {
		for (i = 0; i < n; i++)
			scanf("%u", &self->v[i]);
		self->size = n;
	}
}

int main()
{
	struct fixed_array arr;
	size_t i;
	unsigned int candidate, elem, count;
	int k;

	read_fixed_array(&arr);

	for (k = 31; k >=0; k--) {
		candidate = 0;
		count = 0;
		for (i = 0; i < arr.size; i++) {
			elem = arr.v[i] >> k;
			if (count == 0) {
				candidate = elem;
			}
			if (elem == candidate)
				count++;
			else
				count--;
		}
		count = 0;
		for (i = 0; i < arr.size; i++) {
			elem = arr.v[i] >> k;
			if (elem == candidate)
				count++;
		}
		if (2 * count <= arr.size)
			break;
	}
	printf("%d\n", k);

	return 0;
}
