#include <algorithm>
#include <limits>
#include <queue>
#include <set>
#include <vector>
#include <cstdio>

#define MAX_N 1000
#define MIN_VALUE 1
#define MAX_VALUE 1000000

using namespace std;

struct edge {

	int endpoint;
	int left, right;

	edge() : endpoint(-1), left(-1), right(-1) { }

	edge(int endpoint, int left, int right)
		: endpoint(endpoint), left(left), right(right) { }

};

vector<edge> neighbours[MAX_N];
set< pair<int, int> > inode[MAX_N];

void dijkstra(int root)
{
	priority_queue< pair< pair<int, int>, pair<int, int> > > pq;
	pq.push(make_pair(make_pair(MAX_VALUE - MIN_VALUE + 1, root), make_pair(MIN_VALUE, MAX_VALUE)));
	inode[root].insert(make_pair(MIN_VALUE, MAX_VALUE));
	while (!pq.empty()) {
		pair< pair<int, int>, pair<int, int> > entry = pq.top();
		pq.pop();
		root = entry.first.second;
		pair<int, int> interval = entry.second;
		vector<edge> &nodes = neighbours[root];
		for (vector<edge>::iterator it = nodes.begin(); it != nodes.end(); ++it) {
			int node = it->endpoint;
			int l = max(interval.first, it->left);
			int r = min(interval.second, it->right);
			if (l <= r) {
				set< pair<int, int> >::iterator l_it = inode[node].lower_bound(make_pair(l + 1, -1));
				bool valid = true;
				if (l_it != inode[node].begin()) {
					--l_it;
					if (l_it->first <= l && r <= l_it->second)
						valid = false;
					else if (l <= l_it->first && l_it->second <= r)
						inode[node].erase(l_it);
				}
				if (valid) {
					pq.push(make_pair(make_pair(r - l + 1, node), make_pair(l, r)));
					pair<set< pair<int, int> >::iterator, bool> itp = inode[node].insert(make_pair(l, r));
					l_it = itp.first;
					++l_it;
					set< pair<int, int> >::iterator u_it = l_it;
					for (; u_it != inode[node].end(); ++u_it)
						if (u_it->second > r)
							break;
					inode[node].erase(l_it, u_it);
				}
			}
		}
	}
}

int solve_problem()
{
	int n, m;

	if (scanf("%d %d", &n, &m) != 2)
		return 1;

	for (int i = 0; i < m; i++) {
		int a, b, l, r;
		if (scanf("%d %d %d %d", &a, &b, &l, &r) != 4)
			return 1;
		--a;
		--b;
		neighbours[a].push_back(edge(b, l, r));
		neighbours[b].push_back(edge(a, l, r));
	}

	dijkstra(0);

	if (inode[n - 1].empty()) {
		printf("Nice work, Dima!\n");
	} else {
		int result = numeric_limits<int>::min();
		for (set< pair<int, int> >::iterator s_it = inode[n - 1].begin(); s_it != inode[n - 1].end(); ++s_it) {
			int cand = s_it->second - s_it->first + 1;
			if (cand > result)
				result = cand;
		}
		printf("%d\n", result);
	}

	return 0;
}

int main()
{
	solve_problem();
	return 0;
}
