#include <algorithm>
#include <deque>
#include <cstdio>
#include <cstdlib>

using namespace std;

#define MAX_N 1000000
#define MAX_C 1000

deque<int> books;
int op[MAX_N];

void read_input(const char *filename, int &n, int &c)
{
	FILE *f;

	f = fopen(filename, "rt");
	if (f == NULL) {
		fprintf(stderr, "Could not open file: %s\n", filename);
		exit(EXIT_FAILURE);
	}

	if (fscanf(f, "%d %d", &n, &c) != 2)
		exit(EXIT_FAILURE);

	for (int i = 0; i < c; i++) {
		int x;
		if (fscanf(f, "%d", &x) != 1)
			exit(EXIT_FAILURE);
		books.push_back(x);
	}

	for (int i = 0; i < n; i++) {
		int x;
		if (fscanf(f, "%d", &x) != 1)
			exit(EXIT_FAILURE);
		op[i] = x;
	}

	fclose(f);
}

void write_output(const char *filename, bool result)
{
	FILE *f;

	f = fopen(filename, "wt");
	if (f == NULL) {
		fprintf(stderr, "Could not open file: %s\n", filename);
		exit(EXIT_FAILURE);
	}

	while (!books.empty()) {
		if (result) {
			int x = books.front();
			fprintf(f, "%d\n", x);
			books.pop_front();
		} else {
			int x = books.back();
			fprintf(f, "%d\n", x);
			books.pop_back();
		}
	}

	fclose(f);
}

bool solve_problem(int n, int c)
{
	bool is_front = true;
	(void) c;

	for (int i = 0; i < n; i++)
		if (op[i] == -1) {
			is_front = !is_front;
		} else {
			if (is_front)
				books.push_front(op[i]);
			else
				books.push_back(op[i]);
		}

	return is_front;
}

int main()
{
	int n, c;
	bool result;

	read_input("carti3.in", n, c);
	result = solve_problem(n, c);
	write_output("carti3.out", result);

	return 0;
}
