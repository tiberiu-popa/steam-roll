#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 100000
#define NUM_CHARS (26 + 26 + 10)

struct trie {
	size_t forward, backward;
	int prefix, suffix;
	struct trie *children[NUM_CHARS];
};

struct trie root;
size_t v[2 * MAX_LENGTH + 1];
size_t u[MAX_LENGTH];
size_t w[MAX_LENGTH];
char str[MAX_LENGTH + 1];

void trie_init(struct trie *self);
void trie_destroy(struct trie *self);
struct trie * trie_new();
void trie_delete(struct trie *self);
struct trie * trie_get_child(struct trie *self, int pos);

void trie_init(struct trie *self)
{
	memset(self, 0, sizeof(*self));
}

void trie_destroy(struct trie *self)
{
	int i;

	for (i = 0; i < NUM_CHARS; i++)
		if (self->children[i] != NULL)
			trie_delete(self->children[i]);
	memset(self, 0, sizeof(*self));
}

struct trie * trie_new()
{
	struct trie *self;
	self = calloc(1, sizeof(*self));
	return self;
}

void trie_delete(struct trie *self)
{
	trie_destroy(self);
	free(self);
}

struct trie * trie_get_child(struct trie *self, int pos)
{
	struct trie *child;

	child = self->children[pos];
	if (child == NULL)
		self->children[pos] = child = trie_new();

	return child;
}

int get_position(char ch)
{
	int pos = 0;

	if (ch >= 'A' && ch <= 'Z') {
		pos += ch - 'A';
	} else {
		pos += 26;
		if (ch >= 'a' && ch <= 'z') {
			pos += ch -'a';
		} else {
			if (ch >= '0' && ch <= '9')
				pos += ch - '0';
		}
	}

	return pos;
}

void trie_insert_forward(struct trie *self, const char *str, const size_t *v, size_t length)
{
	struct trie *child;
	size_t i;
	int pos;

	for (i = 0; i < length; i++) {
		if (v[i] > self->forward)
			self->forward = v[i];
		pos = get_position(str[i]);
		child = trie_get_child(self, pos);
		if (child != NULL) {
			self = child;
			self->prefix = 1;
		} else {
			break;
		}
	}
}

void trie_insert_backward(struct trie *self, const char *str, const size_t *v, size_t length)
{
	struct trie *child;
	size_t i;
	int pos;

	for (i = length; i > 0;) {
		i--;
		if (v[i] > self->backward)
			self->backward = v[i];
		pos = get_position(str[i]);
		child = trie_get_child(self, pos);
		if (child != NULL) {
			self = child;
			self->suffix = 1;
		} else {
			break;
		}
	}
}

static size_t min(size_t x, size_t y)
{
	return (x < y) ? x : y;
}

static size_t max(size_t x, size_t y)
{
	return (x > y) ? x : y;
}

size_t trie_find_maximum(struct trie *self, size_t depth)
{
	struct trie *child;
	size_t max_length, candidate;
	int i;

	if (self->prefix && self->suffix)
		max_length = 2 * depth + max(self->forward, self->backward);
	else
		max_length = 0;

	for (i = 0; i < NUM_CHARS; i++) {
		child = self->children[i];
		if (child != NULL) {
			candidate = trie_find_maximum(child, depth + 1);
			if (candidate > max_length)
				max_length = candidate;
		}
	}

	return max_length;
}

void process_palindrome(const char *str, size_t *u, size_t *w)
{
	size_t palindrome_length;
	size_t num_elems;
	size_t length;
	size_t i, j, k;
	int found;

	length = strlen(str);
	palindrome_length = 0;
	num_elems = 0;
	i = 0;
	while (i <= length) {
		while (i > palindrome_length && str[i] == str[i - palindrome_length - 1]) {
			palindrome_length += 2;
			i++;
		}
		v[num_elems++] = palindrome_length;
		found = 0;
		for (j = palindrome_length; j > 0;) {
			j--;
			k = 2 * (i - palindrome_length) + j;
			if (v[k] == j) {
				palindrome_length = j;
				found = 1;
				break;
			}
			v[num_elems++] = min(j, v[k]);
		}
		if (!found) {
			palindrome_length = 1;
			i++;
		}
	}

	memset(u, 0, length * sizeof(*u));
	memset(w, 0, length * sizeof(*w));

	for (i = 0; i < num_elems; i++)
		if (v[i] > 0) {
			j = (i - v[i]) / 2;
			k = j + v[i] - 1;
			if (v[i] > u[j])
				u[j] = v[i];
			if (v[i] > w[k])
				w[k] = v[i];
		}

	trie_insert_forward(&root, str, u, length);
	trie_insert_backward(&root, str, w, length);
}

void solve_problem()
{
	int n, m;
	int i;

	if (scanf("%d %d", &n, &m) != 2)
		exit(EXIT_FAILURE);

	trie_init(&root);
	for (i = 0; i < n; i++) {
		if (scanf("%s", str) != 1)
			exit(EXIT_FAILURE);
		process_palindrome(str, u, w);
	}
	printf("%lu\n", (unsigned long) trie_find_maximum(&root, 0));
	trie_destroy(&root);
}

int main()
{
	solve_problem();
	return 0;
}
