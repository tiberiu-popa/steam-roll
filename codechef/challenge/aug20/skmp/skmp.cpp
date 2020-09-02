#include <cstdio>
#include <cstring>

#define BUFFER_SIZE 100000

char str[BUFFER_SIZE + 1], pattern[BUFFER_SIZE + 1];
char res1[BUFFER_SIZE + 1], res2[BUFFER_SIZE + 1];

void add_string(int *x, const char *s)
{
  size_t len = strlen(s);
  for (size_t i = 0; i < len; i++) {
    x[s[i] - 'a']++;
  }
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
  int x[26], y[26], z[26];
  int k1 = 0, k2 = 0;
  size_t patlen;

  memset(x, 0, sizeof(x));
  memset(y, 0, sizeof(y));

  if (scanf("%s", str) != 1)
    return 1;

  if (scanf("%s", pattern) != 1)
    return 1;

  add_string(x, str);
  add_string(y, pattern);

  for (int i = 0; i < 26; i++) {
    z[i] = x[i] - y[i];
  }

  patlen = strlen(pattern);
  for (int i = 0; i < 26; i++) {
    char ch = 'a' + i;
    bool is_first = (ch == pattern[0]);
    if (is_first) {
      for (size_t j = 0; j < patlen; j++) {
        res1[k1++] = pattern[j];
      }
    }
    for (int j = 0; j < z[i]; j++) {
      res1[k1++] = res2[k2++] = ch;
    }
    if (is_first) {
      for (size_t j = 0; j < patlen; j++) {
        res2[k2++] = pattern[j];
      }
    }
  }
  res1[k1] = '\0';
  res2[k2] = '\0';

  if (strcmp(res1, res2) < 0) {
    printf("%s\n", res1);
  } else {
    printf("%s\n", res2);
  }

  return 0;
}

int main()
{
  int num_tests;

  if (scanf("%d", &num_tests) != 1)
    return 1;
  for (int i = 0; i < num_tests; i++) {
    int ret = solve_problem();
    if (ret != 0) {
      fprintf(stderr, "Read error at case #%d\n", i + 1);
      break;
    }
  }
  return 0;
}
