#include <algorithm>
#include <fstream>
#include <vector>

using namespace std;

void read_input(const char *filename, vector<int> &v)
{
	ifstream f(filename);
	int n;

	f >> n;
	for (int i = 0; i < n; i++) {
		int elem;
		f >> elem;
		v.push_back(elem);
	}

	f.close();
}

void write_output(const char *filename, pair<int, size_t> result)
{
	ofstream f(filename);

	if (result.first < 0)
		f << result.first << endl;
	else
		f << result.first << ' ' << result.second << endl;

	f.close();
}

pair<int, size_t> solve_problem(const vector<int> &v)
{
	size_t num_votes = 0;
	int candidate = 0;
	for (vector<int>::const_iterator it = v.begin(); it != v.end(); ++it) {
		int value = *it;
		if (num_votes == 0)
			candidate = value;

		if (candidate == value)
			num_votes++;
		else
			num_votes--;
	}
	if (num_votes > 0)
		num_votes = (size_t) count(v.begin(), v.end(), candidate);

	if (num_votes > v.size() / 2)
		return pair<int, size_t>(candidate, num_votes);
	else
		return pair<int, size_t>(-1, num_votes);
}

int main()
{
	vector<int> v;

	read_input("elmaj.in", v);
	pair<int, size_t> result = solve_problem(v);
	write_output("elmaj.out", result);

	return 0;
}
