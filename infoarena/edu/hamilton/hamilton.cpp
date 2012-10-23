#include <fstream>
#include <iostream>
#include <limits>
#include <vector>

using namespace std;

template<class T>
void print_vector(ostream &os, const vector<T> &v)
{
	for (typename vector<T>::const_iterator it = v.begin(); it != v.end(); ++it) {
		if (it != v.begin())
			os << ' ';
		os << *it;
	}
	os << endl;
}

template<class T>
ostream & operator <<(ostream &os, const vector<T> &v)
{
	unsigned int i = 0;
	for (typename vector<T>::const_iterator it = v.begin(); it != v.end(); ++it) {
		os << i << ": ";
		print_vector(os, *it);
		i++;
	}
	return os;
}

void read_input(const char *filename, vector< vector< pair<int, int> > > &neighbour_list)
{
	ifstream f(filename);
	int n, m;

	f >> n >> m;
	for (int i = 0; i < n; i++)
		neighbour_list.push_back(vector< pair<int, int> >());
	for (int i = 0; i < m; i++) {
		int x, y, cost;
		f >> x >> y >> cost;
		neighbour_list[y].push_back(pair<int, int>(x, cost));
	}

	f.close();
}

void write_output(const char *filename, int result)
{
	ofstream f(filename);

	if (result < numeric_limits<int>::max())
		f << result << endl;
	else
		f << "Nu exista solutie" << endl;

	f.close();
}

int solve_problem(const vector< vector< pair<int, int> > > &neighbour_list)
{
	vector< vector<int> > dp;
	int result = numeric_limits<int>::max();
	const size_t num_nodes = neighbour_list.size();
	const size_t num_masks = 1U << num_nodes;

	for (size_t i = 0; i < num_masks; i++)
		dp.push_back(vector<int>(num_nodes, numeric_limits<int>::max()));
	dp[1][0] = 0;

	// Consider only masks with the first bit set
	for (size_t i = 1; i < num_masks; i += 2) {
		for (size_t j = 1; j < num_nodes; j++) {
			unsigned int mask = 1U << j;
			if (i & mask) {
				size_t k = i & ~mask;
				const vector< pair<int, int> > &n_list = neighbour_list[j];
				for (vector< pair<int, int> >::const_iterator it = n_list.begin();
						it != n_list.end(); ++it) {
					pair<int, int> elem = *it;
					int node = elem.first;
					if (k & (1U << node)) {
						int candidate = dp[k][node];
						if (candidate < numeric_limits<int>::max()) {
							candidate += elem.second;
							dp[i][j] = min(dp[i][j], candidate);
						}
					}
				}
			}
		}
	}

	const vector< pair<int, int> > &n_list = neighbour_list.front();
	const vector<int> &final_dp = dp.back();
	for (vector< pair<int, int> >::const_iterator it = n_list.begin();
			it != n_list.end(); ++it) {
		pair<int, int> elem = *it;
		int sum = final_dp[elem.first];
		if (sum < numeric_limits<int>::max())
			sum += elem.second;
		result = min(sum, result);
	}

	return result;
}

int main()
{
	vector< vector< pair<int, int> > > neighbour_list;
	int result;

	read_input("hamilton.in", neighbour_list);
	result = solve_problem(neighbour_list);
	write_output("hamilton.out", result);

	return 0;
}
