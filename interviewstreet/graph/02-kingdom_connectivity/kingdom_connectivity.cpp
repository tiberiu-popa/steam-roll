#include <algorithm>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <vector>

#define MAX_N 10000
#define MOD_NUMBER 1000000000

using namespace std;

vector<int> fw_neighbour_lists[MAX_N];
vector<int> bw_neighbour_lists[MAX_N];
map<int, int> bw_neighbour_counts[MAX_N];
int dp[MAX_N];

template<class T>
void print_vector(const vector<T> &v)
{
        for (typename vector<T>::const_iterator it = v.begin(); it != v.end(); ++it) {
                if (it != v.begin())
                        cout << ' ';
                cout << *it;
        }
        cout << endl;
}

template<class T>
void print_vector(const char *title, const vector<T> &v)
{
        cout << title << ": ";
        print_vector(v);
}

template<class T>
void print_set(const set<T> &v)
{
        for (typename set<T>::const_iterator it = v.begin(); it != v.end(); ++it) {
                if (it != v.begin())
                        cout << ' ';
                cout << *it;
        }
        cout << endl;
}

template<class T>
void print_set(const char *title, const set<T> &v)
{
        cout << title << ": ";
        print_set(v);
}

int add_modulo(int x, int y)
{
	int sum;

	sum = x + y;
	if (sum >= MOD_NUMBER)
		sum -= MOD_NUMBER;

	return sum;
}

int mul_modulo(int x, int y)
{
	long long product;

	product = x;
	product *= y;

	return product % MOD_NUMBER;
}

void bfs(const vector<int> neighbour_lists[], int source, set<int> &visited)
{
	queue<int> node_queue;

	node_queue.push(source);
	visited.insert(source);
	while (!node_queue.empty()) {
		int u = node_queue.front();
		node_queue.pop();

		const vector<int> &n_list = neighbour_lists[u];
		for (vector<int>::const_iterator it = n_list.begin(); it != n_list.end(); ++it) {
			int v = *it;
			if (visited.find(v) == visited.end()) {
				visited.insert(v);
				node_queue.push(v);
			}
		}
	}
}

void dfs(const vector<int> neighbour_lists[], int source, map<int, bool> &color_visited,
	set<int> &cycle_nodes, vector<int> &topological_sort)
{
	stack< pair<int, bool> > node_stack;

	node_stack.push(pair<int, bool>(source, false));
	while (!node_stack.empty()) {
		pair<int, bool> entry = node_stack.top();
		node_stack.pop();
		int u = entry.first;
		if (entry.second) {
			color_visited[u] = true;
			topological_sort.push_back(u);
		} else if (color_visited.find(u) == color_visited.end()) {
			color_visited[u] = false;
			node_stack.push(pair<int, bool>(u, true));
			const vector<int> &n_list = neighbour_lists[u];
			for (vector<int>::const_reverse_iterator it = n_list.rbegin();
					it != n_list.rend(); ++it) {
				int v = *it;
				map<int, bool>::iterator c_it = color_visited.find(v);
				if (c_it == color_visited.end())
					node_stack.push(pair<int, bool>(v, false));
				else if (!c_it->second)
					cycle_nodes.insert(v);
			}
		}
	}
}

void write_output(int result)
{
	cout << result << endl;
}

void solve_problem()
{
	map<int, size_t> inverse_topsort;
	map<int, bool> color_visited;
	set<int> visited;
	set<int> cycle_nodes;
	int source, sink;
	int n, m;

	cin >> n >> m;

	for (int i = 0; i < n; i++) {
		fw_neighbour_lists[i].clear();
		bw_neighbour_lists[i].clear();
		bw_neighbour_counts[i].clear();
	}

	for (int i = 0; i < m; i++) {
		int x, y;
		cin >> x >> y;
		--x;
		--y;
		fw_neighbour_lists[x].push_back(y);
		bw_neighbour_lists[y].push_back(x);
		bw_neighbour_counts[y][x]++;
	}

	source = 0;
	sink = n - 1;

	vector<int> topological_sort;
	dfs(bw_neighbour_lists, sink, color_visited, cycle_nodes, topological_sort);
	//print_vector("topological_sort", topological_sort);
	//print_set("cycle_nodes", cycle_nodes);

	bfs(fw_neighbour_lists, source, visited);
	//print_set("bfs_visited", visited);

	size_t num_elems = 0;
	for (vector<int>::const_iterator it = topological_sort.begin();
			it != topological_sort.end(); ++it) {
		int elem = *it;
		if (visited.find(elem) != visited.end()) {
			inverse_topsort.insert(pair<int, size_t>(elem, num_elems));
			topological_sort[num_elems++] = elem;
		}
	}
	topological_sort.resize(num_elems);
	//print_vector("topological_sort", topological_sort);

	bool is_infinite = false;
	for (vector<int>::const_iterator it = topological_sort.begin();
			it != topological_sort.end(); ++it) {
		int elem = *it;
		if (cycle_nodes.find(elem) != cycle_nodes.end()) {
			is_infinite = true;
			break;
		}
	}

	if (is_infinite) {
		cout << "INFINITE PATHS" << endl;
		return;
	}

	for (size_t i = 0; i < num_elems; i++) {
		int u = topological_sort[i];
		int value = 0;
		if (u == source) {
			++value;
		} else {
			const map<int, int> &n_count = bw_neighbour_counts[u];
			for (map<int, int>::const_iterator it = n_count.begin();
					it != n_count.end(); ++it) {
				int elem = it->first;
				map<int, size_t>::iterator inv_it = inverse_topsort.find(elem);
				if (inv_it != inverse_topsort.end()) {
					int count = it->second;
					int term = mul_modulo(count, dp[inv_it->second]);
					value = add_modulo(value, term);
				}
			}
		}
		dp[i] = value;
	}
	write_output(dp[inverse_topsort[sink]]);
}

int main()
{
	solve_problem();
	return 0;
}
