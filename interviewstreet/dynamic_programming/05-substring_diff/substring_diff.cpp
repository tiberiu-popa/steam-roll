#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#define MAX_LENGTH 1500

using namespace std;

template<class T>
void print_vector(const vector<T> &v)
{
	for (typename vector<T>::const_iterator it = v.begin(); it != v.end(); ++it) {
		if (it != v.begin())
			cout << ' ';
		cout << *it;
	}
	cout << endl;
}

template<class T>
void print_vector(const char *title, const vector<T> &v)
{
	cout << title << ": ";
	print_vector(v);
}

size_t find_maxseq(const vector<int> &v, int k)
{
	size_t max_diff = 0;
	for (size_t i = 0, j = 0; i < v.size(); i++) {
		while (j + 1 < v.size() && v[j + 1] <= v[i] + k)
			j++;
		max_diff = max(max_diff, j - i);
	}
	return max_diff;
}

void solve_problem()
{
	vector<int> v;
	string p, q;
	int k;

	cin >> k >> p >> q;

	size_t answer = 0;
	for (size_t t = 0; t < p.size(); t++) {
		v.clear();
		v.push_back(0);
		for (size_t i = t, j = 0; i < p.size() && j < q.size(); i++, j++) {
			int value = (p[i] != q[j]);
			v.push_back(v.back() + value);
		}
		answer = max(answer, find_maxseq(v, k));
	}
	for (size_t t = 1; t < q.size(); t++) {
		v.clear();
		v.push_back(0);
		for (size_t i = 0, j = t; i < p.size() && j < q.size(); i++, j++) {
			int value = (p[i] != q[j]);
			v.push_back(v.back() + value);
		}
		answer = max(answer, find_maxseq(v, k));
	}
	cout << answer << endl;
}

int main()
{
	int num_tests;

	cin >> num_tests;
	for (int i = 0; i < num_tests; i++)
		solve_problem();

	return 0;
}
