#include <algorithm>
#include <map>
#include <vector>
#include <cstdio>
#include <cstdlib>

#define MAX_N 1000

using namespace std;

struct rectangle {
	pair<int, int> top_left;
	pair<int, int> bottom_right;
};

struct node_entry {

	int x;
	pair<int, int> y;
	bool endpoint;

	node_entry() : x(0), y(pair<int, int>(0, 0)), endpoint(false) { }

	node_entry(int x, int ly, int ry, bool endpoint) :
		x(x), y(pair<int, int>(ly, ry)), endpoint(endpoint) { }

	bool operator < (const node_entry &other) const
	{
		if (x != other.x)
			return x < other.x;
		if (endpoint != other.endpoint)
			return endpoint < other.endpoint;
		return y < other.y;
	}

};

struct tree_node {
	short count;
	short extra;
};

node_entry nodes[MAX_N << 1];
rectangle rect[MAX_N];

void read_input(const char *filename, int &n, int &k)
{
	FILE *f;

	f = fopen(filename, "rt");
	if (f == NULL) {
		fprintf(stderr, "Could not open file: %s\n", filename);
		exit(EXIT_FAILURE);
	}

	if (fscanf(f, "%d %d", &n, &k) != 2)
		exit(EXIT_FAILURE);
	for (int i = 0; i < n; i++) {
		int x, y, z, t;
		if (fscanf(f, "%d %d %d %d", &x, &y, &z, &t) != 4)
			exit(EXIT_FAILURE);
		if (x == z || y == t) {
			--i;
			--n;
			continue;
		}
		rect[i].top_left = pair<int, int>(min(x, z), min(y, t));
		rect[i].bottom_right = pair<int, int>(max(x, z), max(y, t));
	}
	fclose(f);
}

void write_output(const char *filename, long long result)
{
	FILE *f;

	f = fopen(filename, "wt");
	if (f == NULL) {
		fprintf(stderr, "Could not open file: %s\n", filename);
		exit(EXIT_FAILURE);
	}

	fprintf(f, "%lld\n", result);
	fclose(f);
}

void trim_unique(vector<int> &v)
{
	vector<int>::iterator it = unique(v.begin(), v.end());
	vector<int>::difference_type diff = it - v.begin();
	v.resize((vector<int>::size_type) diff);
}

long long solve_problem(int n, int k)
{
	map<int, size_t> reverse_index;
	vector<int> yc;
	int overlap[2 * MAX_N];
	long long result = 0;

	for (int i = 0; i < n; i++) {
		nodes[2 * i] = node_entry(rect[i].top_left.first,
			rect[i].top_left.second, rect[i].bottom_right.second, false);
		nodes[2 * i + 1] = node_entry(rect[i].bottom_right.first,
			rect[i].top_left.second, rect[i].bottom_right.second, true);
		yc.push_back(rect[i].top_left.second);
		yc.push_back(rect[i].bottom_right.second);
	}
	sort(nodes, nodes + 2 * n);
	sort(yc.begin(), yc.end());
	trim_unique(yc);

	for (size_t i = 0; i < yc.size(); i++)
		reverse_index[yc[i]] = i;

	fill(overlap, overlap + yc.size(), 0);
	for (int i = 0; i < 2 * n; i++) {
		node_entry &current = nodes[i];
		bool must_update = false;
		if (i < 2 * n - 1) {
			node_entry &next = nodes[i + 1];
			if (current.x < next.x)
				must_update = true;
		}
		size_t inf = reverse_index[current.y.first];
		size_t sup = reverse_index[current.y.second];
		if (current.endpoint) {
			for (size_t j = inf; j < sup; j++)
				overlap[j]--;
		} else {
			for (size_t j = inf; j < sup; j++)
				overlap[j]++;
		}
		if (must_update) {
			node_entry &next = nodes[i + 1];
			for (size_t j = 0; j + 1 < yc.size(); j++)
				if (overlap[j] == k) {
					long long diff = yc[j + 1] - yc[j];
					diff *= (next.x - current.x);
					result += diff;
				}
		}
	}

	return result;
}

int main()
{
	int n, k;
	long long result;

	read_input("dreptunghiuri4.in", n, k);
	result = solve_problem(n, k);
	write_output("dreptunghiuri4.out", result);

	return 0;
}
