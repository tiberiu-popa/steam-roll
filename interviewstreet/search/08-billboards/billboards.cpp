#include <iostream>
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
	vector<long long> v, u, dp;
	set<node> deck;
	int n, k;

	cin >> n >> k;
	for (int i = 0; i < n; i++) {
		long long elem;
		cin >> elem;
		v.push_back(elem);
	}

	u.push_back(v[0]);
	for (int i = 1; i < n; i++)
		u.push_back(u.back() + v[i]);

	dp.push_back(0);
	for (int i = 1; i <= k; i++)
		deck.insert(node(0, -i));
	for (int i = 0; i < n; i++) {
		deck.insert(node(dp[i] - u[i], i));
		long long val = u[i] + deck.rbegin()->value;
		dp.push_back(val);
		int j = i - k;
		if (i < k)
			deck.erase(node(0, j));
		else
			deck.erase(node(dp[j] - u[j], j));
	}
	cout << *dp.rbegin() << endl;
}

int main()
{
	solve_problem();
	return 0;
}
