#include <algorithm>
#include <limits>
#include <map>
#include <set>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

#define MAX_N 100000
#define MAX_EDGES 300000

using namespace std;

vector< pair<int, int> > polygons[MAX_N];
vector< pair< pair<int, int>, pair<int, int> > > edges;

int skip_whitespace()
{
	int ch;
	while (true) {
		ch = getchar_unlocked();
		if (ch != ' ' && ch != '\n')
			break;
	}
	return ch;
}

template<typename T>
T read_unsigned_integer()
{
	T result = (T) 0;
	int ch = skip_whitespace();
	while (ch >= '0' && ch <= '9') {
		result = 10 * result + (ch - '0');
		ch = getchar_unlocked();
	}
	return result;
}

template<typename T>
T read_signed_integer()
{
	T result = (T) 0;
	bool flip = false;
	int ch = skip_whitespace();
	if (ch == '-') {
		flip = true;
		ch = skip_whitespace();
	}
	while (ch >= '0' && ch <= '9') {
		result = 10 * result + (ch - '0');
		ch = getchar_unlocked();
	}
	return flip ? -result : result;
}

char *trim_line(char *str)
{
	return strtok(str, "\r\n");
}

template<class T>
void trim_unique(vector<T> &v)
{
	typename vector<T>::iterator it = unique(v.begin(), v.end());
	typename vector<T>::difference_type diff = it - v.begin();
	v.resize((typename vector<T>::size_type) diff);
}

void tree_bfs(const vector<int> *neighbours, int root, vector<int> &nodes)
{
	nodes.push_back(root);
	for (size_t k = 0; k < nodes.size(); k++) {
		int root = nodes[k];
		for (size_t i = 0; i < neighbours[root].size(); i++) {
			int node = neighbours[root][i];
			nodes.push_back(node);
		}
	}
}

template<typename T>
T pow2(int k)
{
	T result = 1;
	return result << k;
}

template<typename T>
int get_msb(T n)
{
	int k = 0;
	for (; n > 0; n >>= 1)
		k++;
	return k;
}

template<typename T>
int get_upper_exp(T n)
{
	int k = get_msb(n);
	T result = pow2<T>(k - 1);
	if (result == n)
		return k - 1;
	else
		return k;
}

long long ll_mul(long long x, long long y)
{
	return x * y;
}

int better_rand(int n, int shift)
{
	int r;
	do {
		r = rand() >> shift;
	} while (r >= n);
	return r;
}

template<typename T>
void fisher_yates(vector<T> &v)
{
	int n = (int) v.size();
	int m = get_upper_exp(n);
	int max_lg = get_upper_exp<int>(RAND_MAX);

	for (int i = n - 1; i >= 1; i--) {
		size_t j = (size_t) better_rand(i + 1, max_lg - m);
		if (i != j)
			swap(v[(size_t) i], v[j]);
		if ((i & (i - 1)) == 0)
			--m;
	}
}

struct search_node;

struct trapezoid {

	pair< pair<int, int>, pair<int, int> > top, bottom;
	pair<int, int> leftp, rightp;

	trapezoid *upper_left;
	trapezoid *lower_left;
	trapezoid *upper_right;
	trapezoid *lower_right;

	search_node *s;

	trapezoid() : upper_left(NULL), lower_left(NULL), upper_right(NULL), lower_right(NULL), s(NULL) { }

};

struct search_node {

	search_node *left, *right;
	vector< pair<search_node *, int> > parents;

	search_node() : left(NULL), right(NULL) { }

	search_node(const vector< pair<search_node *, int> > &parents) : left(NULL), right(NULL), parents(parents)
	{
		for (size_t i = 0; i < parents.size(); i++) {
			pair<search_node *, int> entry = parents[i];
			if (entry.second)
				entry.first->right = this;
			else
				entry.first->left = this;
		}
	}

	search_node(search_node *parent, int dir) : left(NULL), right(NULL)
	{
		if (dir)
			parent->right = this;
		else
			parent->left = this;
		parents.push_back(make_pair(parent, dir));
	}

	virtual ~search_node() { }

	void add_parent(search_node *parent, int dir)
	{
		parents.push_back(make_pair(parent, dir));
		if (dir)
			parent->right = this;
		else
			parent->left = this;
	}

	virtual pair<trapezoid *, bool> search(pair<int, int> p, pair<int, int> q) = 0;

};

void delete_search_node(search_node *s)
{
	vector<search_node *> nodes;
	set<search_node *> visited;
	map<search_node *, int> indeg;

	nodes.push_back(s);
	visited.insert(s);
	for (size_t k = 0; k < nodes.size(); k++) {
		search_node *root = nodes[k];
		indeg[root];
		if (root->left) {
			indeg[root->left]++;
			if (visited.find(root->left) == visited.end()) {
				nodes.push_back(root->left);
				visited.insert(root->left);
			}
		}
		if (root->right) {
			indeg[root->right]++;
			if (visited.find(root->right) == visited.end()) {
				nodes.push_back(root->right);
				visited.insert(root->right);
			}
		}
	}

	vector<search_node *> zeros;
	for (map<search_node *, int>::iterator it = indeg.begin(); it != indeg.end(); ++it)
		if (it->second == 0)
			zeros.push_back(it->first);

	for (size_t k = 0; k < zeros.size(); k++) {
		search_node *root = zeros[k];
		if (root->left) {
			if (--indeg[root->left] == 0)
				zeros.push_back(root->left);
		}
		if (root->right) {
			if (--indeg[root->right] == 0)
				zeros.push_back(root->right);
		}
	}

	for (size_t i = zeros.size(); i > 0;) {
		--i;
		delete zeros[i];
	}
}

struct x_node : public search_node {

	pair<int, int> endpoint;

	x_node(pair<int, int> endpoint) : endpoint(endpoint) { }

	x_node(const vector< pair<search_node *, int> > &parents, pair<int, int> endpoint) :
		search_node(parents), endpoint(endpoint) { }

	x_node(search_node *parent, int dir, pair<int, int> endpoint) :
		search_node(parent, dir), endpoint(endpoint) { }

	virtual pair<trapezoid *, bool> search(pair<int, int> p, pair<int, int> q)
	{
		if (p < endpoint)
			return left->search(p, q);
		else
			return right->search(p, q);
	}

};

bool is_below(pair<int, int> p, pair< pair<int, int>, pair<int, int> > segm)
{
	long long x = ll_mul(p.second - segm.first.second, segm.second.first - segm.first.first);
	long long y = ll_mul(p.first - segm.first.first, segm.second.second - segm.first.second);
	return x < y;
}

struct y_node : public search_node {

	pair< pair<int, int>, pair<int, int> > segm;

	y_node(pair< pair<int, int>, pair<int, int> > segm) : segm(segm) { }

	y_node(const vector< pair<search_node *, int> > &parents, pair< pair<int, int>, pair<int, int> > segm) :
		search_node(parents), segm(segm) { }

	y_node(search_node *parent, int dir, pair< pair<int, int>, pair<int, int> > segm) :
		search_node(parent, dir), segm(segm) { }

	virtual pair<trapezoid *, bool> search(pair<int, int> p, pair<int, int> q)
	{
		long long x = ll_mul(p.second - segm.first.second, segm.second.first - segm.first.first);
		long long y = ll_mul(p.first - segm.first.first, segm.second.second - segm.first.second);
		if (x == y) {
			x = ll_mul(q.second - p.second, segm.second.first - segm.first.first);
			y = ll_mul(q.first - p.first, segm.second.second - segm.first.second);
		}
		if (x < y) {
			return left->search(p, q);
		} else {
			pair<trapezoid *, bool> result = right->search(p, q);
			if (x == y)
				result.second = true;
			return result;
		}
	}

};

struct leaf_node : public search_node {

	trapezoid *t;

	leaf_node(trapezoid *t) : t(t)
	{
		t->s = this;
	}

	leaf_node(const vector< pair<search_node *, int> > &parents, trapezoid *t) : search_node(parents), t(t)
	{
		t->s = this;
	}

	leaf_node(search_node *parent, int dir, trapezoid *t) : search_node(parent, dir), t(t)
	{
		t->s = this;
	}

	~leaf_node()
	{
		delete t;
	}

	virtual pair<trapezoid *, bool> search(pair<int, int> p, pair<int, int> q)
	{
		(void) p;
		(void) q;
		return make_pair(t, false);
	}

};

search_node *s = NULL;
trapezoid *t = NULL;
int x_min = numeric_limits<int>::max();
int x_max = numeric_limits<int>::min();
int y_min = numeric_limits<int>::max();
int y_max = numeric_limits<int>::min();

void edge_no_inside(trapezoid *node, pair<int, int> p, pair<int, int> q)
{
	pair< pair<int, int>, pair<int, int> > edge = make_pair(p, q);

	trapezoid *c = new trapezoid;
	c->leftp = p;
	c->rightp = q;
	c->top = node->top;
	c->bottom = edge;

	trapezoid *d = new trapezoid;
	d->leftp = p;
	d->rightp = q;
	d->top = edge;
	d->bottom = node->bottom;

	c->upper_left = node->upper_left;
	c->upper_right = node->upper_right;

	d->lower_left = node->lower_left;
	d->lower_right = node->lower_right;

	if (node->upper_left != NULL)
		node->upper_left->upper_right = c;
	if (node->lower_left != NULL)
		node->lower_left->lower_right = d;
	if (node->upper_right != NULL)
		node->upper_right->upper_left = c;
	if (node->lower_right != NULL)
		node->lower_right->lower_left = d;

	search_node *s0 = node->s;
	search_node *t0 = new y_node(s0->parents, edge);

	new leaf_node(t0, 0, d);
	new leaf_node(t0, 1, c);

	if (s0->parents.empty())
		s = t0;

	delete s0;
}

void edge_right_inside(trapezoid *node, pair<int, int> p, pair<int, int> q)
{
	pair< pair<int, int>, pair<int, int> > edge = make_pair(p, q);

	trapezoid *b = new trapezoid;
	b->leftp = q;
	b->rightp = node->rightp;
	b->top = node->top;
	b->bottom = node->bottom;

	trapezoid *c = new trapezoid;
	c->leftp = p;
	c->rightp = q;
	c->top = node->top;
	c->bottom = edge;

	trapezoid *d = new trapezoid;
	d->leftp = p;
	d->rightp = q;
	d->top = edge;
	d->bottom = node->bottom;

	b->upper_left = c;
	b->lower_left = d;
	b->upper_right = node->upper_right;
	b->lower_right = node->lower_right;

	c->upper_left = node->upper_left;
	c->upper_right = b;

	d->lower_left = node->lower_left;
	d->lower_right = b;

	if (node->upper_left != NULL)
		node->upper_left->upper_right = c;
	if (node->lower_left != NULL)
		node->lower_left->lower_right = d;
	if (node->upper_right != NULL)
		node->upper_right->upper_left = b;
	if (node->lower_right != NULL)
		node->lower_right->lower_left = b;

	search_node *s0 = node->s;
	search_node *t0 = new x_node(s0->parents, q);

	search_node *tl = new y_node(t0, 0, edge);

	new leaf_node(t0, 1, b);
	new leaf_node(tl, 0, d);
	new leaf_node(tl, 1, c);

	if (s0->parents.empty())
		s = t0;

	delete s0;
}

void edge_left_inside(trapezoid *node, pair<int, int> p, pair<int, int> q)
{
	pair< pair<int, int>, pair<int, int> > edge = make_pair(p, q);

	trapezoid *a = new trapezoid;
	a->leftp = node->leftp;
	a->rightp = p;
	a->top = node->top;
	a->bottom = node->bottom;

	trapezoid *c = new trapezoid;
	c->leftp = p;
	c->rightp = q;
	c->top = node->top;
	c->bottom = edge;

	trapezoid *d = new trapezoid;
	d->leftp = p;
	d->rightp = q;
	d->top = edge;
	d->bottom = node->bottom;

	a->upper_left = node->upper_left;
	a->lower_left = node->lower_left;
	a->upper_right = c;
	a->lower_right = d;

	c->upper_left = a;
	c->upper_right = node->upper_right;

	d->lower_left = a;
	d->lower_right = node->lower_right;

	if (node->upper_left != NULL)
		node->upper_left->upper_right = a;
	if (node->lower_left != NULL)
		node->lower_left->lower_right = a;
	if (node->upper_right != NULL)
		node->upper_right->upper_left = c;
	if (node->lower_right != NULL)
		node->lower_right->lower_left = d;

	search_node *s0 = node->s;
	search_node *t0 = new x_node(s0->parents, p);

	new leaf_node(t0, 0, a);

	search_node *tr = new y_node(t0, 1, edge);

	new leaf_node(tr, 0, d);
	new leaf_node(tr, 1, c);

	if (s0->parents.empty())
		s = t0;

	if (node->leftp == make_pair(x_min, y_min))
		t = a;

	delete s0;
}

void edge_full_inside(trapezoid *node, pair<int, int> p, pair<int, int> q)
{
	pair< pair<int, int>, pair<int, int> > edge = make_pair(p, q);

	trapezoid *a = new trapezoid;
	a->leftp = node->leftp;
	a->rightp = p;
	a->top = node->top;
	a->bottom = node->bottom;

	trapezoid *b = new trapezoid;
	b->leftp = q;
	b->rightp = node->rightp;
	b->top = node->top;
	b->bottom = node->bottom;

	trapezoid *c = new trapezoid;
	c->leftp = p;
	c->rightp = q;
	c->top = node->top;
	c->bottom = edge;

	trapezoid *d = new trapezoid;
	d->leftp = p;
	d->rightp = q;
	d->top = edge;
	d->bottom = node->bottom;

	a->upper_left = node->upper_left;
	a->lower_left = node->lower_left;
	a->upper_right = c;
	a->lower_right = d;

	c->upper_left = a;
	c->upper_right = b;

	d->lower_left = a;
	d->lower_right = b;

	b->upper_left = c;
	b->lower_left = d;
	b->upper_right = node->upper_right;
	b->lower_right = node->lower_right;

	if (node->upper_left != NULL)
		node->upper_left->upper_right = a;
	if (node->lower_left != NULL)
		node->lower_left->lower_right = a;
	if (node->upper_right != NULL)
		node->upper_right->upper_left = b;
	if (node->lower_right != NULL)
		node->lower_right->lower_left = b;

	search_node *s0 = node->s;
	search_node *t0 = new x_node(s0->parents, p);

	new leaf_node(t0, 0, a);

	search_node *tr = new x_node(t0, 1, q);

	search_node *t1 = new y_node(tr, 0, edge);

	new leaf_node(tr, 1, b);
	new leaf_node(t1, 0, d);
	new leaf_node(t1, 1, c);

	if (s0->parents.empty())
		s = t0;

	if (node->leftp == make_pair(x_min, y_min))
		t = a;

	delete s0;
}

int solve_problem()
{
	map< pair< pair<int, int>, pair<int, int> >, int> direction;
	map< pair<int, int>, int> poly_vertex;
	int n, q;

	n = read_unsigned_integer<int>();

	for (int i = 0; i < n; i++) {
		int k = read_unsigned_integer<int>();
		polygons[i].clear();
		for (int j = 0; j < k; j++) {
			int x = read_unsigned_integer<int>();
			int y = read_unsigned_integer<int>();
			if (x < x_min)
				x_min = x;
			if (x > x_max)
				x_max = x;
			if (y < y_min)
				y_min = y;
			if (y > y_max)
				y_max = y;
			pair<int, int> edge(x, y);
			polygons[i].push_back(edge);
			poly_vertex[edge] = i;
		}
		long long area = 0;
		for (size_t j = 1; j < polygons[i].size(); j++) {
			area += ll_mul(polygons[i][j - 1].first, polygons[i][j].second)
				- ll_mul(polygons[i][j].first, polygons[i][j - 1].second);
		}
		area += ll_mul(polygons[i].back().first, polygons[i].front().second)
			- ll_mul(polygons[i].front().first, polygons[i].back().second);
		if (area < 0)
			reverse(polygons[i].begin(), polygons[i].end());
		for (size_t j = 1; j < polygons[i].size(); j++)
			edges.push_back(make_pair(polygons[i][j - 1], polygons[i][j]));
		edges.push_back(make_pair(polygons[i].back(), polygons[i].front()));
	}

	--x_min;
	++x_max;
	--y_min;
	++y_max;

	int poly_index = 0;
	size_t counter = polygons[poly_index].size();
	for (size_t i = 0; i < edges.size(); i++) {
		if (counter == 0) {
			++poly_index;
			counter = polygons[poly_index].size();
		}
		if (edges[i].second < edges[i].first) {
			swap(edges[i].first, edges[i].second);
			direction[edges[i]] = n + poly_index;
		} else {
			direction[edges[i]] = poly_index;
		}
		--counter;
	}

	poly_vertex[make_pair(x_min, y_min)] = -1;
	direction[make_pair(make_pair(x_min, y_min), make_pair(x_max, y_min))] = -1;

	fisher_yates(edges);

	t = new trapezoid;
	t->top = make_pair(make_pair(x_min, y_max), make_pair(x_max, y_max));
	t->bottom = make_pair(make_pair(x_min, y_min), make_pair(x_max, y_min));
	t->leftp = make_pair(x_min, y_min);
	t->rightp = make_pair(x_max, y_min);

	s = new leaf_node(t);

	vector< pair<search_node *, int> > parents;

	vector<trapezoid *> nodes;
	for (size_t i = 0; i < edges.size(); i++) {
		pair<int, int> p = edges[i].first;
		pair<int, int> q = edges[i].second;
		trapezoid *node = s->search(p, q).first;

		nodes.clear();
		nodes.push_back(node);
		while (q > node->rightp) {
			if (is_below(node->rightp, edges[i]))
				node = node->upper_right;
			else
				node = node->lower_right;
			if (node == NULL)
				break;
			nodes.push_back(node);
		}

		if (nodes.size() == 1) {
			node = nodes.front();
			if (p == node->leftp) {
				if (q == node->rightp) {
					edge_no_inside(node, p, q);
				} else {
					edge_right_inside(node, p, q);
				}
			} else {
				if (q == node->rightp) {
					edge_left_inside(node, p, q);
				} else {
					edge_full_inside(node, p, q);
				}
			}
		} else {
			pair< pair<int, int>, pair<int, int> > edge(p, q);
			trapezoid *leftmost = NULL, *rightmost = NULL;

			node = nodes.front();
			search_node *sl0 = node->s;
			search_node *tl0;
			if (p != node->leftp) {
				leftmost = new trapezoid;
				leftmost->leftp = node->leftp;
				leftmost->rightp = p;
				leftmost->top = node->top;
				leftmost->bottom = node->bottom;

				leftmost->upper_left = node->upper_left;
				leftmost->lower_left = node->lower_left;

				if (node->upper_left != NULL)
					node->upper_left->upper_right = leftmost;
				if (node->lower_left != NULL)
					node->lower_left->lower_right = leftmost;

				tl0 = new x_node(sl0->parents, p);

				new leaf_node(tl0, 0, leftmost);
				search_node *ur = new y_node(tl0, 1, edge);

				tl0 = ur;
			} else {
				tl0 = new y_node(sl0->parents, edge);
			}
			node = nodes.back();
			search_node *sr0 = node->s;
			search_node *tr0;
			if (q != node->rightp) {
				rightmost = new trapezoid;
				rightmost->leftp = q;
				rightmost->rightp = node->rightp;
				rightmost->top = node->top;
				rightmost->bottom = node->bottom;

				rightmost->upper_right = node->upper_right;
				rightmost->lower_right = node->lower_right;

				if (node->upper_right != NULL)
					node->upper_right->upper_left = rightmost;
				if (node->lower_right != NULL)
					node->lower_right->lower_left = rightmost;

				tr0 = new x_node(sr0->parents, q);

				search_node *ul = new y_node(tr0, 0, edge);
				new leaf_node(tr0, 1, rightmost);

				tr0 = ul;
			} else {
				tr0 = new y_node(sr0->parents, edge);
			}

			node = nodes.front();
			trapezoid *up = new trapezoid;
			up->leftp = p;
			up->rightp = node->rightp;
			up->top = node->top;
			up->bottom = edge;
			if (leftmost != NULL) {
				up->upper_left = leftmost;
				leftmost->upper_right = up;
			} else {
				up->upper_left = node->upper_left;
				if (node->upper_left != NULL)
					node->upper_left->upper_right = up;
			}

			trapezoid *down = new trapezoid;
			down->leftp = p;
			down->rightp = node->rightp;
			down->top = edge;
			down->bottom = node->bottom;
			if (leftmost != NULL) {
				down->lower_left = leftmost;
				leftmost->lower_right = down;
			} else {
				down->lower_left = node->lower_left;
				if (node->lower_left != NULL)
					node->lower_left->lower_right = down;
			}

			search_node *s_down = new leaf_node(tl0, 0, down);
			search_node *s_up = new leaf_node(tl0, 1, up);

			trapezoid *prev_node = nodes.front();
			for (size_t j = 1; j < nodes.size(); j++) {
				node = nodes[j];
				if (is_below(node->leftp, edge)) {
					up->rightp = (j + 1 < nodes.size()) ? node->rightp : q;

					trapezoid *prev_down = down;
					down = new trapezoid;
					down->leftp = node->leftp;
					down->rightp = (j + 1 < nodes.size()) ? node->rightp : q;
					down->top = edge;
					down->bottom = node->bottom;
					s_down = new leaf_node(down);

					down->upper_left = prev_down;
					prev_down->upper_right = down;
					if (prev_node->lower_right != NULL) {
						prev_down->lower_right = prev_node->lower_right;
						if (prev_node->lower_right != NULL)
							prev_node->lower_right->lower_left = prev_down;
					} else {
						down->lower_left = node->lower_left;
						if (node->lower_left != NULL)
							node->lower_left->lower_right = down;
					}
				} else {
					trapezoid *prev_up = up;
					up = new trapezoid;
					up->leftp = node->leftp;
					up->rightp = (j + 1 < nodes.size()) ? node->rightp : q;
					up->top = node->top;
					up->bottom = edge;
					s_up = new leaf_node(up);

					up->lower_left = prev_up;
					prev_up->lower_right = up;
					if (prev_node->upper_right != NULL) {
						prev_up->upper_right = prev_node->upper_right;
						if (prev_node->upper_right != NULL)
							prev_node->upper_right->upper_left = prev_up;
					} else {
						up->upper_left = node->upper_left;
						if (node->upper_left != NULL)
							node->upper_left->upper_right = up;
					}

					down->rightp = (j + 1 < nodes.size()) ? node->rightp : q;
				}
				if (j + 1 < nodes.size()) {
					search_node *sx0 = node->s;

					search_node *tx0 = new y_node(sx0->parents, edge);

					s_down->add_parent(tx0, 0);
					s_up->add_parent(tx0, 1);
				} else {
					s_down->add_parent(tr0, 0);
					s_up->add_parent(tr0, 1);
				}
				prev_node = node;
			}
			node = nodes.back();
			if (rightmost != NULL) {
				up->upper_right = rightmost;
				rightmost->upper_left = up;
				down->lower_right = rightmost;
				rightmost->lower_left = down;
			} else {
				up->upper_right = node->upper_right;
				if (node->upper_right != NULL)
					node->upper_right->upper_left = up;
				down->lower_right = node->lower_right;
				if (node->lower_right != NULL)
					node->lower_right->lower_left = down;
			}

			for (size_t j = 0; j < nodes.size(); j++)
				delete nodes[j]->s;
		}
	}

	q = read_unsigned_integer<int>();
	for (int i = 0; i < q; i++) {
		int x = read_unsigned_integer<int>();
		int y = read_unsigned_integer<int>();
		pair<int, int> edge(x, y);
		pair<trapezoid *, bool> tp = s->search(edge, edge);
		if (edge == tp.first->leftp) {
			int idx = poly_vertex[tp.first->leftp];
			printf("%d\n", idx < 0 ? idx : idx + 1);
		} else {
			int orientation = direction[tp.first->bottom];
			if (orientation >= 0 && orientation < n) {
				printf("%d\n", orientation + 1);
			} else {
				printf("%d\n", (tp.second && orientation >= 0) ? (orientation - n + 1) : -1);
			}
		}
	}

	//delete_search_node(s);

	return 0;
}

int main()
{
	setlinebuf(stdout);
	srand((unsigned int) time(NULL));
	solve_problem();
	return 0;
}
