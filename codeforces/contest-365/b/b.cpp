#include <limits>
#include <vector>
#include <cstdio>
#include <cstring>

#define MAX_N 100000

using namespace std;

int a[MAX_N];
bool fibo[MAX_N];

int solve_problem()
{
	int n;

	if (scanf("%d", &n) != 1)
		return 1;

	for (int i = 0; i < n; i++)
		if (scanf("%d", &a[i]) != 1)
			return 1;

	for (int i = 2; i < n; i++)
		if (a[i - 2] + a[i - 1] == a[i])
			fibo[i] = true;

	int result = 0;
	int run = 0;
	for (int i = 0; i < n; i++)
		if (fibo[i]) {
			++run;
			if (run > result)
				result = run;
		} else {
			run = 0;
		}

	result += 2;
	if (result > n)
		result = n;
	printf("%d\n", result);

	return 0;
}

int main()
{
	solve_problem();
	return 0;
}
