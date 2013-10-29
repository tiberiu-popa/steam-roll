#include <algorithm>
#include <set>
#include <vector>
#include <cstdio>
#include <cstring>

#define BUFFER_SIZE 100000
#define MAX_N 100000
#define MAX_NUM_BITS 18
#define NUM_CHARACTERS 26

#define pow2i(x) (1 << (x))

using namespace std;

char str[BUFFER_SIZE + 10];
int revpos[MAX_N + 1];
int last_pos[NUM_CHARACTERS];
int prev_app[MAX_N];
vector< pair<int, int> > perms[MAX_N];
int table_perms[MAX_N][NUM_CHARACTERS];
int rmq[2][MAX_N][MAX_NUM_BITS];

struct prefix_entry {

	int key;
	int left, right;

	prefix_entry() : key(-1), left(-1), right(-1) { }

	prefix_entry(int key, int left, int right) : key(key), left(left), right(right) { }

	bool operator< (const prefix_entry &rhs) const
	{
		return key < rhs.key;
	}

	bool operator== (const prefix_entry &rhs) const
	{
		return key == rhs.key;
	}

	bool operator!= (const prefix_entry &rhs) const
	{
		return key != rhs.key;
	}

};

struct merge_entry {

	const prefix_entry *tokens[2];
	int position;
	int step;
	int n;

	merge_entry(int n) : n(n) { }

	bool operator< (const merge_entry &rhs) const;

	bool operator== (const merge_entry &rhs) const;

};

struct ring_entry {

	int idx;
	bool type;
	int value;

	ring_entry(int idx, bool type, int value) : idx(idx), type(type), value(value) { }

	bool operator< (const ring_entry &rhs) const
	{
		return idx < rhs.idx;
	}

};

static const int INSERTION_THRESHOLD = 16;

typedef merge_entry *element_type;
element_type merge_temp[(MAX_N + 2) / 2];

prefix_entry prefix[MAX_NUM_BITS][MAX_N];
merge_entry *merger[MAX_N];

static void __mergesort(element_type *v, element_type *tmp, int lo, int hi);
static void __merge(element_type *v, element_type *tmp, int lo, int mid, int hi);

char *trim_line(char *str)
{
	return strtok(str, "\r\n");
}

template<typename T>
int get_lower_exp(T n)
{
	int e = 0;
	T k = (T) 1;
	n >>= 1;
	while (k <= n) {
		k <<= 1;
		++e;
	}
	return e;
}

long long nC2(int n)
{
	long long s = n;
	return s * (n - 1) / 2;
}

void insertion_sort(element_type *v, int n)
{
	element_type value;
	int i, j;

	for (i = 1; i < n; i++) {
		value = v[i];
		for (j = i - 1; j >= 0 && *value < *v[j]; j--)
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

int query_rmq(int left, int right);

int lcp(int x, int y, int k, int n, bool updating)
{
	int result = 0;
	if (x == y) {
		result = min(n - x, 1 << k);
	} else {
		if (!updating) {
			if (prefix[k][x] == prefix[k][y])
				return pow2i(k);
		}

		if (k > 0) {
			if (prefix[k - 1][x] == prefix[k - 1][y]) {
				int mx = x + pow2i(k - 1);
				int my = y + pow2i(k - 1);
				result = pow2i(k - 1);
				if (mx < n && my < n) {
					if (prefix[k - 1][mx] == prefix[k - 1][my])
						result += pow2i(k - 1);
					else
						result += query_rmq(revpos[mx], revpos[my]);
					for (size_t i = 0; i < perms[mx].size(); i++) {
						int left = perms[mx][i].first;
						if (left - x < result) {
							int right = left - x + y;
							if (prev_app[left] >= x || prev_app[right] >= y) {
								if (prev_app[left] - prev_app[right] != x - y) {
									result = left - x;
									break;
								}
							}
						} else {
							break;
						}
					}
				}
			} else {
				result = query_rmq(revpos[x], revpos[y]);
			}
		}
	}
	return result;
}

void build_rmq(int k, int n)
{
	for (int i = 0; i < n; i++) {
		rmq[0][i][0] = (i == n - 1) ? 0 : lcp(merger[i]->position, merger[i + 1]->position, k, n, false);
		for (int j = 1; j < MAX_NUM_BITS - 1; j++) {
			int offset = 1 << j;
			if (i >= offset - 1) {
				rmq[0][i][j] = min(rmq[0][i][j - 1], rmq[0][i - offset / 2][j - 1]);
			} else {
				break;
			}
		}
	}
	for (int i = n - 1; i >= 0; i--) {
		rmq[1][i][0] = rmq[0][i][0];
		for (int j = 1; j < MAX_NUM_BITS - 1; j++) {
			int offset = 1 << j;
			if (i + offset <= n) {
				rmq[1][i][j] = min(rmq[1][i][j - 1], rmq[1][i + offset / 2][j - 1]);
			} else {
				break;
			}
		}
	}
}

int query_rmq(int left, int right)
{
	if (left > right)
		swap(left, right);
	--right;
	int e = get_lower_exp(right - left + 1);
	return min(rmq[0][right][e], rmq[1][left][e]);
}

bool merge_entry::operator< (const merge_entry &rhs) const
{
	if (*tokens[0] != *rhs.tokens[0])
		return *tokens[0] < *rhs.tokens[0];

	int z = lcp(position, rhs.position, step + 1, n, true);
	if (z == pow2i(step + 1))
		return false;

	int p0 = position + z;
	int p1 = rhs.position + z;
	if (p0 >= n)
		return true;
	if (p1 >= n)
		return false;

	int c0 = str[p0] - 'a';
	int c1 = str[p1] - 'a';
	return table_perms[position][c0] < table_perms[rhs.position][c1];
}

bool merge_entry::operator== (const merge_entry &rhs) const
{
	if (*tokens[0] != *rhs.tokens[0] || *tokens[1] != *rhs.tokens[1])
		return false;

	int z = lcp(position, rhs.position, step + 1, n, true);
	return (z == pow2i(step + 1));
}

void compute_results(vector<long long> &results, int n)
{
	set<ring_entry> ring;
	int pos = 0;
	long long s = 0;
	for (int i = 1; i < n; i++) {
		int x = merger[i - 1]->position;
		int y = merger[i]->position;
		int z = rmq[0][i - 1][0];
		s += z;
		if (i == 1) {
			revpos[x] = pos;
			ring.insert(ring_entry(pos++, true, x));
		}
		ring.insert(ring_entry(pos++, false, z));
		revpos[y] = pos;
		ring.insert(ring_entry(pos++, true, y));
	}

	results.push_back(nC2(n + 1) - s);
	for (int i = 0; i < n - 1; i++) {
		int pos = revpos[i];
		ring_entry elem(pos, true, i);
		set<ring_entry>::iterator it = ring.find(elem);
		set<ring_entry>::iterator next_it = it;
		++next_it;
		if (it != ring.begin()) {
			set<ring_entry>::iterator prev_it = it;
			--prev_it;
			if (next_it != ring.end()) {
				if (prev_it->value > next_it->value) {
					s -= prev_it->value;
					ring.erase(prev_it);
				} else {
					s -= next_it->value;
					ring.erase(next_it);
				}
			} else {
				s -= prev_it->value;
				ring.erase(prev_it);
			}
		} else {
			if (next_it != ring.end()) {
				s -= next_it->value;
				ring.erase(next_it);
			}
		}
		ring.erase(elem);
		results.push_back(nC2(n - i) - s);
	}
}

int solve_problem()
{
	if (fgets(str, sizeof(str), stdin) == NULL)
		return 1;

	char *p = trim_line(str);
	int n = (int) strlen(p);

	for (int i = 0; i < n / 2; i++) {
		char tmp = str[i];
		str[i] = str[n - 1 - i];
		str[n - 1 - i] = tmp;
	}

	fill(last_pos, last_pos + NUM_CHARACTERS, -1);
	for (int i = 0; i < n; i++) {
		int ch = str[i] - 'a';
		int prev = last_pos[ch];
		prev_app[i] = prev;
		for (int j = prev + 1; j <= i; j++)
			perms[j].push_back(make_pair(i, ch));
		last_pos[ch] = i;
	}
	for (int i = 0; i < n; i++) {
		int j = 0;
		memset(table_perms[i], 0xff, NUM_CHARACTERS * sizeof(*table_perms[i]));
		for (vector< pair<int, int> >::iterator it = perms[i].begin(); it != perms[i].end(); ++it)
			table_perms[i][it->second] = j++;
	}

	for (int i = 0; i < n; i++)
		prefix[0][i] = prefix_entry(0, i, i + 1);

	for (int i = 0; i < n; i++)
		merger[i] = new merge_entry(n);

	for (int i = 0; i <= n; i++)
		revpos[i] = i;

	int k = 0;
	prefix_entry me;
	for (int diff = 1; diff < n; k++, diff <<= 1) {
		for (int i = 0; i < n; i++) {
			merger[i]->tokens[0] = &prefix[k][i];
			merger[i]->tokens[1] = i + diff < n ? &prefix[k][i + diff] : &me;
			merger[i]->position = i;
			merger[i]->step = k;
		}
		mergesort(merger, n);
		for (int i = 0; i < n; i++) {
			const merge_entry *elem = merger[i];
			if (i > 0 && *merger[i] == *merger[i - 1]) {
				prefix[k + 1][elem->position].key = prefix[k + 1][merger[i - 1]->position].key;
			} else {
				prefix[k + 1][elem->position].key = i;
			}
			prefix[k + 1][elem->position].left = elem->tokens[0]->left;
			prefix[k + 1][elem->position].right = max(elem->tokens[0]->right, elem->tokens[1]->right);
			revpos[merger[i]->position] = i;
		}
		build_rmq(k + 1, n);
	}

	vector<long long> results;
	compute_results(results, n);
	for (vector<long long>::reverse_iterator it = results.rbegin(); it != results.rend(); ++it)
		printf("%lld\n", *it);

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

void __merge(element_type *v, element_type *tmp,
	int lo, int mid, int hi)
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

	while (i < length && !(*v[j] < *tmp[i]))
		i++;

	k = lo + i;

	while (i < length && j <= hi) {
		if (*tmp[i] < *v[j])
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
