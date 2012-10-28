#include <algorithm>
#include <iostream>
#include <queue>
#include <set>
#include <vector>
#include <cstdio>
#include <cstdlib>

#define MAX_K 101

using namespace std;

set<int> neighbours[MAX_K];

void bfs(int source, set<int> &visited, vector<int> &new_nodes)
{
	queue<int> nodes;

	nodes.push(source);
	visited.insert(source);
	new_nodes.push_back(source);

	while (!nodes.empty()) {
		int node = nodes.front();
		nodes.pop();
		const set<int> &n_set = neighbours[node];
		for (set<int>::const_iterator it = n_set.begin(); it != n_set.end(); ++it) {
			int next = *it;
			if (visited.find(next) == visited.end()) {
				nodes.push(next);
				visited.insert(next);
				new_nodes.push_back(next);
			}
		}
	}
}

void solve_problem()
{
	vector<int> v;
	int k;
	char str[MAX_K + 1];

	if (scanf("%d", &k) != 1)
		exit(EXIT_FAILURE);

	for (int i = 0; i < k; i++) {
		int elem;
		if (scanf("%d", &elem) != 1)
			exit(EXIT_FAILURE);
		v.push_back(elem);
	}

	for (int i = 0; i < k; i++) {
		if (scanf("%s", str) != 1)
			exit(EXIT_FAILURE);
		for (int j = 0; j < k; j++)
			if (j != i && str[j] == 'Y') {
				neighbours[i].insert(j);
				neighbours[j].insert(i);
			}
	}

	set<int> visited;
	for (int i = 0; i < k; i++)
		if (visited.find(i) == visited.end()) {
			vector<int> new_nodes;
			vector<int> subseq;
			bfs(i, visited, new_nodes);
			for (vector<int>::iterator it = new_nodes.begin(); it != new_nodes.end(); ++it)
				subseq.push_back(v[*it]);
			sort(new_nodes.begin(), new_nodes.end());
			sort(subseq.begin(), subseq.end());
			for (size_t i = 0; i < new_nodes.size(); i++)
				v[new_nodes[i]] = subseq[i];
		}

	for (vector<int>::iterator it = v.begin(); it != v.end(); ++it) {
		if (it != v.begin())
			cout << ' ';
		cout << *it;
	}
	cout << endl;
}

int main()
{
	solve_problem();
	return 0;
}
