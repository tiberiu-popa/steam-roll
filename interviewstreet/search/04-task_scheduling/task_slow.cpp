#include <iostream>
#include <set>
#include <limits>

using namespace std;

struct task {

	int d, m;
	int pos;

	bool operator< (const task& rhs) const
	{
		if (d < rhs.d) {
			return true;
		} else if (d == rhs.d) {
			if (m < rhs.m)
				return true;
			else if (m == rhs.m)
				return pos < rhs.pos;
		}
		return false;
	}

	friend ostream& operator<< (ostream& out, const task &obj);

};

ostream& operator<< (ostream& out, const task &obj)
{
	out << '(' << obj.d << ' ' << obj.m << ')';
	return out;
}

void solve_problem()
{
	int t;
	set<task> tasks;

	cin >> t;
	for (int i = 0; i < t; i++) {
		task elem;
		cin >> elem.d >> elem.m;
		elem.pos = i;
		tasks.insert(elem);
		int time = 0;
		int result = numeric_limits<int>::min();
		for (set<task>::iterator it = tasks.begin(); it != tasks.end(); ++it) {
			time += it->m;
			int diff = time - it->d;
			if (diff > result)
				result = diff;
		}
		cout << max(0, result) << endl;
	}
}

int main()
{
	solve_problem();
	return 0;
}
