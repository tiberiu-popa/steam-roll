#include <fstream>
#include <deque>
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

void read_input(const char *filename, vector<int> &v, int &k)
{
	ifstream f(filename);
	int n;

	f >> n >> k;
	for (int i = 0; i < n; i++) {
		int elem;
		f >> elem;
		v.push_back(elem);
	}

	f.close();
}

void write_output(const char *filename, long long result)
{
	ofstream f(filename);

	f << result << endl;

	f.close();
}

long long solve_problem(vector<int> &v, size_t k)
{
	deque< pair<int, size_t> > window;
	long long result = 0;

	for (size_t i = 0; i < v.size(); i++) {
		int elem = v[i];
		while (!window.empty()) {
			pair<int, size_t> entry = window.back();
			if (entry.first >= elem)
				window.pop_back();
			else
				break;
		}
		window.push_back(pair<int, size_t>(elem, i));
		if (i + 1 >= k) {
			pair<int, size_t> entry = window.front();
			result += entry.first;
			if (entry.second == i + 1 - k)
				window.pop_front();
		}
	}

	return result;
}

int main()
{
	vector<int> v;
	long long result;
	int k;

	read_input("deque.in", v, k);
	result = solve_problem(v, (size_t) k);
	write_output("deque.out", result);

	return 0;
}
