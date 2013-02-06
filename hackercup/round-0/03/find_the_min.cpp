#include <map>
#include <set>
#include <cstdio>

#define WINDOW_SIZE 100001
#define NUM_BUCKETS 317
#define BUCKET_SIZE NUM_BUCKETS

using namespace std;

map<int, int> counter;
set<int> elems[NUM_BUCKETS];
int seq[WINDOW_SIZE];

int main()
{
	int m, n, k, a, b, c, r;
	int i, x;

	scanf("%d", &m);
	for (int num_cases = 1; num_cases <= m; num_cases++) {
		scanf("%d %d", &n, &k);
		scanf("%d %d %d %d", &a, &b, &c, &r);
		counter.clear();
		for (int j = 0; j < NUM_BUCKETS; j++)
			elems[j].clear();
		for (i = 0; i <= k; i++) {
			counter[i] = 0;
			seq[i] = -1;
		}
		x = a;
		if (x <= k) {
			counter[x]++;
			elems[x / BUCKET_SIZE].insert(x);
			seq[0] = x;
		}
		for (i = 1; i < k; i++) {
			long long elem = b;
			elem = elem * x + c;
			x = elem % r;
			if (x <= k) {
				counter[x]++;
				elems[x / BUCKET_SIZE].insert(x);
				seq[i] = x;
			}
		}
		for (; i < n; i++) {
			for (int j = 0; j < NUM_BUCKETS; j++)
				if (elems[j].size() < BUCKET_SIZE) {
					for (int l = j * BUCKET_SIZE; l < (j + 1) * BUCKET_SIZE; l++)
						if (elems[j].find(l) == elems[j].end()) {
							counter[l]++;
							elems[l / BUCKET_SIZE].insert(l);
							seq[i % (k + 1)] = l;
							j = NUM_BUCKETS;
							break;
						}
				}
			int num_elems = 0;
			for (int j = 0; j < NUM_BUCKETS; j++)
				num_elems += (int) elems[j].size();
			if (num_elems == k + 1)
				break;
			int to_delete = seq[(i - k) % (k + 1)];
			if (--counter[to_delete] == 0) {
				elems[to_delete / BUCKET_SIZE].erase(to_delete);
			}
		}
		printf("Case #%d: %d\n", num_cases, seq[(n - 1) % (k + 1)]);
	}

	return 0;
}
