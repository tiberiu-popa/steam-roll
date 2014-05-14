#include <algorithm>
#include <cstdio>

using namespace std;

int solve_problem(int num_case)
{
  double c, f, x;

  if (scanf("%lf %lf %lf", &c, &f, &x) != 3)
    return 1;

  int num_cookies = (int) (x / c - 2. / f);
  if (num_cookies < 0)
    num_cookies = 0;

  double result = x / (2 + num_cookies * f);
  for (int i = 0; i < num_cookies; i++)
    result += c / (2 + i * f);

  printf("Case #%d: %.7lf\n", num_case, result);

  return 0;
}

int main()
{
  int num_tests;

  if (scanf("%d", &num_tests) != 1) {
    return 1;
  }
  for (int i = 0; i < num_tests; i++) {
    int ret = solve_problem(i + 1);
    if (ret != 0) {
      return ret;
    }
  }

  return 0;
}
