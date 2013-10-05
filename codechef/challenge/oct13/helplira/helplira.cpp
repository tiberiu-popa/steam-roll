#include <algorithm>
#include <complex>
#include <limits>
#include <vector>
#include <cmath>
#include <cstdio>

using namespace std;

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

int solve_problem()
{
	vector<long long> areas;
	int triangle[3][2];
	int n;

	n = read_unsigned_integer<int>();
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < 3; j++)
			for (int k = 0; k < 2; k++)
				triangle[j][k] = read_signed_integer<int>();
		for (int j = 1; j < 3; j++)
			for (int k = 0; k < 2; k++)
				triangle[j][k] -= triangle[0][k];
		long long a = triangle[1][0];
		a *= triangle[2][1];
		long long b = triangle[2][0];
		b *= triangle[1][1];
		if (a >= b)
			areas.push_back(a - b);
		else
			areas.push_back(b - a);
	}

	long long min_value = numeric_limits<long long>::max();
	long long max_value = numeric_limits<long long>::min();
	int min_index = -1, max_index = -1;
	int index = n;
	for (vector<long long>::const_reverse_iterator it = areas.rbegin(); it != areas.rend(); ++it) {
		long long value = *it;
		if (value < min_value) {
			min_value = value;
			min_index = index;
		}
		if (value > max_value) {
			max_value = value;
			max_index = index;
		}
		--index;
	}
	printf("%d %d\n", min_index, max_index);

	return 0;
}

int main()
{
	solve_problem();
	return 0;
}
