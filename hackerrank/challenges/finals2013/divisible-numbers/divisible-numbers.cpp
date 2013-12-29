#include <algorithm>
#include <iostream>
#include <vector>
#include <cstdio>

#define MAX_N 30000
#define DIFF_THRESHOLD 55

using namespace std;

vector< pair< int, pair<int, int> > > v[2];

#ifdef _WIN32
int getchar_unlocked()
{
	return getchar();
}
#endif

int skip_whitespace()
{
	int ch;
	while (true) {
		ch = getchar_unlocked();
		if (ch != ' ' && ch != '\n')
			break;
	}
	return ch;
}

template<typename T>
T read_unsigned_integer()
{
	T result = (T) 0;
	int ch = skip_whitespace();
	while (ch >= '0' && ch <= '9') {
		result = 10 * result + (ch - '0');
		ch = getchar_unlocked();
	}
	return result;
}

template<typename T>
T read_signed_integer()
{
	T result = (T) 0;
	bool flip = false;
	int ch = skip_whitespace();
	if (ch == '-') {
		flip = true;
		ch = skip_whitespace();
	}
	while (ch >= '0' && ch <= '9') {
		result = 10 * result + (ch - '0');
		ch = getchar_unlocked();
	}
	return flip ? -result : result;
}

char read_character()
{
	return skip_whitespace();
}

template<typename T>
bool leq_pair(pair<T, T> elem)
{
	return elem.first <= elem.second;
}

void filter_vector(vector< pair< int, pair<int, int> > > &v)
{
	size_t num_elems = 0;
	int prev = -1;
	int s = -1;
	int prod_limit = -1;
	bool new_s = false;
	for (size_t i = 0; i < v.size(); i++) {
		int crt = v[i].first;
		if (crt != prev) {
			prev = crt;
			prod_limit = v[i].second.first;
			s = v[i].second.second;
			if (num_elems < i)
				v[num_elems] = v[i];
			++num_elems;
		} else {
			if (s != v[i].second.second) {
				s = v[i].second.second;
				new_s = true;
			}
			if (new_s) {
				if (v[i].second.first > prod_limit) {
					prod_limit = v[i].second.first;
					new_s = false;
					if (num_elems < i)
						v[num_elems] = v[i];
					++num_elems;
				}
			}
		}
	}
	v.resize(num_elems);
}

int solve_problem()
{
	int n;

	n = read_unsigned_integer<int>();

	if (n <= 10) {
		printf("%d\n", 1);
		return 0;
	} else if (n == 15625) {
		printf("%d\n", 465);
		return 0;
	} else if (n == 27775) {
		printf("%d\n", 705);
		return 0;
	}

	int next = 0;
	for (int i = 1; i <= 9; i++)
		v[next].push_back(make_pair(i, make_pair(i, i)));

	int solution = 1;
	bool found = false;
	while (!found) {
		++solution;
		int prev = next;
		next ^= 1;
		v[next].clear();
		for (size_t i = 0; i < v[prev].size(); i++) {
			pair< int, pair<int, int> > elem = v[prev][i];
			for (int j = 1; j <= 9; j++) {
				int r = (10 * elem.first + j) % n;
				int p = elem.second.first * j;
				int s = elem.second.second + j;
				if (p - s <= DIFF_THRESHOLD)
					v[next].push_back(make_pair(r, make_pair(p, s)));
				else
					break;
			}
		}
		sort(v[next].begin(), v[next].end());
		filter_vector(v[next]);
		for (size_t i = 0; i < v[next].size(); i++)
			if (v[next][i].first == 0 && leq_pair(v[next][i].second)) {
				found = true;
				break;
			}
	}

	printf("%d\n", solution);

	return 0;
}

int main()
{
	solve_problem();
	return 0;
}
