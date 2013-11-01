#include <algorithm>
#include <cstdio>

using namespace std;

const long long v[] = {
	0LL, 1LL, 4LL, 9LL, 49LL, 100LL, 144LL, 400LL, 441LL, 900LL, 1444LL, 4900LL,
	9409LL, 10000LL, 10404LL, 11449LL, 14400LL, 19044LL, 40000LL, 40401LL,
	44100LL, 44944LL, 90000LL, 144400LL, 419904LL, 490000LL, 491401LL, 904401LL,
	940900LL, 994009LL, 1000000LL, 1004004LL, 1014049LL, 1040400LL, 1100401LL,
	1144900LL, 1440000LL, 1904400LL, 1940449LL, 4000000LL, 4004001LL, 4040100LL,
	4410000LL, 4494400LL, 9000000LL, 9909904LL, 9941409LL, 11909401LL,
	14010049LL, 14040009LL, 14440000LL, 19909444LL, 40411449LL, 41990400LL,
	49000000LL, 49014001LL, 49140100LL, 49999041LL, 90440100LL, 94090000LL,
	94109401LL, 99400900LL, 99940009LL, 100000000LL, 100040004LL, 100140049LL,
	100400400LL, 101404900LL, 101949409LL, 104040000LL, 104919049LL,
	110040100LL, 111049444LL, 114041041LL, 114490000LL, 144000000LL,
	190440000LL, 194044900LL, 400000000LL, 400040001LL, 400400100LL,
	404010000LL, 404090404LL, 409941009LL, 414000409LL, 414041104LL,
	441000000LL, 449440000LL, 490091044LL, 900000000LL, 990990400LL,
	991494144LL, 994140900LL, 1190940100LL, 1401004900LL, 1404000900LL,
	1409101444LL, 1444000000LL, 1449401041LL, 1490114404LL, 1990944400LL,
	4014109449LL, 4019940409LL, 4041144900LL, 4199040000LL, 4900000000LL,
	4900140001LL, 4901400100LL, 4914010000LL, 4914991449LL, 4941949401LL,
	4999904100LL, 9044010000LL, 9409000000LL, 9409194001LL, 9410940100LL,
	9900449001LL, 9940090000LL, 9994000900LL, 9999400009LL, 10000000000LL,
};
const size_t num_elems = sizeof(v) / sizeof(*v);

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
ptrdiff_t index_of(const T *v, size_t n, T elem)
{
	const T *u = lower_bound(v, v + n, elem);
	if (*u != elem)
		--u;
	return u - v;
}

int solve_problem()
{
	long long a, b;

	a = read_unsigned_integer<long long>();
	b = read_unsigned_integer<long long>();

	ptrdiff_t result = index_of(v, num_elems, b) - index_of(v, num_elems, a - 1);
	printf("%td\n", result);

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
