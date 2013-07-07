#include <algorithm>
#include <cstdio>

#define MAX_N 100000
#define MAX_NUM_BITS 18
#define MAX_HEIGHT 100
#define MOD_PRIME 1000000009

using namespace std;

struct entry {

	int values[2];
	int position;

	bool operator<(const entry &rhs) const
	{
		if (values[0] != rhs.values[0])
			return values[0] < rhs.values[0];
		return values[1] < rhs.values[1];
	}

	bool operator==(const entry &rhs) const
	{
		return values[0] == rhs.values[0] && values[1] == rhs.values[1];
	}

};

int h[MAX_N];
int dh[MAX_N];

int prefix[MAX_NUM_BITS][MAX_N];
entry merger[MAX_N];

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

int addmod(int x, int y)
{
	int sum = x + y;
	if (sum >= MOD_PRIME)
		sum -= MOD_PRIME;
	return sum;
}

int lcp(int x, int y, int k, int n)
{
	int result = 0;
	if (x == y) {
		result = n - x;
	} else {
		for (int i = k - 1; i >= 0 && x < n && y < n; i--)
			if (prefix[i][x] == prefix[i][y]) {
				int offset = 1 << i;
				x += offset;
				y += offset;
				result += offset;
			}
	}
	return result;
}

int solve_problem()
{
	int n;

	n = read_unsigned_integer<int>();
	for (int i = 0; i < n; i++)
		h[i] = read_signed_integer<int>();
	for (int i = 1; i < n; i++)
		dh[i - 1] = h[i] - h[i - 1];

	--n;
	for (int i = 0; i < n; i++)
		prefix[0][i] = MAX_HEIGHT + dh[i];

	int k = 0;
	for (int diff = 1; diff < n; k++, diff <<= 1) {
		for (int i = 0; i < n; i++) {
			merger[i].values[0] = prefix[k][i];
			merger[i].values[1] = i + diff < n ? prefix[k][i + diff] : -1;
			merger[i].position = i;
		}
		sort(merger, merger + n);
		for (int i = 0; i < n; i++) {
			const entry &elem = merger[i];
			if (i > 0 && merger[i] == merger[i - 1])
				prefix[k + 1][elem.position] = prefix[k + 1][merger[i - 1].position];
			else
				prefix[k + 1][elem.position] = i;
		}
	}

	int result = n - merger[0].position;
	for (int i = 1; i < n; i++) {
		int x = merger[i - 1].position;
		int y = merger[i].position;
		result = addmod(result, n - y - lcp(x, y, k, n));
	}

	printf("%d\n", result);

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
