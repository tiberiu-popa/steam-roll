#include <algorithm>
#include <vector>
#include <cstdio>
#include <cstdlib>

using namespace std;

#define MAX_N 500000

int preorder[MAX_N];
int inorder[MAX_N];
int inv_inorder[MAX_N];
int children[MAX_N][3];

void read_input(const char *filename, int &n)
{
	FILE *f;

	f = fopen(filename, "rt");
	if (f == NULL) {
		fprintf(stderr, "Could not open file: %s\n", filename);
		exit(EXIT_FAILURE);
	}

	if (fscanf(f, "%d", &n) != 1)
		exit(EXIT_FAILURE);

	for (int i = 0; i < n; i++) {
		int x;
		if (fscanf(f, "%d", &x) != 1)
			exit(EXIT_FAILURE);
		inorder[i] = x - 1;
		inv_inorder[x - 1] = i;
	}

	for (int i = 0; i < n; i++) {
		int x;
		if (fscanf(f, "%d", &x) != 1)
			exit(EXIT_FAILURE);
		preorder[i] = x - 1;
	}

	fclose(f);
}

void write_output(const char *filename, int n)
{
	FILE *f;

	f = fopen(filename, "wt");
	if (f == NULL) {
		fprintf(stderr, "Could not open file: %s\n", filename);
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < n; i++) {
		int j;
		for (j = 1; j <= children[i][0]; j++)
			fprintf(f, "%d ", children[i][j]);
		for (; j <= 2; j++)
			fprintf(f, "%d ", 0);
		fprintf(f, "\n");
	}
	fclose(f);
}

void rec_tree(int parent, int pre_left, int pre_right, int in_left, int in_right)
{
	if (pre_left <= pre_right) {
		int root = preorder[pre_left];
		if (parent >= 0) {
			children[parent][++children[parent][0]] = root + 1;
		}
		if (pre_left < pre_right) {
			int k = inv_inorder[root];
			rec_tree(root, pre_left + 1, pre_left + k - in_left, in_left, k - 1);
			rec_tree(root, pre_left + k - in_left + 1, pre_right, k + 1, in_right);
		}
	} else {
		if (parent >= 0) {
			children[parent][++children[parent][0]] = 0;
		}
	}
}

void solve_problem(int n)
{
	rec_tree(-1, 0, n - 1, 0, n - 1);
	printf("\n");
}

int main()
{
	int n;

	read_input("curatenie.in", n);
	solve_problem(n);
	write_output("curatenie.out", n);

	return 0;
}
