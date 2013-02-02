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

vector<unsigned int> small_patterns;
vector<unsigned int> large_patterns;
int invtable[MAX_DISTANCE + 1];

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

int mulmod(int x, int y, int mod)
{
	long long result = x;
	return (result * y) % mod;
}

int invmod(int x, int mod)
{
	int result[2][2] = { { 1, 0}, { 0, 1 } };
	int base[2][2] = { { 0, 1}, { 1, 0 } };
	int tmp[2][2];

	int y = x % mod;
	x = mod;
	while (y > 0) {
		int q = x / y;
		int r = x % y;
		base[1][1] = -q;
		memset(tmp, 0, sizeof(tmp));
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 2; j++) {
				for (int k = 0; k < 2; k++)
					tmp[i][j] += result[i][k] * base[k][j];
			}
		memcpy(result, tmp, sizeof(result));
		x = y;
		y = r;
	}
	int inv = result[1][0];
	if (inv < 0)
		inv += mod;
	return inv;
}

void build_small_patterns(int size)
{
	unsigned int mask;

	small_patterns.clear();
	size -= 3;

	mask = (1 << 0) | (1 << 1) | (1 << 2) | (1 << 8);
	for (int j = 0; j <= size; j++)
		small_patterns.push_back(mask << j);

	mask = (1 << 0) | (1 << 1) | (1 << 2) | (1 << 10);
	for (int j = 0; j <= size; j++)
		small_patterns.push_back(mask << j);

	mask = (1 << 0) | (1 << 8) | (1 << 9) | (1 << 10);
	for (int j = 0; j <= size; j++)
		small_patterns.push_back(mask << j);

	mask = (1 << 2) | (1 << 8) | (1 << 9) | (1 << 10);
	for (int j = 0; j <= size; j++)
		small_patterns.push_back(mask << j);
}

void build_large_patterns(int size)
{
	unsigned int mask;

	large_patterns.clear();
	size -= 2;

	mask = (1 << 0) | (1 << 1) | (1 << 8) | (1 << 16);
	for (int j = 0; j <= size; j++)
		large_patterns.push_back(mask << j);

	mask = (1 << 0) | (1 << 8) | (1 << 16) | (1 << 17);
	for (int j = 0; j <= size; j++)
		large_patterns.push_back(mask << j);

	mask = (1 << 0) | (1 << 1) | (1 << 9) | (1 << 17);
	for (int j = 0; j <= size; j++)
		large_patterns.push_back(mask << j);

	mask = (1 << 1) | (1 << 9) | (1 << 16) | (1 << 17);
	for (int j = 0; j <= size; j++)
		large_patterns.push_back(mask << j);
}

void build_inverse_table()
{
	for (int i = 1; i <= MAX_DISTANCE; i++)
		invtable[i] = invmod(i, MOD_PRIME);
}

void build_precomputed()
{
	build_inverse_table();
}

void bfs(unsigned int b, const vector<unsigned int> &patterns,
	map<unsigned int, int> &visited)
{
	queue< pair<unsigned int, int> > nodes;
	nodes.push(pair<unsigned int, int>(b, 0));
	visited[b] = 1;
	while (!nodes.empty()) {
		const pair<unsigned int, int> &elem = nodes.front();
		unsigned int mask = elem.first;
		int distance = elem.second;
		int num_paths = visited[mask];
		if (distance > 1) {
			num_paths = mulmod(num_paths, invtable[distance], MOD_PRIME);
			visited[mask] = num_paths;
		}
		nodes.pop();
		for (size_t i = 0; i < patterns.size(); i++) {
			unsigned int pat_mask = patterns[i];
			if ((mask & pat_mask) == pat_mask) {
				unsigned int next_mask = mask & ~pat_mask;
				if (visited.find(next_mask) == visited.end())
					nodes.push(pair<unsigned int, int>(next_mask, distance + 1));
				add_to_mod(visited[next_mask], num_paths, MOD_PRIME);
			}
		}
	}
}

void solve_problem()
{
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

	build_small_patterns(m);
	build_large_patterns(m);

	unsigned int b = grid[0] | (grid[1] << 8);
	map<unsigned int, int> reduced_visited, total_visited, single_visited;
	bfs(b, small_patterns, reduced_visited);
	for (int i = 2; i < n; i++) {
		total_visited.clear();
		for (map<unsigned int, int>::iterator it = reduced_visited.begin();
				it != reduced_visited.end(); ++it) {
			single_visited.clear();
			bfs(it->first | (grid[i] << 16), large_patterns, single_visited);
			int num_paths = it->second;
			for (map<unsigned int, int>::iterator s_it = single_visited.begin();
					s_it != single_visited.end(); ++s_it) {
				unsigned int s_mask = s_it->first;
				if ((s_mask & 255) == 0) {
					int all_paths = mulmod(num_paths, s_it->second, MOD_PRIME);
					add_to_mod(total_visited[s_mask >> 8], all_paths, MOD_PRIME);
				}
			}
		}
		reduced_visited.clear();
		for (map<unsigned int, int>::iterator it = total_visited.begin();
				it != total_visited.end(); ++it) {
			single_visited.clear();
			bfs(it->first, small_patterns, single_visited);
			int num_paths = it->second;
			for (map<unsigned int, int>::iterator s_it = single_visited.begin();
					s_it != single_visited.end(); ++s_it) {
				int all_paths = mulmod(num_paths, s_it->second, MOD_PRIME);
				add_to_mod(reduced_visited[s_it->first], all_paths, MOD_PRIME);
			}
		}
	}

	cout << reduced_visited[0] << endl;
}

int main()
{
	int num_tests;

	build_precomputed();

	cin >> num_tests;
	for (int i = 0; i < num_tests; i++)
		solve_problem();

	return 0;
}
