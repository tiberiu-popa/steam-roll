#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <queue>
#include <stack>
#include <vector>
#include <cstring>

using namespace std;

template<class T, class S>
ostream & operator<< (ostream &os, const pair<T, S> &elem)
{
	os << '(' << elem.first << ',' << elem.second << ')';
	return os;
}

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

void bfs_to_tree(vector< vector<int> > &children)
{
	vector<int> parent(children.size(), -1);
	queue<int> nodes;
	nodes.push(0);
	while (!nodes.empty()) {
		int node = nodes.front();
		nodes.pop();
		vector<int> &node_children = children[node];
		int parent_node = parent[node];
		for (vector<int>::iterator it = node_children.begin();
				it != node_children.end();) {
			int child = *it;
			if (child == parent_node) {
				it = node_children.erase(it);
			} else {
				nodes.push(child);
				parent[child] = node;
				++it;
			}
		}
	}
}

void tree_dfs_annotate(const vector< vector<int> > &children,
	const vector<int> &population, vector<int> &sum,
	vector< pair<int, int> > &times)
{
	stack< pair<int, bool> > nodes;
	int time = 0;

	nodes.push(pair<int, bool>(0, true));
	while (!nodes.empty()) {
		pair<int, bool> entry = nodes.top();
		int node = entry.first;
		nodes.pop();
		const vector<int> &node_children = children[node];
		if (entry.second) {
			times[node].first = time;
			nodes.push(pair<int, bool>(node, false));
			for (vector<int>::const_iterator it = node_children.begin();
					it != node_children.end(); ++it) {
				int child = *it;
				nodes.push(pair<int, bool>(child, true));
			}
		} else {
			times[node].second = time;
			int node_sum = population[node];
			for (vector<int>::const_iterator it = node_children.begin();
					it != node_children.end(); ++it) {
				int child = *it;
				node_sum += sum[child];
			}
			sum[node] = node_sum;
		}
		++time;
	}
}

void read_input(const char *filename, vector<int> &population, vector< vector<int> > &children)
{
	ifstream f(filename);
	int n;

	f >> n;
	for (int i = 0; i < n; i++) {
		int elem;
		f >> elem;
		population.push_back(elem);
		children.push_back(vector<int>());
	}
	for (int i = 0; i < n - 1; i++) {
		int x, y;
		f >> x >> y;
		--x;
		--y;
		children[x].push_back(y);
		children[y].push_back(x);
	}

	bfs_to_tree(children);

	f.close();
}

void write_output(const char *filename, int answer)
{
	ofstream f(filename);

	f << answer << endl;

	f.close();
}

int solve_problem(const vector<int> &population, const vector< vector<int> > &children)
{
	pair<int, size_t> nodes[4];
	int parent[4];
	int scores[4];
	vector<int> sum(children.size());
	vector< pair<int, int> > times(children.size());
	int min_diff = numeric_limits<int>::max();

	tree_dfs_annotate(children, population, sum, times);
	//print_vector("sum", sum);
	//print_vector("times", times);

	for (size_t i = 1; i < children.size(); i++)
		for (size_t j = i + 1; j < children.size(); j++)
			for (size_t k = j + 1; k < children.size(); k++) {
				nodes[0] = pair<int, size_t>(times[0].first, 0);
				nodes[1] = pair<int, size_t>(times[i].first, i);
				nodes[2] = pair<int, size_t>(times[j].first, j);
				nodes[3] = pair<int, size_t>(times[k].first, k);
				sort(nodes, nodes + 4);
				for (int u = 0; u < 4; u++) {
					int node_parent = -1;
					int u_idx = nodes[u].second;
					for (int v = u - 1; v >= 0; v--) {
						int v_idx = nodes[v].second;
						if (times[u_idx].second < times[v_idx].second) {
							node_parent = v;
							break;
						}
					}
					parent[u] = node_parent;
				}
				for (int u = 0; u < 4; u++) {
					int u_idx = nodes[u].second;
					int score = sum[u_idx];
					for (int v = u + 1; v < 4; v++) {
						int v_idx = nodes[v].second;
						if (parent[v] == u)
							score -= sum[v_idx];
					}
					scores[u] = score;
				}
				int max_score = *max_element(scores, scores + 4);
				int min_score = *min_element(scores, scores + 4);
				min_diff = min(min_diff, max_score - min_score);
			}

	return min_diff;
}

int main()
{
	vector< vector<int> > children;
	vector<int> population;
	int answer;

	read_input("petrica.in", population, children);
	answer = solve_problem(population, children);
	write_output("petrica.out", answer);

	return 0;
}
