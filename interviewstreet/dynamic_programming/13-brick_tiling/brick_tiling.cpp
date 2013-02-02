#include <iostream>
#include <map>
#include <queue>
#include <vector>
#include <cstring>

using namespace std;

#define NUM_ROWS 20
#define NUM_COLUMNS 8
#define GRID_SIZE (NUM_ROWS * NUM_COLUMNS)
#define MAX_DISTANCE ((GRID_SIZE + 3) / 4)
#define MOD_PRIME 1000000007

int addmod(int x, int y, int mod)
{
	int sum = x + y;
	if (sum >= mod)
		sum -= mod;
	return sum;
}

void add_to_mod(int &x, int y, int mod)
{
	x = addmod(x, y, mod);
}

void add_small_patterns(vector<unsigned int> &v, int offset, int size)
{
	unsigned int mask;
	int limit;

	limit = offset + size;

	mask = (1 << 0) | (1 << 1) | (1 << 2) | (1 << 8);
	for (int j = offset; j <= limit; j++)
		v.push_back(mask << j);

	mask = (1 << 0) | (1 << 1) | (1 << 2) | (1 << 10);
	for (int j = offset; j <= limit; j++)
		v.push_back(mask << j);

	mask = (1 << 0) | (1 << 8) | (1 << 9) | (1 << 10);
	for (int j = offset; j <= limit; j++)
		v.push_back(mask << j);

	mask = (1 << 2) | (1 << 8) | (1 << 9) | (1 << 10);
	for (int j = offset; j <= limit; j++)
		v.push_back(mask << j);
}

void add_large_patterns(vector<unsigned int> &v, int offset, int size)
{
	unsigned int mask;
	int limit;

	limit = offset + size;

	mask = (1 << 0) | (1 << 1) | (1 << 8) | (1 << 16);
	for (int j = offset; j <= limit; j++)
		v.push_back(mask << j);

	mask = (1 << 0) | (1 << 8) | (1 << 16) | (1 << 17);
	for (int j = offset; j <= limit; j++)
		v.push_back(mask << j);

	mask = (1 << 0) | (1 << 1) | (1 << 9) | (1 << 17);
	for (int j = offset; j <= limit; j++)
		v.push_back(mask << j);

	mask = (1 << 1) | (1 << 9) | (1 << 16) | (1 << 17);
	for (int j = offset; j <= limit; j++)
		v.push_back(mask << j);
}

void build_small_patterns(vector<unsigned int> &patterns, int size)
{
	add_small_patterns(patterns, 0, size - 3);
}

void build_large_patterns(vector<unsigned int> &patterns, int size)
{
	add_large_patterns(patterns, 0, size - 2);
}

void run_dp(const vector<unsigned int> &patterns,
	map<unsigned int, int> &visited)
{
	for (vector<unsigned int>::const_iterator pat_it = patterns.begin();
			pat_it != patterns.end(); ++pat_it) {
		unsigned int pattern = *pat_it;
		for (map<unsigned int, int>::iterator it = visited.begin();
				it != visited.end(); ++it) {
			unsigned int mask = it->first;
			if ((mask & pattern) == pattern) {
				unsigned int r_mask = mask & ~pattern;
				add_to_mod(visited[r_mask], it->second, MOD_PRIME);
			}
		}
	}
}

void solve_problem()
{
	vector<unsigned int> small_patterns, large_patterns;
	vector<unsigned int> grid;
	int n, m;

	cin >> n >> m;
	for (int i = 0; i < n; i++) {
		string line;
		unsigned int mask = 0;
		cin >> line;
		for (size_t j = 0; j < line.length(); j++)
			if (line[j] == '.')
				mask |= 1 << j;
		grid.push_back(mask);
	}

	build_small_patterns(small_patterns, m);
	build_large_patterns(large_patterns, m);

	unsigned int b = grid[0] | (grid[1] << 8);
	map<unsigned int, int> reduced_visited, total_visited;
	reduced_visited[b] = 1;
	run_dp(small_patterns, reduced_visited);
	for (int i = 2; i < n; i++) {
		total_visited.clear();
		unsigned int row_mask = grid[i] << 16;
		for (map<unsigned int, int>::iterator it = reduced_visited.begin();
				it != reduced_visited.end(); ++it) {
			unsigned int mask = it->first;
			total_visited[mask | row_mask] = it->second;
		}
		run_dp(large_patterns, total_visited);
		reduced_visited.clear();
		for (map<unsigned int, int>::iterator it = total_visited.begin();
				it != total_visited.end(); ++it) {
			unsigned int mask = it->first;
			if ((mask & 255) == 0)
				reduced_visited[mask >> 8] = it->second;
		}
		run_dp(small_patterns, reduced_visited);
	}

	cout << reduced_visited[0] << endl;
}

int main()
{
	int num_tests;

	cin >> num_tests;
	for (int i = 0; i < num_tests; i++)
		solve_problem();

	return 0;
}
