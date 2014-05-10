#include <algorithm>
#include <limits>
#include <vector>
#include <cmath>
#include <cstdio>
#include <cstring>

using namespace std;

template<typename T>
int get_bit(T n, int bit)
{
	return (n & (1 << bit)) != 0;
}

template<typename T>
int get_msb(T n)
{
  int k = 0;
  for (; n > 0; n >>= 1)
    k++;
  return k;
}

template<typename T>
int get_lower_exp(T n)
{
  return get_msb(n >> 1);
}

template<typename T>
int get_upper_exp(T n)
{
  return get_msb(n - 1);
}

template<typename T>
T ipow(T x, int e)
{
  T result = 1;
  T base = x;

  if (e > 0) {
    while (true) {
      if (e & 1) {
        result *= base;
      }
      e >>= 1;
      if (e <= 0)
        break;
      base *= base;
    }
  }

  return result;
}

int ms_different_bit(int n, int m)
{
	return get_msb(n ^ m);
}

void split_interval(int n, vector< pair<int, int> > &v)
{
	v.clear();
	v.push_back(make_pair(0, 0));
	int x = 1;
	while (1) {
		int y = x << 1;
		if (y > n)
			break;
		v.push_back(make_pair(x, y - 1));
		x = y;
	}
	for (int t = x >> 1; t > 0; t >>= 1) {
		int y = x + t;
		if (y <= n) {
			v.push_back(make_pair(x, y - 1));
			x = y;
		}
	}
}

long long solve_subcase(pair<int, int> u, pair<int, int> v, pair<int, int> w)
{
	long long s = 0;
	int pw2 = 0;
	int pw3 = 0;

	int x = ms_different_bit(u.first, u.second);
	int y = ms_different_bit(v.first, v.second);
	int z = ms_different_bit(w.first, w.second);

	int upper_bit = max(x, y);
	int lower_bit = min(x, y);

	int highest_bit = max(z, upper_bit);
	int left_root = (u.first & v.first) >> highest_bit;
	int right_root = w.first >> highest_bit;
	if (left_root != right_root) {
		return s;
	}

	int lol = (x < y) ? u.first : v.first;
	for (int i = upper_bit - 1; i >= 0; i--) {
		if (i >= lower_bit) {
			if (i >= z) {
				int operand_bit = get_bit(lol, i);
				int result_bit = get_bit(w.first, i);
				if (!operand_bit) {
					if (result_bit) {
						return s;
					} else {
						++pw2;
					}
				}
			} else {
				++pw2;
			}
		} else {
			if (i >= z) {
				int result_bit = get_bit(w.first, i);
				if (!result_bit) {
					++pw3;
				}
			} else {
				pw2 += 2;
			}
		}
	}

	s = 1LL << pw2;
	s *= ipow(3LL, pw3);

	return s;
}

int solve_problem(int num_case)
{
	vector< pair<int, int> > u, v, w;
	int a, b, k;

	if (scanf("%d %d %d", &a, &b, &k) != 3)
		return 1;

	split_interval(a, u);
	split_interval(b, v);
	split_interval(k, w);

	long long result = 0;
	for (size_t i = 0; i < u.size(); i++) {
		for (size_t j = 0; j < v.size(); j++) {
			for (size_t k = 0; k < w.size(); k++) {
				result += solve_subcase(u[i], v[j], w[k]);
			}
		}
	}

	printf("Case #%d: %lld\n", num_case, result);

	return 0;
}

int main()
{
	int num_tests;

	if (scanf("%d", &num_tests) != 1)
		return 1;
	for (int i = 0; i < num_tests; i++)
		solve_problem(i + 1);

	return 0;
}
