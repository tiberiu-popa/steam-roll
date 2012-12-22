#include <list>
#include <cstdio>
#include <cstdlib>

#define MAX_N 400000
#define ARRAY_SIZE 5

using namespace std;

struct static_array {
	int array[ARRAY_SIZE];
	size_t size;
};

int v[MAX_N];
struct static_array table[MAX_N];

void sa_insert(struct static_array *self, int elem)
{
	self->array[self->size++] = elem;
}

void read_input(const char *filename, int &n, int &m, int &g, int &h)
{
	FILE *f;

	f = fopen(filename, "rt");
	if (f == NULL) {
		fprintf(stderr, "Could not open file: %s\n", filename);
		exit(EXIT_FAILURE);
	}

	if (fscanf(f, "%d %d", &n, &m) != 2)
		exit(EXIT_FAILURE);
	for (int i = 0; i < n; i++)
		if (fscanf(f, "%d", &v[i]) != 1)
			exit(EXIT_FAILURE);
	if (fscanf(f, "%d %d", &g, &h) != 2)
		exit(EXIT_FAILURE);

	fclose(f);
}

void write_output(const char *filename, int n, int m, int g, int h)
{
	FILE *f;

	f = fopen(filename, "wt");
	if (f == NULL) {
		fprintf(stderr, "Could not open file: %s\n", filename);
		exit(EXIT_FAILURE);
	}

	int p = 1;
	int k = 1;
	for (int i = 0; i < m; i++) {
		p = 1 + (i + 1 + p * g) % n;
		k = 1 + (i + 1 + k * h) % 5;
		size_t idx = (size_t) k;
		const static_array &row = table[p - 1];
		if (row.size >= idx)
			fprintf(f, "%d\n", 1 + row.array[idx - 1]);
		else
			fprintf(f, "%d\n", 0);
	}

	fclose(f);
}

void solve_problem(int n)
{
	list< pair<int, int> > zone;

	for (int i = 0; i < n; i++) {
		int elem = v[i];
		list< pair<int, int> >::iterator it;
		for (it = zone.begin(); it != zone.end();) {
			int cand = it->first;
			if (cand < elem) {
				int pos = it->second;
				sa_insert(&table[pos], i);
				if (table[pos].size == 5) {
					it = zone.erase(it);
				} else {
					++it;
				}
			} else {
				break;
			}
		}
		zone.insert(it, pair<int, int>(elem, i));
	}
}

int main()
{
	int n, m;
	int g, h;

	read_input("ismquery.in", n, m, g, h);
	solve_problem(n);
	write_output("ismquery.out", n, m, g, h);

	return 0;
}
