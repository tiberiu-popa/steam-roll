#include <algorithm>
#include <set>
#include <utility>
#include <vector>
#include <cstdio>
#include <cstdlib>

#define MAX_N 1000000

using namespace std;

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

void run_test(int test)
{
	set< pair<int, int> > points;
	vector<node_entry> nodes;
	int w, h;
	int p, q;
	int x, y;
	int n;
	int a, b, c, d;

	if (scanf("%d %d", &w, &h) != 2)
		exit(EXIT_FAILURE);
	if (scanf("%d %d", &p, &q) != 2)
		exit(EXIT_FAILURE);
	if (scanf("%d", &n) != 1)
		exit(EXIT_FAILURE);
	if (scanf("%d %d", &x, &y) != 2)
		exit(EXIT_FAILURE);
	if (scanf("%d %d %d %d", &a, &b, &c, &d) != 4)
		exit(EXIT_FAILURE);

	for (int i = 0; i < n; i++) {
		//printf("x=%d y=%d\n", x, y);
		points.insert(pair<int, int>(x, y));
		int nx = (x * a + y * b + 1) % w;
		int ny = (x * c + y * d + 1) % h;
		x = nx;
		y = ny;
	}

	for (set< pair<int, int> >::iterator it = points.begin(); it != points.end(); ++it) {
		int rx = 1 + min(w - p, it->first);
		int ry = 1 + min(h - q, it->second);
		int lx = max(0, 1 + it->first - p);
		int ly = max(0, 1 + it->second - q);
		//printf("(%d, %d) to (%d, %d)\n", lx, ly, rx, ry);
		nodes.push_back(node_entry(lx, ly, ry, false));
		nodes.push_back(node_entry(rx, ly, ry, true));
	}
	//printf("\n");
	sort(nodes.begin(), nodes.end());
	//printf("num_points=%zu\n", points.size());
#if 0
	for (vector<node_entry>::iterator it = nodes.begin(); it != nodes.end(); ++it)
		printf("%d at (%d, %d)\n", it->x, it->y.first, it->y.second);
	printf("\n");
#endif

	vector<int> overlap(h);
	int result = 0;
	int loops = 0;
	for (size_t i = 0; i < nodes.size(); i++) {
		node_entry &current = nodes[i];
		bool must_update = false;
		if (i < nodes.size() - 1) {
			node_entry &next = nodes[i + 1];
			if (current.x < next.x)
				must_update = true;
		}
		int inf = current.y.first;
		int sup = current.y.second;
		loops += sup - inf;
		if (current.endpoint) {
			for (int j = inf; j < sup; j++)
				overlap[j]--;
		} else {
			for (int j = inf; j < sup; j++)
				overlap[j]++;
		}
		if (must_update) {
			node_entry &next = nodes[i + 1];
			for (size_t j = 0; j < overlap.size(); j++)
				if (overlap[j] > 0) {
					result += next.x - current.x;
				}
		}
	}
	//printf("loops=%d\n", loops);

	printf("Case #%d: %d\n", test, (w - p + 1) * (h - q + 1) - result);
	//printf("\n");
}

int main()
{
	int num_tests;

	if (scanf("%d", &num_tests) != 1)
		exit(EXIT_FAILURE);
	for (int i = 0; i < num_tests; i++)
		run_test(i + 1);

	return 0;
}
