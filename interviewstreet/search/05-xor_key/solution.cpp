#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int NUM_BITS = 15;
const int NUM_VALUES = 1 << NUM_BITS;

vector<int> tree[(1 << (NUM_BITS + 1)) - 1];

bool checkExistence(const vector<int> &positions, int u, int v)
{
	vector<int>::const_iterator it = lower_bound(positions.begin(), positions.end(), u);
	if (it != positions.end()) {
		if (*it <= v)
			return true;
	}
	return false;
}

void solveProblem()
{
	int n, q;

	cin >> n >> q;

	int *x = new int[n];
	for (int i = 0; i < n; i++)
		cin >> x[i];

	for (int i = 0; i < (1 << (NUM_BITS + 1)) - 1; i++)
		tree[i].clear();

	int offset = (1 << NUM_BITS) - 1;
	for (int i = 0; i < n; i++)
		tree[offset + x[i]].push_back(i);

	for (int k = NUM_BITS - 1; k >= 0; k--) {
		int prevOffset = offset;
		offset = (1 << k) - 1;
		for (int i = 0; i <= offset; i++) {
			const vector<int> &fst = tree[prevOffset + 2 * i];
			const vector<int> &snd = tree[prevOffset + 2 * i + 1];
			vector<int> &crt = tree[offset + i];
			size_t total_size = fst.size() + snd.size();
			for (size_t j = 0; j < total_size; j++)
				crt.push_back(0);
			merge(fst.begin(), fst.end(), snd.begin(), snd.end(), crt.begin());
		}
	}

	for (int i = 0; i < q; i++) {
		int a, u, v;
		cin >> a >> u >> v;
		--u;
		--v;

		int start = 0;
		int diff = NUM_VALUES;
		int index = 0;
		for (int j = 1; j <= NUM_BITS; j++) {
			int s = 2 * index + 1;
			int t = s + 1;
			int mask = 1 << (NUM_BITS - j);
			int reducedQuery = a & mask;
			int reducedStart = start & mask;
			int reducedMiddle = (start + diff / 2) & mask;
			diff /= 2;
			if ((reducedQuery ^ reducedStart) > (reducedQuery ^ reducedMiddle)) {
				if (checkExistence(tree[s], u, v)) {
					index = s;
				} else {
					start += diff;
					index = t;
				}
			} else {
				if (checkExistence(tree[t], u, v)) {
					start += diff;
					index = t;
				} else {
					index = s;
				}
			}
		}
		index -= (1 << NUM_BITS) - 1;
		cout << (a ^ index) << endl;
	}
}

int main()
{
	int num_tests;

	cin >> num_tests;
	for (int i = 0; i < num_tests; i++)
		solveProblem();

	return 0;
}
