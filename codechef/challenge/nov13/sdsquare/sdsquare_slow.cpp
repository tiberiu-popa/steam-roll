#include <algorithm>
#include <cstdio>

#define MAX_M 100000

using namespace std;

int sum[MAX_M + 2];

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

template<typename T>
T sqr(T x)
{
	return x * x;
}

template<typename T>
int get_lower_exp(T n)
{
	int e = 0;
	T k = (T) 1;
	n >>= 1;
	while (k <= n) {
		k <<= 1;
		++e;
	}
	return e;
}

template<typename T>
T i_usqrt(T n)
{
	int e = get_lower_exp(n);
	T left = 1, right = ((T) 1) << (e / 2 + 1);
	while (left < right) {
		T mid = left + (right - left) / 2;
		T sqm = mid * mid;
		if (sqm == n)
			return mid;
		else if (sqm < n)
			left = mid + 1;
		else
			right = mid;
	}
	return left;
}

bool is_perfect(int digit)
{
	return digit == 0 || digit == 1 || digit == 4 || digit == 9;
}

void build_sdsq()
{
	for (int i = 0; i <= MAX_M; i++) {
		long long j = sqr<long long>(i);
		bool perfect = true;
		while (j > 0) {
			if (!is_perfect(j % 10)) {
				perfect = false;
				break;
			}
			j /= 10;
		}
		sum[i + 1] = sum[i] + perfect;
	}
}

int solve_problem()
{
	long long a, b;

	a = read_unsigned_integer<long long>();
	b = read_unsigned_integer<long long>();

	long long x = i_usqrt(a);
	if (x * x < a)
		++x;
	long long y = i_usqrt(b);
	int result = sum[y + 1] - sum[x];
	printf("%d\n", result);

	return 0;
}

int main()
{
	int num_tests;

	build_sdsq();

	num_tests = read_unsigned_integer<int>();
	for (int i = 0; i < num_tests; i++)
		solve_problem();

	return 0;
}
