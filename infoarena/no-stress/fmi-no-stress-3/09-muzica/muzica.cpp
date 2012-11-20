#include <algorithm>
#include <vector>
#include <cstdio>
#include <cstdlib>

using namespace std;

#define MAX_N 10000

int a, b, c, d, e;
vector<int> hmap[MAX_N];

unsigned int hash(int x)
{
	unsigned int n = (unsigned int) x;
	n = (n + 0x7ed55d16) + (n << 12);
	n = (n ^ 0xc761c23c) ^ (n >> 19);
	n = (n + 0x165667b1) + (n << 5);
	n = (n + 0xd3a2646c) ^ (n << 9);
	n = (n + 0xfd7046c5) + (n << 3);
	n = (n ^ 0xb55a4f09) ^ (n >> 16);
	return n % MAX_N;
}

void read_input(const char *filename, int &n, int &m)
{
	FILE *f;

	f = fopen(filename, "rt");
	if (f == NULL) {
		fprintf(stderr, "Could not open file: %s\n", filename);
		exit(EXIT_FAILURE);
	}

	if (fscanf(f, "%d %d", &n, &m) != 2)
		exit(EXIT_FAILURE);

	if (fscanf(f, "%d %d %d %d %d", &a, &b, &c, &d, &e) != 5)
		exit(EXIT_FAILURE);

	for (int i = 0; i < n; i++) {
		int x;
		if (fscanf(f, "%d", &x) != 1)
			exit(EXIT_FAILURE);
		hmap[hash(x)].push_back(x);
	}

	fclose(f);
}

void write_output(const char *filename, int result)
{
	FILE *f;

	f = fopen(filename, "wt");
	if (f == NULL) {
		fprintf(stderr, "Could not open file: %s\n", filename);
		exit(EXIT_FAILURE);
	}

	fprintf(f, "%d\n", result);
	fclose(f);
}

void update_result(int n, int &result)
{
	vector<int> &bucket = hmap[hash(n)];
	vector<int>::iterator it = find(bucket.begin(), bucket.end(), n);
	if (it != bucket.end()) {
		++result;
		bucket.erase(it);
	}
}

int solve_problem(int n, int m)
{
	int result = 0;
	(void) n;

	update_result(a, result);
	update_result(b, result);
	for (int i = 3; i <= m; i++) {
		long long left = c;
		left *= b;
		long long right = d;
		right *= a;
		int tmp = (left + right) % e;
		a = b;
		b = tmp;
		update_result(b, result);
	}

	return result;
}

int main()
{
	int n, m;
	int result;

	read_input("muzica.in", n, m);
	result = solve_problem(n, m);
	write_output("muzica.out", result);

	return 0;
}
