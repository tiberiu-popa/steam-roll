#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void solve_problem()
{
	vector<int> quadrant;
	int n;
	int q;

	cin >> n;
	for (int i = 0; i < n; i++) {
		int px, py;
		cin >> px >> py;
		if (px > 0) {
			if (py > 0)
				quadrant.push_back(0);
			else
				quadrant.push_back(3);
		} else {
			if (py > 0)
				quadrant.push_back(1);
			else
				quadrant.push_back(2);
		}
	}

	cin >> q;
	for (int i = 0; i < q; i++) {
		char type;
		int left, right;
		cin >> type >> left >> right;
		--left;
		--right;
		if (type == 'C') {
			vector<int> qc(4);
			for (int j = left; j <= right; j++)
				qc[quadrant[j]]++;
			for (size_t j = 0; j < qc.size(); j++) {
				if (j > 0)
					cout << ' ';
				cout << qc[j];
			}
			cout << endl;
		} else {
			if (type == 'X') {
				for (int j = left; j <= right; j++)
					quadrant[j] = 3 - quadrant[j];
			} else {
				for (int j = left; j <= right; j++)
					quadrant[j] ^= 1;
			}
		}
	}
}

int main()
{
	solve_problem();
	return 0;
}
