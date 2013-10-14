#include <algorithm>
#include <limits>
#include <map>
#include <queue>
#include <vector>
#include <cstdio>

#define pow2s(x) (((size_t) 1) << (x))

#define MAX_N 100
#define COMPONENT_SIZE 9
#define NUM_CONSTRAINTS 4

using namespace std;

vector<int> neighbours[MAX_N];
vector<int> components[MAX_N];
vector<int> bitree[MAX_N];
vector<int> articulations;
int weights[MAX_N][MAX_N];
int visited[MAX_N];
int parent[MAX_N];
int lowlink[MAX_N];
int indices[MAX_N];
int head[MAX_N];
bool articulated[MAX_N];
int current_index;
int num_components;

size_t leaf_index[COMPONENT_SIZE];

pair<int, unsigned int> dp[1 << COMPONENT_SIZE][COMPONENT_SIZE][NUM_CONSTRAINTS];
pair<int, unsigned int> optimum[MAX_N][NUM_CONSTRAINTS];
pair<int, unsigned int> intermediate[MAX_N][MAX_N][NUM_CONSTRAINTS];

size_t least_set_bit[1 << COMPONENT_SIZE];

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

template<typename T, typename S>
T ipow(T x, S e)
{
	T result = 1;
	T base = x;

	if (e > 0) {
		while (true) {
			if (e & 1) {
				result *= base;
			}
			e >>= 1;
			if (e <= 0)
				break;
			base *= base;
		}
	}

	return result;
}

void dfs(int root)
{
	int num_children = 0;
	int childlink = numeric_limits<int>::min();

	visited[root] = 1;
	indices[root] = current_index++;
	lowlink[root] = indices[root];
	for (vector<int>::iterator it = neighbours[root].begin(); it != neighbours[root].end(); ++it) {
		int node = *it;
		if (node != parent[root]) {
			if (!visited[node]) {
				++num_children;
				parent[node] = root;
				dfs(node);
				if (lowlink[node] < lowlink[root])
					lowlink[root] = lowlink[node];
				if (lowlink[node] > childlink)
					childlink = lowlink[node];
			} else {
				if (indices[node] < lowlink[root])
					lowlink[root] = indices[node];
			}
		}
	}
	if (parent[root] < 0) {
		if (num_children > 1)
			articulations.push_back(root);
	} else {
		if (indices[root] <= childlink)
			articulations.push_back(root);
	}
}

void articulated_dfs(int root, int current_component)
{
	components[current_component].push_back(root);
	visited[root] = 1;
	for (vector<int>::iterator it = neighbours[root].begin(); it != neighbours[root].end(); ++it) {
		int node = *it;
		if (node != parent[root]) {
			if (!visited[node]) {
				int next_component = current_component;
				if (articulated[root] && lowlink[node] >= indices[root] &&
						(current_component > 0 || components[current_component].size() > 1)) {
					next_component = num_components++;
					bitree[current_component].push_back(next_component);
					head[next_component] = root;
					components[next_component].push_back(root);
				}
				parent[node] = root;
				articulated_dfs(node, next_component);
			}
		}
	}
}

pair<int, unsigned int> max_pair(pair<int, unsigned int> x, pair<int, unsigned int> y)
{
	if (x.first > y.first)
		return x;
	else if (x.first < y.first)
		return y;
	else
		return make_pair(x.first, x.second + y.second);
}

void update_max_pair(pair<int, unsigned int> &x, pair<int, unsigned int> y)
{
	x = max_pair(x, y);
}

pair<int, unsigned int> sum_pair(pair<int, unsigned int> x, pair<int, unsigned int> y)
{
	return make_pair(x.first + y.first, x.second * y.second);
}

void build_least_set_bit()
{
	size_t max_mask = pow2s(COMPONENT_SIZE);
	for (size_t i = 1; i < max_mask; i++) {
		size_t j = 0;
		for (size_t k = i; (k & 1) == 0; j++, k >>= 1) { }
		least_set_bit[i] = j;
	}
}

void process_component(const vector<int> &component)
{
	vector<size_t> elems;
	vector<size_t> reduced_elems;
	size_t num_elems = component.size();
	size_t max_mask = pow2s(num_elems);

	for (size_t i = 0; i < max_mask; i++)
		for (size_t j = 0; j < num_elems; j++)
			fill(dp[i][j], dp[i][j] + NUM_CONSTRAINTS, make_pair(numeric_limits<int>::min(), 0));

	for (size_t j = 0; j < num_elems; j++)
		dp[0][j][0] = make_pair(0, 1);

	for (size_t i = 1; i < max_mask; i++) {
		elems.clear();
		for (size_t j = 0, k = i; j < num_elems; j++, k >>= 1)
			if (k & 1)
				elems.push_back(j);

		if (elems.size() == 1) {
			size_t celem = elems.front();
			copy(optimum[component[celem]], optimum[component[celem]] + NUM_CONSTRAINTS, dp[i][celem]);
		} else {
			for (size_t j = 0; j < elems.size(); j++) {
				size_t celem = elems[j];
				int vertex = component[celem];
				size_t reduced_mask = i & ~pow2s(celem);
				for (size_t k = reduced_mask; k > reduced_mask / 2; k = (k - 1) & reduced_mask) {
					vector<size_t> yelems;
					size_t x = i & ~k;
					for (size_t i0 = 0, tmp = k; i0 < num_elems; i0++, tmp >>= 1)
						if (tmp & 1) {
							if (weights[vertex][component[i0]] > numeric_limits<int>::min())
								yelems.push_back(i0);
						}

					for (vector<size_t>::iterator it = yelems.begin(); it != yelems.end(); ++it) {
						size_t e = *it;
						int v = component[e];
						pair<int, unsigned int> cand1 = max_pair(dp[k][e][0], max_pair(dp[k][e][1], dp[k][e][2]));
						if (cand1.first == numeric_limits<int>::min())
							continue;
						cand1.first += weights[vertex][v];
						for (int l = 0; l < NUM_CONSTRAINTS - 1; l++) {
							pair<int, unsigned int> cand0 = dp[x][celem][l];
							if (cand0.first > numeric_limits<int>::min())
								update_max_pair(dp[i][celem][l + 1], sum_pair(cand0, cand1));
						}
					}
				}
			}
		}
	}
}

pair<int, unsigned int> analyze_table(size_t component_size, pair<int, unsigned int> result, size_t main_pos, const vector<int> &leaves)
{
	size_t max_mask = pow2s(component_size) - 1;

	if (main_pos != (size_t) (-1))
		max_mask &= ~pow2s(main_pos);

	for (size_t i = max_mask; i > 0; i = (i - 1) & max_mask) {
		size_t j = least_set_bit[i];
		if (i == pow2s(j)) {
			size_t *p = find(leaf_index, leaf_index + leaves.size(), j);
			if (p != leaf_index + leaves.size())
				continue;
		}

		for (int k = 0; k < NUM_CONSTRAINTS; k++)
			update_max_pair(result, dp[i][j][k]);
	}

	return result;
}

void solve_deg3maxt(int n, int source)
{
	pair<int, unsigned int> result(numeric_limits<int>::min(), 0);

	fill(visited, visited + n, 0);
	fill(parent, parent + n, -1);
	current_index = 0;
	articulations.clear();
	dfs(source);

	sort(articulations.begin(), articulations.end());

	fill(articulated, articulated + n, false);
	for (size_t i = 0; i < articulations.size(); i++)
		articulated[articulations[i]] = true;

	for (int i = 0; i < n; i++) {
		bitree[i].clear();
		components[i].clear();
	}
	fill(visited, visited + n, 0);
	fill(parent, parent + n, -1);
	num_components = 0;
	head[num_components++] = -1;
	if (articulations.empty()) {
		for (int i = 0; i < n; i++)
			components[0].push_back(i);
	} else {
		articulated_dfs(source, num_components - 1);
	}

	for (int i = 0; i < n; i++) {
		optimum[i][0] = make_pair(0, 1);
		fill(optimum[i] + 1, optimum[i] + NUM_CONSTRAINTS, make_pair(numeric_limits<int>::min(), 0));
		for (int j = 0; j < n; j++)
			fill(intermediate[i][j], intermediate[i][j] + NUM_CONSTRAINTS - 1, make_pair(numeric_limits<int>::min(), 0));
	}

	vector< pair< pair<int, unsigned int>, int> > v[NUM_CONSTRAINTS - 1];
	vector<int> leaves;
	for (int i = num_components - 1; i >= 0; i--) {
		const vector<int> &component = components[i];
		leaves.clear();
		fill(leaf_index, leaf_index + component.size(), 0);
		for (size_t j = 0; j < component.size(); j++) {
			int node = component[j];
			if (node != head[i] && articulated[node]) {
				leaf_index[leaves.size()] = j;
				leaves.push_back(node);
			}
		}
		for (size_t j = 0; j < leaves.size(); j++) {
			int node = leaves[j];
			for (int l = 0; l < NUM_CONSTRAINTS - 1; l++)
				v[l].clear();
			for (int k = 0; k < num_components; k++)
				for (int l = 0; l < NUM_CONSTRAINTS - 1; l++)
					if (intermediate[node][k][l].first > numeric_limits<int>::min()) {
						v[l].push_back(make_pair(intermediate[node][k][l], k));
					}
			for (int l = 0; l < NUM_CONSTRAINTS - 1; l++) {
				sort(v[l].begin(), v[l].end(), greater< pair< pair<int, unsigned int>, int> >());
				for (vector< pair< pair<int, unsigned int>, int> >::iterator it = v[l].begin(); it != v[l].end(); ++it)
					update_max_pair(optimum[node][l + 1], it->first);
			}

			size_t x[3] = { 0, 0, 0 };
			for (int l = 0; l < 3; l++) {
				if (l > 0)
					x[l] = x[l - 1];
				size_t base = x[l];
				while (x[l] < v[0].size() && v[0][x[l]].first.first == v[0][base].first.first)
					x[l]++;
			}
			if (x[0] >= 2) {
				for (size_t i0 = 0; i0 < x[0]; i0++)
					for (size_t i1 = i0 + 1; i1 < x[0]; i1++)
						update_max_pair(optimum[node][2], sum_pair(v[0][i0].first, v[0][i1].first));
			} else if (x[1] >= 2) {
				for (size_t i1 = x[0]; i1 < x[1]; i1++)
					update_max_pair(optimum[node][2], sum_pair(v[0][0].first, v[0][i1].first));
			}

			size_t y[2] = { 0, 0 };
			for (int l = 0; l < 2; l++) {
				if (l > 0)
					y[l] = y[l - 1];
				size_t base = y[l];
				while (y[l] < v[1].size() && v[1][y[l]].first.first == v[1][base].first.first)
					y[l]++;
			}
			for (size_t i0 = 0; i0 < x[1]; i0++)
				for (size_t i1 = 0; i1 < y[1]; i1++)
					if (v[0][i0].second != v[1][i1].second)
						update_max_pair(optimum[node][3], sum_pair(v[0][i0].first, v[1][i1].first));

			if (x[0] >= 3) {
				for (size_t i0 = 0; i0 < x[0]; i0++)
					for (size_t i1 = i0 + 1; i1 < x[0]; i1++)
						for (size_t i2 = i1 + 1; i2 < x[0]; i2++)
							update_max_pair(optimum[node][3], sum_pair(v[0][i0].first, sum_pair(v[0][i1].first, v[0][i2].first)));
			} else if (x[1] >= 3) {
				if (x[0] == 1) {
					for (size_t i1 = x[0]; i1 < x[1]; i1++)
						for (size_t i2 = i1 + 1; i2 < x[1]; i2++)
							update_max_pair(optimum[node][3], sum_pair(v[0][0].first, sum_pair(v[0][i1].first, v[0][i2].first)));
				} else {
					for (size_t i2 = x[0]; i2 < x[1]; i2++)
						update_max_pair(optimum[node][3], sum_pair(v[0][0].first, sum_pair(v[0][1].first, v[0][i2].first)));
				}
			} else if (x[2] >= 3) {
				for (size_t i2 = x[1]; i2 < x[2]; i2++)
					update_max_pair(optimum[node][3], sum_pair(v[0][0].first, sum_pair(v[0][1].first, v[0][i2].first)));
			}
			for (int k = 0; k < NUM_CONSTRAINTS; k++)
				update_max_pair(result, optimum[node][k]);
		}

		process_component(component);

		int main_elem = head[i];
		size_t main_pos = (size_t) -1;
		if (i > 0) {
			for (size_t k = 0; k < component.size(); k++)
				if (component[k] == main_elem) {
					main_pos = k;
					break;
				}
			size_t max_mask = pow2s(component.size());
			for (size_t j = 1; j < max_mask; j++) {
				pair<int, unsigned int> *last = dp[j][main_pos];
				for (int k = 1; k < NUM_CONSTRAINTS; k++)
					update_max_pair(intermediate[main_elem][i][k - 1], last[k]);
			}
		}
		result = analyze_table(component.size(), result, main_pos, leaves);
	}

	if (result.first == 0)
		result.second += 1;
	printf("%d %u\n", result.first, result.second);
}

int solve_problem()
{
	int n, m;

	n = read_unsigned_integer<int>();
	m = read_unsigned_integer<int>();

	for (int i = 0; i < n; i++) {
		neighbours[i].clear();
		fill(weights[i], weights[i] + n, numeric_limits<int>::min());
	}

	for (int i = 0; i < m; i++) {
		int x = read_unsigned_integer<int>() - 1;
		int y = read_unsigned_integer<int>() - 1;
		int cost = read_signed_integer<int>();
		neighbours[x].push_back(y);
		neighbours[y].push_back(x);
		weights[x][y] = cost;
		weights[y][x] = cost;
	}

	solve_deg3maxt(n, 0);

	return 0;
}

int main()
{
	int num_tests;

	build_least_set_bit();

	num_tests = read_unsigned_integer<int>();
	for (int i = 0; i < num_tests; i++)
		solve_problem();

	return 0;
}
