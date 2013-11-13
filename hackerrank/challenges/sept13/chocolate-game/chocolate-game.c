#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_N 100000

#define s_calloc(size) calloc(1, size)

#define s_malloc_object(obj) \
		((obj) = malloc(sizeof(*(obj))))

#define s_realloc_vector(p, size) \
		(s_reallocv(p, size, sizeof(*(p))))

#define s_free_object free

#define SIZE_MAX ((size_t) (-2))

#define SIZE_OVERFLOWS(nmemb, size) \
		((nmemb) > SIZE_MAX / (size))

#define STATUS_IS_OK(status) \
		((status) == STATUS_OK)

#define RETURN_STATUS_IF_FAIL(expr) \
		do { \
			status_t prv_status = (expr); \
			if (!STATUS_IS_OK(prv_status)) \
				return prv_status; \
		} while (0)

#define RETURN_VALUE_IF_FAIL(pred, val) \
		do { \
			if (!(pred)) \
				return val; \
		} while (0)

#define RETURN_NULL_IF_FAIL(pred) \
		RETURN_VALUE_IF_FAIL(pred, NULL)

#define DYNAMIC_ARRAY_DEFAULT_SIZE 10

#define HASH_MAP_LOG_BUCKETS 11
#define HASH_MAP_NUM_BUCKETS (1 << HASH_MAP_LOG_BUCKETS)
#define HASH_MAP_MASK (HASH_MAP_NUM_BUCKETS - 1)

enum status {
	STATUS_OK,
	STATUS_ENOMEM,
	STATUS_ENOSIZE,
	STATUS_EBOUNDS
};

typedef enum status status_t;

typedef struct dynarray dynarray_t;

typedef int hash_map_key_t;

typedef int hash_map_value_t;

struct hash_map_entry {
	hash_map_key_t key;
	hash_map_value_t value;
	unsigned int hash;
};
typedef struct hash_map_entry hash_map_entry_t;

typedef hash_map_entry_t dynarray_element_t;

struct dynarray {
	size_t size;
	size_t capacity;
	dynarray_element_t *array;
};

struct hash_map {
	size_t size;
	dynarray_t buckets[HASH_MAP_NUM_BUCKETS];
};
typedef struct hash_map hash_map_t;

dynarray_t * da_new()
		__attribute__((warn_unused_result));

void da_delete(dynarray_t **self)
		__attribute__((nonnull (1)));

status_t da_init(dynarray_t *self)
		__attribute__((nonnull (1)));

void da_destroy(dynarray_t *self)
		__attribute__((nonnull (1)));

size_t da_size(const dynarray_t *self)
		__attribute__((nonnull (1), pure));

int da_empty(const dynarray_t *self)
		__attribute__((nonnull (1), pure));

dynarray_element_t da_get(const dynarray_t *self, size_t index)
		__attribute__((nonnull (1), pure));

dynarray_element_t da_get_front(const dynarray_t *self)
		__attribute__((nonnull (1), pure));

dynarray_element_t da_get_back(const dynarray_t *self)
		__attribute__((nonnull (1), pure));

status_t da_try_get(const dynarray_t *self, size_t index,
		dynarray_element_t *x) __attribute__((nonnull (1, 3), warn_unused_result));

void da_set(dynarray_t *self, size_t index, dynarray_element_t x)
		__attribute__((nonnull (1)));

void da_set_front(dynarray_t *self, dynarray_element_t x)
		__attribute__((nonnull (1)));

status_t da_try_set(dynarray_t *self, size_t index, dynarray_element_t x)
		__attribute__((nonnull (1), warn_unused_result));

status_t da_try_set_front(dynarray_t *self, dynarray_element_t x)
		__attribute__((nonnull (1), warn_unused_result));

dynarray_element_t * da_ptr(dynarray_t *self, size_t index)
		__attribute__((nonnull (1), pure));

status_t da_append(dynarray_t *self, dynarray_element_t x)
		__attribute__((nonnull (1)));

void da_clear(dynarray_t *self)
		__attribute__((nonnull (1)));

status_t da_reserve(dynarray_t *self, size_t capacity)
		__attribute__((nonnull (1), warn_unused_result));

status_t da_expand(struct dynarray *self)
		__attribute__((nonnull (1), warn_unused_result));

void da_reverse(struct dynarray *self)
		__attribute__((nonnull (1)));

hash_map_t * hm_new();
void hm_delete(hash_map_t ** self);
void hm_clear(hash_map_t *self);
void hm_put(hash_map_t * self, hash_map_key_t k, hash_map_value_t v);
int hm_try_get(hash_map_t * self, hash_map_key_t k, hash_map_value_t * v);

unsigned int hash_int(int x);
unsigned int djb2_hash_string(const char * str);
unsigned int djb2_hash(const void * p, size_t size);
unsigned int oat_hash(const void * p, size_t size);
unsigned int hash_fixed(const void * p, size_t size);

int d[MAX_N];
int v[MAX_N];

int skip_whitespace()
{
	int ch;
	while (1) {
		ch = getchar_unlocked();
		if (ch != ' ' && ch != '\n')
			break;
	}
	return ch;
}

int read_unsigned_integer_int()
{
	int result = 0;
	int ch = skip_whitespace();
	while (ch >= '0' && ch <= '9') {
		result = 10 * result + (ch - '0');
		ch = getchar_unlocked();
	}
	return result;
}

int read_signed_integer_int()
{
	int result = 0;
	int flip = 0;
	int ch = skip_whitespace();
	if (ch == '-') {
		flip = 1;
		ch = skip_whitespace();
	}
	while (ch >= '0' && ch <= '9') {
		result = 10 * result + (ch - '0');
		ch = getchar_unlocked();
	}
	return flip ? -result : result;
}

void * s_reallocv(void * v, size_t nmemb, size_t size)
{
	if (SIZE_OVERFLOWS(nmemb, size))
		return NULL;
	else
		return realloc(v, nmemb * size);
}

int solve_problem()
{
	hash_map_t *cnt;
	hash_map_value_t entry, other;
	int i, n;

	cnt = hm_new();

	n = read_unsigned_integer_int();

	int prev_x = -1;
	for (i = 0; i < n; i++) {
		int x = read_unsigned_integer_int();
		if (prev_x > 0)
			v[i - 1] = x - prev_x;
		d[i] = prev_x = x;
	}

	long long result = 0;
	int val = 0;
	int total = 1;
	if (!hm_try_get(cnt, val, &entry))
		entry = 0;
	hm_put(cnt, val, entry + 1);
	for (i = n - 2; i >= 0; i -= 2) {
		val ^= v[i];
		if (!hm_try_get(cnt, val, &entry))
			entry = 0;
		result += total - entry;
		if (i > 0) {
			if (!hm_try_get(cnt, val ^ d[i - 1], &other))
				other = 0;
			result += total - other;
		}
		hm_put(cnt, val, entry + 1);
		++total;
	}

	hm_clear(cnt);

	val = 0;
	total = 1;
	if (!hm_try_get(cnt, val, &entry))
		entry = 0;
	hm_put(cnt, val, entry + 1);
	for (i = n - 3; i >= 0; i -= 2) {
		val ^= v[i];
		if (!hm_try_get(cnt, val, &entry))
			entry = 0;
		result += total - entry;
		if (i > 0) {
			if (!hm_try_get(cnt, val ^ d[i - 1], &other))
				other = 0;
			result += total - other;
		}
		hm_put(cnt, val, entry + 1);
		++total;
	}

	printf("%lld\n", result);

	hm_delete(&cnt);

	return 0;
}

int main()
{
	solve_problem();
	return 0;
}

dynarray_t * da_new()
{
	return s_calloc(sizeof(dynarray_t));
}

void da_delete(dynarray_t **self)
{
	free((*self)->array);
	free(*self);
	*self = NULL;
}

status_t da_init(dynarray_t *self)
{
	memset(self, 0, sizeof(*self));
	return STATUS_OK;
}

void da_destroy(dynarray_t *self)
{
	free(self->array);
	memset(self, 0, sizeof(*self));
}

size_t da_size(const dynarray_t *self)
{
	return self->size;
}

int da_empty(const dynarray_t *self)
{
	return !self->size;
}

dynarray_element_t da_get(const dynarray_t *self, size_t index)
{
	if (index < self->size) {
		return self->array[index];
	} else {
		fprintf(stderr, "error<dynamic_array>: index %zu out of bounds [%zu, %zu[\n",
				index, (size_t) 0, self->size);
		exit(EXIT_FAILURE);
	}
}

dynarray_element_t da_get_front(const dynarray_t *self)
{
	if (self->size != 0) {
		return self->array[0];
	} else {
		// TODO put errmsg here
		exit(EXIT_FAILURE);
	}
}

dynarray_element_t da_get_back(const dynarray_t *self)
{
	if (self->size != 0) {
		return self->array[self->size - 1];
	} else {
		// TODO put errmsg here
		exit(EXIT_FAILURE);
	}
}

status_t da_try_get(const dynarray_t *self, size_t index, dynarray_element_t *x)
{
	if (index < self->size) {
		*x = self->array[index];
		return STATUS_OK;
	} else {
		return STATUS_EBOUNDS;
	}
}

void da_set(dynarray_t *self, size_t index, dynarray_element_t x)
{
	if (index < self->size) {
		self->array[index] = x;
	} else {
		fprintf(stderr, "error<dynamic_array>: index %zu out of bounds [%zu, %zu[\n",
				index, (size_t) 0, self->size);
		exit(EXIT_FAILURE);
	}
}

void da_set_front(dynarray_t *self, dynarray_element_t x)
{
	if (self->size != 0) {
		self->array[0] = x;
	} else {
		// TODO put errmsg here
		exit(EXIT_FAILURE);
	}
}

status_t da_try_set(dynarray_t *self, size_t index, dynarray_element_t x)
{
	if (index < self->size) {
		self->array[index] = x;
		return STATUS_OK;
	} else {
		return STATUS_EBOUNDS;
	}
}

status_t da_try_set_front(dynarray_t *self, dynarray_element_t x)
{
	if (self->size != 0) {
		self->array[0] = x;
		return STATUS_OK;
	} else {
		return STATUS_EBOUNDS;
	}
}

dynarray_element_t * da_ptr(dynarray_t *self, size_t index)
{
	if (index < self->size) {
		return &self->array[index];
	}
	else {
		// ERROR MSG
		exit(EXIT_FAILURE);
	}
}

/* Append the element 'x' to the dynamic array */
status_t da_append(dynarray_t *self, dynarray_element_t x)
{
	if (self->size == self->capacity)
		RETURN_STATUS_IF_FAIL(da_expand(self));

	self->array[self->size++] = x;
	return STATUS_OK;
}

void da_clear(dynarray_t *self)
{
	self->size = 0;
}

status_t da_expand(struct dynarray *self)
{
	void *p;
	size_t new_capacity;

	if (self->capacity > (SIZE_MAX >> 1)) {
		if (self->capacity == SIZE_MAX)
			return STATUS_ENOSIZE;
		new_capacity = SIZE_MAX;
	} else {
		new_capacity = self->capacity << 1;
		if (new_capacity < DYNAMIC_ARRAY_DEFAULT_SIZE)
			new_capacity = DYNAMIC_ARRAY_DEFAULT_SIZE;
	}

	p = s_realloc_vector(self->array, new_capacity);
	if (p == NULL)
		return STATUS_ENOMEM;

	self->array = p;
	self->capacity = new_capacity;

	return STATUS_OK;
}

hash_map_t * hm_new()
{
	hash_map_t * self;
	size_t i, j;

	RETURN_NULL_IF_FAIL(s_malloc_object(self) != NULL);

	for (i = 0; i < HASH_MAP_NUM_BUCKETS; i++)
		if (!STATUS_IS_OK(da_init(&self->buckets[i]))) {
			for (j = 0; j < i; j++)
				da_destroy(&self->buckets[j]);
			s_free_object(self);
			return NULL;
		}

	self->size = 0;

	return self;
}

void hm_print(const hash_map_t * self)
{
	hash_map_entry_t entry;
	size_t i, j, bucket_size;

	for (i = 0; i < HASH_MAP_NUM_BUCKETS; i++) {
		bucket_size = da_size(&self->buckets[i]);
		printf("%zu: %zu\n", i, bucket_size);
		for (j = 0; j < bucket_size; j++) {
			entry = da_get(&self->buckets[i], j);
			printf("  %u\n", entry.hash);
		}
	}
}

void hm_delete(hash_map_t ** self)
{
	size_t i;

	for (i = 0; i < HASH_MAP_NUM_BUCKETS; i++)
		da_destroy(&(*self)->buckets[i]);
	s_free_object(*self);
	*self = NULL;
}

void hm_clear(hash_map_t *self)
{
	size_t i;

	for (i = 0; i < HASH_MAP_NUM_BUCKETS; i++)
		da_clear(&self->buckets[i]);
}

void hm_put(hash_map_t *self, hash_map_key_t k, hash_map_value_t v)
{
	dynarray_t *array;
	hash_map_entry_t entry, *ep;
	size_t i, size;
	unsigned int hash_value, hash_bucket;

	hash_value = hash_int(k);
	hash_bucket = hash_value & HASH_MAP_MASK;
	array = &self->buckets[hash_bucket];
	size = da_size(array);
	for (i = 0; i < size; i++) {
		ep = da_ptr(array, i);
		if (memcmp(&ep->key, &k, sizeof(k)) == 0) {
			ep->value = v;
			return;
		}
	}

	entry.key = k;
	entry.value = v;
	entry.hash = hash_value;
	da_append(&self->buckets[hash_bucket], entry);
	self->size++;
}

int hm_try_get(hash_map_t *self, hash_map_key_t k, hash_map_value_t *v)
{
	dynarray_t * array;
	hash_map_entry_t * ep;
	size_t i, size;
	unsigned int hash_value, hash_bucket;

	hash_value = hash_int(k);
	hash_bucket = hash_value & HASH_MAP_MASK;
	array = &self->buckets[hash_bucket];
	size = da_size(array);
	for (i = 0; i < size; i++) {
		ep = da_ptr(array, i);
		if (ep->hash == hash_value && memcmp(&ep->key, &k, sizeof(k)) == 0) {
			*v = ep->value;
			return 1;
		}
	}

	return 0;
}

unsigned int hash_int(int x)
{
	unsigned int n = (unsigned int) x;
	n = (n + 0x7ed55d16) + (n << 12);
	n = (n ^ 0xc761c23c) ^ (n >> 19);
	n = (n + 0x165667b1) + (n << 5);
	n = (n + 0xd3a2646c) ^ (n << 9);
	n = (n + 0xfd7046c5) + (n << 3);
	n = (n ^ 0xb55a4f09) ^ (n >> 16);
	return n;
}

/**
 * Hash function based on djb2 by Dan Bernstein
 * http://www.cse.yorku.ca/~oz/hash.html
 *
 * @param str the string to be hashed
 * @return the hash key
 */
unsigned int djb2_hash_string(const char *str)
{
	return djb2_hash(str, strlen(str));
}

/**
 * Hash function based on djb2 by Dan Bernstein
 * http://www.cse.yorku.ca/~oz/hash.html
 *
 * @param p pointer to the block of memory to be hashed
 * @param size the number of bytes to be hashed
 * @return the hash key
 */
unsigned int djb2_hash(const void *p, size_t size)
{
	const unsigned char * str;
	size_t i;
	unsigned int hash, c;

	str = (const unsigned char *) p;
	hash = 5381;

	for (i = 0; i < size; i++, str++) {
		c = *str;
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	}

	return hash;
}

unsigned int oat_hash(const void *p, size_t size)
{
	const unsigned char * str;
	size_t i;
	unsigned int hash, c;

	str = p;
	hash = 0;

	for (i = 0; i < size; i++, str++) {
		c = *str;
		hash += c;
		hash += (hash << 10);
		hash ^= (hash >> 6);
	}

	hash += (hash << 3);
	hash ^= (hash >> 11);
	hash += (hash << 15);

	return hash;
}

unsigned int hash_fixed(const void *p, size_t size)
{
	return oat_hash(p, size);
}
