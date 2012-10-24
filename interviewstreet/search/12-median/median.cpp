#include <iostream>
#include <map>
#include <set>
#include <vector>

using namespace std;

struct node {

	long long value;
	int pos;

	node(long long value, int pos) : value(value), pos(pos) { }

	bool operator< (const node &rhs) const {
		if (value < rhs.value)
			return true;
		else if (value == rhs.value)
			return pos < rhs.pos;
		else
			return false;
	}

};

void solve_problem()
{
	map< long long, vector<int> > positions;
	set<node> low, high;
	int n;

	cin >> n;
	for (int i = 0; i < n; i++) {
		char type;
		long long x;
		cin >> type >> x;
		size_t total_size = low.size() + high.size();
		if (type == 'a') {
			positions[x].push_back(i);
			node elem(x, i);
			if (total_size & 1) {
				set<node>::iterator min_high = high.begin();
				if (x < min_high->value) {
					low.insert(elem);
				} else {
					low.insert(*min_high);
					high.erase(min_high);
					high.insert(elem);
				}
			} else {
				if (total_size == 0) {
					high.insert(elem);
				} else {
					set<node>::reverse_iterator max_low = low.rbegin();
					if (x <= max_low->value) {
						high.insert(*max_low);
						low.erase(--max_low.base());
						low.insert(elem);
					} else {
						high.insert(elem);
					}
				}
			}
		} else {
			vector<int> &posvec = positions[x];
			if (posvec.empty()) {
				cout << "Wrong!" << endl;
				continue;
			} else {
				node elem(x, posvec.back());
				if (low.erase(elem)) {
					if (total_size & 1) {
						set<node>::iterator min_high = high.begin();
						low.insert(*min_high);
						high.erase(min_high);
					}
				} else if (high.erase(elem)) {
					// total_size must be > 0
					if (!(total_size & 1)) {
						set<node>::reverse_iterator max_low = low.rbegin();
						high.insert(*max_low);
						low.erase(--max_low.base());
					}
				}
				posvec.pop_back();
			}
		}
		total_size = low.size() + high.size();
		if (total_size & 1) {
			cout << high.begin()->value << endl;
		} else {
			if (total_size > 0) {
				long long sum = low.rbegin()->value;
				sum += high.begin()->value;
				if (sum & 1) {
					if (sum >= 0)
						cout << sum / 2 << ".5" << endl;
					else
						cout << '-' << -sum / 2 << ".5" << endl;
				} else {
					cout << sum / 2 << endl;
				}
			} else {
				cout << "Wrong!" << endl;
			}
		}
	}
}

int main()
{
	solve_problem();
	return 0;
}
