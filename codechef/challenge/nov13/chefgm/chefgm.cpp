#include <algorithm>
#include <limits>
#include <vector>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

#define pow2ll(x) (1LL << (x))

#define MAX_PILES 100
#define MAX_PILE_SIZE 45

using namespace std;

vector<int> piles[MAX_PILES];
int bits[MAX_PILE_SIZE];

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

template<class T>
void trim_unique(vector<T> &v)
{
	typename vector<T>::iterator it = unique(v.begin(), v.end());
	typename vector<T>::difference_type diff = it - v.begin();
	v.resize((typename vector<T>::size_type) diff);
}

int bits_sign()
{
	if (bits[0] > 0)
		return 1;
	else if (bits[0] < 0)
		return -1;

	for (int i = 1; i < MAX_PILE_SIZE; i++)
		if (bits[i] != 0)
			return 1;
	return 0;
}

const char * result_to_string(int result)
{
	switch (result) {
	case -1:
		return "EVEN";
	case 0:
		return "DON'T PLAY";
	case 1:
		return "ODD";
	default:
		return "";
	}
}

int solve_problem()
{
	int num_piles;

	num_piles = read_unsigned_integer<int>();
	for (int i = 0; i < num_piles; i++) {
		int pile_size = read_unsigned_integer<int>();
		piles[i].clear();
		for (int j = 0; j < pile_size; j++)
			piles[i].push_back(read_unsigned_integer<int>());
		sort(piles[i].begin(), piles[i].end());
		trim_unique(piles[i]);
		for (size_t j = 0; j < piles[i].size(); j++)
			piles[i][j] &= 1;
	}

	memset(bits, 0, sizeof(bits));
	for (int i = 0; i < num_piles; i++) {
		size_t j = 1;
		while (j < piles[i].size() && piles[i][j] == piles[i][j - 1])
			++j;
		int first = piles[i].front();
		int sgn = 2 * first - 1;
		if (j < piles[i].size()) {
			bits[0] += sgn * (int) (j - 1);
			int k = 1;
			for (++j; j < piles[i].size(); j++, k++)
				if (piles[i][j] == first)
					bits[k] += sgn;
			bits[k] += sgn;
		} else {
			bits[0] += sgn * (int) j;
		}
	}
	for (int i = MAX_PILE_SIZE - 1; i > 0; i--) {
		bits[i - 1] += bits[i] >> 1;
		bits[i] &= 1;
	}
	int result = bits_sign();
	printf("%s\n", result_to_string(result));

	return 0;
}

int main()
{
	int num_tests;

	num_tests = read_unsigned_integer<int>();
	for (int i = 0; i < num_tests; i++)
		solve_problem();

	return 0;
}
