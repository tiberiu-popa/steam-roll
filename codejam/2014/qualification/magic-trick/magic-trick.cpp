#include <algorithm>
#include <cstdio>

#define GRID_SIZE 4

using namespace std;

int grid_a[GRID_SIZE][GRID_SIZE];
int grid_b[GRID_SIZE][GRID_SIZE];
int v[GRID_SIZE * GRID_SIZE];

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

int solve_problem(int num_case)
{
  int r_a, r_b;

  r_a = read_unsigned_integer<int>() - 1;
  for (int i = 0; i < GRID_SIZE; i++) {
    for (int j = 0; j < GRID_SIZE; j++) {
      grid_a[i][j] = read_unsigned_integer<int>() - 1;
    }
  }

  r_b = read_unsigned_integer<int>() - 1;
  for (int i = 0; i < GRID_SIZE; i++) {
    for (int j = 0; j < GRID_SIZE; j++) {
      grid_b[i][j] = read_unsigned_integer<int>() - 1;
    }
  }

  fill(v, v + GRID_SIZE * GRID_SIZE, false);
  for (int j = 0; j < GRID_SIZE; j++) {
    v[grid_a[r_a][j]] = true;
  }

  int num_common = 0;
  int result = -1;
  for (int j = 0; j < GRID_SIZE; j++) {
    int elem = grid_b[r_b][j];
    num_common += v[elem];
    if (v[elem]) {
      result = elem;
    }
  }

  printf("Case #%d: ", num_case);
  if (num_common == 0) {
    printf("Volunteer cheated!\n");
  } else if (num_common == 1) {
    printf("%d\n", result + 1);
  } else {
    printf("Bad magician!\n");
  }

  return 0;
}

int main()
{
  int num_tests;

  num_tests = read_unsigned_integer<int>();
  for (int i = 0; i < num_tests; i++) {
    int ret = solve_problem(i + 1);
    if (ret != 0) {
      return ret;
    }
  }

  return 0;
}
