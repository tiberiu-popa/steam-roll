#include <algorithm>
#include <list>
#include <cstdio>
#define NDEBUG
#include <cassert>

#define MAXSIZE 200000

using namespace std;

list<int> nodes[MAXSIZE];
list<int> inorder;
int n, root;
unsigned int result;

int print_list(const list<int>& v)
{
	for (list<int>::const_iterator iter = v.begin(); iter != v.end(); ++iter)
		printf("%d ", *iter);
	printf("\n");

	return 1;
}

int print_children(list<int> * nodes)
{
	int i;

	for (i = 0; i < n; i++) {
		printf("children<%d>: ", i);
		print_list(nodes[i]);
	}

	return 1;
}

void read_from_file(const char * filename)
{
	FILE *f;
	int i, parent;

	f = fopen(filename, "rt");
	assert(f != NULL);
	fscanf(f, "%d", &n);
	for (i = 0; i < n; i++) {
		fscanf(f, "%d", &parent);
		if (parent == 0)
			root = i;
		else
			nodes[parent - 1].push_back(i);
	}
	fclose(f);
}

pair<size_t, bool> inorder_traversal(int k)
{
	size_t sum, lower_limit;
	size_t num_paths = 0;

	inorder.push_back(k);
	sum = 0;
	for (list<int>::const_iterator iter = nodes[k].begin(); iter != nodes[k].end(); ++iter) {
		pair<size_t, bool> value = inorder_traversal(*iter);
		sum += value.first;
		if (value.second)
			num_paths++;
	}
	lower_limit = (k == root) ? 2 : 1;
	sum += max(num_paths, lower_limit) - lower_limit;

	return pair<size_t, bool>(sum, num_paths <= 1);
}

void solve()
{
	unsigned int deg_sum;

	deg_sum = inorder_traversal(root).first;

	assert(print_children(nodes));
	assert(printf("inorder: ") && print_list(inorder));

	result = (deg_sum << 1) | 1;
}

void write_to_file(const char * filename)
{
	FILE *f;

	f = fopen(filename, "wt");
	fprintf(f, "%u\n", result);
	fclose(f);
}

int main()
{
	read_from_file("tree.in");
	solve();
	write_to_file("tree.out");
	return 0;
}
