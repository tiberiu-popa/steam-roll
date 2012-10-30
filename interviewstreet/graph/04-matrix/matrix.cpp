#include <algorithm>
#include <iostream>
#include <limits>
#include <queue>
#include <set>
#include <stack>
#include <vector>

#define MAX_N 100000

using namespace std;

vector< pair<int, int> > neighbour_lists[MAX_N];
pair<long long, long long> dp[MAX_N];
int parent[MAX_N];

template<class T, class S>
ostream & operator << (ostream &os, pair<T, S> p)
{
	os << '(' << p.first << ',' << p.second << ')';
	return os;
}

template<class T>
void print_range(T start, T end)
{
	for (T it = start; it != end; ++it) {
		if (it != start)
			cout << ' ';
		cout << *it;
	}
	cout << endl;
}

template<class T>
void print_range(const char *title, T start, T end)
{
	cout << title << ": ";
	print_range(start, end);
}

template<class T>
void print_vector(const vector<T> &v)
{
	print_range(v.begin(), v.end());
}

template<class T>
void print_vector(const char *title, const vector<T> &v)
{
	print_range(title, v.begin(), v.end());
}

void bfs(const vector< pair<int, int> > neighbour_lists[], int n, int source, int *parent)
{
	queue<int> node_queue;

	fill(parent, parent + n, -1);
	node_queue.push(source);
	parent[source] = -2;
	while (!node_queue.empty()) {
		int u = node_queue.front();
		node_queue.pop();

		const vector< pair<int, int> > &n_list = neighbour_lists[u];
		for (vector< pair<int, int> >::const_iterator it = n_list.begin();
				it != n_list.end(); ++it) {
			int v = it->first;
			if (parent[v] == -1) {
				parent[v] = u;
				node_queue.push(v);
			}
		}
	}
}

void tree_dfs(const vector< pair<int, int> > neighbour_lists[], int source,
	vector<int> &topological_sort)
{
	stack< pair<int, bool> > node_stack;

	node_stack.push(pair<int, bool>(source, false));
	while (!node_stack.empty()) {
		pair<int, bool> entry = node_stack.top();
		node_stack.pop();
		int u = entry.first;
		if (entry.second) {
			topological_sort.push_back(u);
		} else {
			node_stack.push(pair<int, bool>(u, true));
			const vector< pair<int, int> > &n_list = neighbour_lists[u];
			for (vector< pair<int, int> >::const_reverse_iterator it = n_list.rbegin();
					it != n_list.rend(); ++it) {
				int v = it->first;
				node_stack.push(pair<int, bool>(v, false));
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
	vector<int> topological_sort;
	set<int> machines;
	int source;
	int n, k;

	cin >> n >> k;

	for (int i = 0; i < n; i++)
		neighbour_lists[i].clear();

	for (int i = 1; i < n; i++) {
		int x, y, z;
		cin >> x >> y >> z;
		neighbour_lists[x].push_back(pair<int, int>(y, z));
		neighbour_lists[y].push_back(pair<int, int>(x, z));
	}

	for (int i = 0; i < k; i++) {
		int elem;
		cin >> elem;
		machines.insert(elem);
	}

	source = 0;
	bfs(neighbour_lists, n, source, parent);
	for (int i = 0; i < n; i++) {
		int u = parent[i];
		if (u >= 0) {
			vector< pair<int, int> > &n_list = neighbour_lists[i];
			for (vector< pair<int, int> >::iterator it = n_list.begin(); it != n_list.end(); ++it)
				if (it->first == u) {
					n_list.erase(it);
					break;
				}
		}
	}

	tree_dfs(neighbour_lists, source, topological_sort);
	//print_vector("topological_sort", topological_sort);

	const long long ll_max = numeric_limits<long long>::max();
	fill(dp, dp + n, pair<long long, long long>(ll_max, ll_max));
	for (vector<int>::iterator it = topological_sort.begin(); it != topological_sort.end(); ++it) {
		int u = *it;
		bool is_machine = (machines.find(u) != machines.end());
		long long sum = 0;
		const vector< pair<int, int> > &n_list = neighbour_lists[u];
		if (is_machine) {
			for (vector< pair<int, int> >::const_iterator n_it = n_list.begin();
					n_it != n_list.end(); ++n_it) {
				int v = n_it->first;
				int cost = n_it->second;
				pair<long long, long long> child = dp[v];
				if (child.second < ll_max)
					sum += min(child.first, child.second + cost);
			}
			dp[u].second = sum;
		} else {
			long long min_diff = ll_max;
			int num_relevant = 0;
			for (vector< pair<int, int> >::const_iterator n_it = n_list.begin();
					n_it != n_list.end(); ++n_it) {
				int v = n_it->first;
				int cost = n_it->second;
				pair<long long, long long> child = dp[v];
				if (child.second < ll_max) {
					++num_relevant;
					long long base = min(child.first, child.second + cost);
					sum += base;
					long long diff = child.second - base;
					if (diff < min_diff)
						min_diff = diff;
				}
			}
			if (num_relevant > 0) {
				dp[u].first = sum;
				dp[u].second = sum + min_diff;
			}
		}
	}

	//print_range("dp", dp, dp + n);
	write_output(dp[topological_sort.back()].second);
}

int main()
{
	solve_problem();
	return 0;
}
