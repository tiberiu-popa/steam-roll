#include <algorithm>
#include <cstdio>

#define MAX_N 1000000

using namespace std;

int v[MAX_N];
long long tree[MAX_N];

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
T bit_get_element(const T *tree, const int *v, int l, int r)
{
	T sum = 0;

	while (l <= r) {
		int idx = (r & (r + 1)) - 1;
		if (idx + 1 >= l) {
			sum += tree[r];
		} else {
			sum += v[r];
			idx = r - 1;
		}
		r = idx;
	}

	return sum;
}

template<typename T>
void bit_update_element(T *tree, int tree_size, int idx, T value)
{
	v[idx] += value;
	while (idx < tree_size) {
		tree[idx] += value;
		idx |= (idx + 1);
	}
}

int solve_problem()
{
	int n, q;

	n = read_unsigned_integer<int>();
	q = read_unsigned_integer<int>();

	for (int i = 0; i < n; i++) {
		int value = read_unsigned_integer<int>();
		bit_update_element<long long>(tree, n, i, value);
	}

	for (int i = 0; i < q; i++) {
		char t = read_character();
		int x = read_unsigned_integer<int>();
		int y = read_unsigned_integer<int>();
		if (t == 'S') {
			long long result = bit_get_element(tree, v, x, y);
			printf("%lld\n", result);
		} else if (t == 'G') {
			bit_update_element<long long>(tree, n, x, y);
		} else {
			bit_update_element<long long>(tree, n, x, -y);
		}
	}

	return 0;
}

int main()
{
	solve_problem();
	return 0;
}
