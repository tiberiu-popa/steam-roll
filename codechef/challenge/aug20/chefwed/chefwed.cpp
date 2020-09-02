#include <cstdio>
#include <cstring>

#define MAX_N 1000
#define MAX_GROUP 100

int f[MAX_N];
int g[MAX_GROUP];
int h[MAX_GROUP];
int query[MAX_N][MAX_N];
int dp[MAX_N + 1];
int conv[MAX_N];
int changed[MAX_N];

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

int solve_problem()
{
  int n, k;
  int max_g;

  n = read_unsigned_integer<int>();
  k = read_unsigned_integer<int>();

  memset(g, 0, sizeof(g));

  for (int i = 0; i < n; i++) {
    f[i] = read_unsigned_integer<int>();
    g[f[i] - 1]++;
  }

  max_g = g[0];
  for (int i = 1; i < MAX_GROUP; i++) {
    if (g[i] > max_g) {
      max_g = g[i];
    }
  }

  for (int i = 0; i < n; i++) {
    int r = 0;
    memset(h, 0, sizeof(h));
    for (int j = i; j < n; j++) {
      int val = ++h[f[j] - 1];
      if (val == 2) {
        r += 2;
      } else if (val > 2) {
        r++;
      }
      query[i][j] = r;
    }
  }

  dp[0] = 0;
  for (int i = 1; i <= n; i++) {
    dp[i] = -1;
  }

  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j <= n; j++) {
      int cand = dp[i] + k + query[i][j - 1];
      if (dp[j] < 0 || cand < dp[j]) {
        dp[j] = cand;
      }
    }
  }

  printf("%d\n", dp[n]);

  return 0;
}

int main()
{
  int num_tests;

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
