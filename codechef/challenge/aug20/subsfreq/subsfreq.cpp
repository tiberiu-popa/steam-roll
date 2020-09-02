#include <algorithm>
#include <cstdio>
#include <cstring>
#include <map>
#include <vector>

#define MAX_N 500000
#define MAX_GROUP 100
#define MOD_PRIME 1000000007

int a[MAX_N];
int mult[MAX_N], tmp_mult[MAX_N];
int upto[MAX_N];
int result[MAX_N];
int fact[MAX_N + 1];

int addmod(int x, int y, int p)
{
	long long sum = x;
	sum += y;
	if (sum >= p)
		sum -= p;
	return (int) sum;
}

int mulmod(int x, int y, int p)
{
	long long prod = x;
	prod = (prod * y) % p;
	return (int) prod;
}

int invmod(int x, int p)
{
	int result[2][2] = { {1, 0}, {0, 1} };
	int base[2][2] = { {0, 1}, {1, 0}};

	int y = x % p;
	x = p;
	while (y > 0) {
		int q = x / y;
		int r = x % y;
		base[1][1] = -q;
		int tmp[2][2] = { {0, 0}, {0, 0} };
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 2; j++)
				for (int k = 0; k < 2; k++)
					tmp[i][j] += result[i][k] * base[k][j];
		memcpy(result, tmp, sizeof(result));
		x = y;
		y = r;
	}

	int inv = result[1][0];
	if (inv < 0)
		inv += p;
	return inv;
}

int comb(int n, int k, int p)
{
  int x = invmod(fact[k], p);
  int y = invmod(fact[n - k], p);
  return mulmod(fact[n], mulmod(x, y, p), p);
}

int skip_whitespace()
{
  int ch;
  while (true) {
    ch = getchar_unlocked();
    if (ch != ' ' && ch != '\n') {
      break;
    }
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

void print_vec(int *v, size_t len)
{
  for (size_t i = 0; i < len; i++) {
    printf("%d ", v[i]);
  }
  printf("\n");
}

int solve_problem()
{
  std::map<int, std::vector<int>> freqs;
  int n;

  n = read_unsigned_integer<int>();

  for (int i = 0; i < n; i++) {
    a[i] = read_unsigned_integer<int>();
    mult[i] = 0;
    tmp_mult[i] = 0;
    upto[i] = 1;
    result[i] = 0;
  }

  for (int i = 0; i < n; i++) {
    int elem = a[i] - 1;
    int f = ++mult[elem];
    freqs[f].push_back(elem);
  }

  int covered = 1;
  for (int f = 1; f <= n && !freqs[f].empty(); f++) {
    std::sort(freqs[f].rbegin(), freqs[f].rend());
    for (auto &k : freqs[f]) {
      int crt_mult = ++tmp_mult[k];
      int full_mult = mult[k];
      int pos = comb(full_mult, crt_mult, MOD_PRIME);
      covered = mulmod(covered, invmod(upto[k], MOD_PRIME), MOD_PRIME);
      result[k] = addmod(result[k], mulmod(covered, pos, MOD_PRIME), MOD_PRIME);
      upto[k] = addmod(upto[k], pos, MOD_PRIME);
      covered = mulmod(covered, upto[k], MOD_PRIME);
    }
  }

  for (int i = 0; i < n; i++) {
    printf("%d ", result[i]);
  }
  printf("\n");

  return 0;
}

int main()
{
  int num_tests;

  fact[0] = 1;
  for (int i = 1; i <= MAX_N; i++) {
    fact[i] = mulmod(fact[i - 1], i, MOD_PRIME);
  }

  num_tests = read_unsigned_integer<int>();
  for (int i = 0; i < num_tests; i++) {
    int ret = solve_problem();
    if (ret != 0) {
      fprintf(stderr, "Read error at case #%d\n", i + 1);
      break;
    }
  }
  return 0;
}
