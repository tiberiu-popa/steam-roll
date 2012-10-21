#include <algorithm>
#include <deque>
#include <fstream>
#include <iostream>

using namespace std;

template<class T>
struct double_deque {

	deque<T> first, second;

	T pop()
	{
		T elem;
		if (first.empty()) {
			elem = second.front();
			second.pop_front();
		} else {
			elem = first.front();
			bool remove_first = true;
			if (!second.empty()) {
				T candidate = second.front();
				if (candidate < elem) {
					elem = candidate;
					remove_first = false;
				}
			}
			if (remove_first)
				first.pop_front();
			else
				second.pop_front();
		}
		return elem;
	}

	void push(T elem)
	{
		first.push_back(elem);
	}

	void push_sum(T elem)
	{
		second.push_back(elem);
	}

	size_t size() const
	{
		return first.size() + second.size();
	}

};

void read_input(double_deque<long long> &v, int &n, int &m)
{
	ifstream f("scandura.in");

	f >> n >> m;
	for (int i = 0; i < n; i++) {
		long long elem;
		f >> elem;
		v.push(elem);
	}

	f.close();
}

void write_output(long long sum)
{
	ofstream f("scandura.out");
	f << sum << endl;
	f.close();
}

int main()
{
	double_deque<long long> v;
	int n, m;

	read_input(v, n, m);

	long long sum = 0;
	int r = (n + m - 2) % (m - 1);
	if (r > 0) {
		long long elem = 0;
		for (int i = 0; i <= r; i++)
			elem += v.pop();
		v.push_sum(elem);
		sum += elem;
	}
	while (v.size() > 1) {
		long long elem = 0;
		for (int i = 0; i < m; i++)
			elem += v.pop();
		v.push_sum(elem);
		sum += elem;
	}
	write_output(sum);

	return 0;
}
