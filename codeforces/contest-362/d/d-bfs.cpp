#include <algorithm>
#include <limits>
#include <queue>
#include <vector>
#include <cstdio>
#include <cstring>

#define MAX_N 100000
#define MAX_VALUE 1000000000

using namespace std;

vector< pair<int, int> > neighbours[MAX_N];
pair<int, int> v[MAX_N];
int node_queue[MAX_N];
int s[MAX_N];
int visited[MAX_N];
int comproot[MAX_N];
int num_components = 0;

static const int INSERTION_THRESHOLD = 16;

typedef pair<int, int> element_type;
element_type merge_temp[(MAX_N + 2) / 2];

static void __mergesort(element_type *v, element_type *tmp, int lo, int hi);
static void __merge(element_type *v, element_type *tmp, int lo, int mid, int hi);

void insertion_sort(element_type *v, int n)
{
	element_type value;
	int i, j;

	for (i = 1; i < n; i++) {
		value = v[i];
		for (j = i - 1; j >= 0 && value < v[j]; j--)
			v[j + 1] = v[j];
		v[j + 1] = value;
	}
}

void mergesort(element_type *v, int n)
{
	if (n < INSERTION_THRESHOLD) {
		insertion_sort(v, n);
		return;
	}

	/* Call the real mergesort routine */
	__mergesort(v, merge_temp, 0, n - 1);
}

void bfs(int root)
{
	int q = 0;
	node_queue[q++] = root;
	for (int p = 0; p < q; p++) {
		root = node_queue[p];
		visited[root] = 2;
		vector< pair<int, int> > &nodes = neighbours[root];
		for (vector< pair<int, int> >::iterator it = nodes.begin(); it != nodes.end(); ++it) {
			int node = it->first;
			if (visited[node] != 2)
				s[num_components] = min(s[num_components] + it->second, MAX_VALUE);
			if (!visited[node]) {
				visited[node] = 1;
				node_queue[q++] = node;
			}
		}
	}
}

int solve_problem()
{
	int n, m, p, q;
	int x = -1, y = -1;

	if (scanf("%d %d %d %d", &n, &m, &p, &q) != 4)
		return 1;

	for (int i = 0; i < m; i++) {
		int l;
		if (scanf("%d %d %d", &x, &y, &l) != 3)
			return 1;
		--x;
		--y;
		neighbours[x].push_back(make_pair(y, l));
		neighbours[y].push_back(make_pair(x, l));
	}

	for (int i = 0; i < n; i++)
		if (!visited[i]) {
			comproot[num_components] = i;
			bfs(i);
			++num_components;
		}

	if (num_components < q || num_components - q > p) {
		printf("NO\n");
		return 0;
	}

	if (m == 0 && p != 0 && num_components == q) {
		printf("NO\n");
		return 0;
	}

	for (int i = 0; i < num_components; i++)
		v[i] = make_pair(s[i], i);

	mergesort(v, num_components);

	queue< pair<int, int> > huffman[2];
	for (int i = 0; i < num_components; i++)
		huffman[0].push(v[i]);

	vector< pair<int, int> > result;
	pair<int, int> elems[2];
	for (int i = 0; i < num_components - q; i++) {
		for (int j = 0; j < 2; j++) {
			pair<int, int> elem(numeric_limits<int>::max(), -1);
			int pos = -1;
			for (int k = 0; k < 2; k++)
				if (!huffman[k].empty()) {
					pair<int, int> cand = huffman[k].front();
					if (cand < elem) {
						elem = cand;
						pos = k;
					}
				}
			huffman[pos].pop();
			elems[j] = elem;
		}
		x = comproot[elems[0].second];
		y = comproot[elems[1].second];
		result.push_back(make_pair(x, y));
		int t = elems[0].first + elems[1].first;
		if (t < MAX_VALUE / 2)
			t = 2 * t + 1;
		else
			t = MAX_VALUE;
		huffman[1].push(make_pair(t, elems[0].second));
	}

	p -= (num_components - q);

	for (int k = 0; k < p; k++)
		result.push_back(make_pair(x, y));

	printf("YES\n");
	for (size_t i = 0; i < result.size(); i++)
		printf("%d %d\n", result[i].first + 1, result[i].second + 1);

	return 0;
}

int main()
{
	solve_problem();
	return 0;
}

/**
 * Actual recursive merge sort function.
 *
 * The algorithm goes as follows: we split the vector in half and then we sort
 * the two halves separately; the final step consists of merging the sorted
 * halves into one big sorted array; as a minor optimization, when the size of
 * the array becomes too small, we switch to the useful insertion sort.
 */
void __mergesort(element_type *v, element_type *tmp, int lo, int hi)
{
	int mid;

	if (hi - lo < INSERTION_THRESHOLD) {
		insertion_sort(v + lo, hi - lo + 1);
		return;
	}

	mid = lo + ((hi - lo) >> 1);

	__mergesort(v, tmp, lo, mid);
	__mergesort(v, tmp, mid + 1, hi);

	__merge(v, tmp, lo, mid, hi);
}

void __merge(element_type *v, element_type *tmp, int lo, int mid, int hi)
{
	int length;
	int i, j, k;

	length = mid - lo + 1;

	/*
	 * Equivalent with:
	 *     for(i = 0; i < length; i++)
	 *         tmp[i] = v[i + lo];
	 */
	memcpy(tmp, v + lo, ((size_t) length) * sizeof(element_type));

	i = 0;
	j = mid + 1;

	while (i < length && !(v[j] < tmp[i]))
		i++;

	k = lo + i;

	while (i < length && j <= hi) {
		if (tmp[i] < v[j])
			v[k++] = tmp[i++];
		else
			v[k++] = v[j++];
	}

	/*
	 * Equivalent with:
	 *     while(i < length) {
	 *         v[k] = tmp[i];
	 *         i++;
	 *         k++;
	 *     }
	 */
	if (j > hi)
		memcpy(v + k, tmp + i, ((size_t) (length - i)) * sizeof(element_type));
}
