#include <random>
#include <unordered_map>
#include <cstdio>

#define MAX_N 500
#define MAX_M 46

using namespace std;

long long v[MAX_N];

void print_set(int m, long long mask)
{
	for (int i = 0; i < m; i++) {
		if (mask & 1)
			printf(" %lld", v[i]);
		mask >>= 1;
	}
	printf("\n");
}

int solve_problem(int num_case)
{
	default_random_engine generator;
	unordered_map<long long, long long> sum_index;
	int n;

	if (scanf("%d", &n) != 1)
		return 1;

	for (int i = 0; i < n; i++)
		if (scanf("%lld", &v[i]) != 1)
			return 1;

	int m = min(n, MAX_M);
	uniform_int_distribution<long long> distribution(1, (1LL << m) - 1);
	while (true) {
		long long mask = distribution(generator);
		long long sum = 0;
		for (int i = 0; i < m; i++)
			if (mask & (1LL << i))
				sum += v[i];

		unordered_map<long long, long long>::iterator it = sum_index.find(sum);
		if (it != sum_index.end()) {
			if (it->second != mask) {
				printf("Case #%d:\n", num_case);
				print_set(m, mask);
				print_set(m, it->second);
				break;
			}
		} else {
			sum_index[sum] = mask;
		}
	}

	return 0;
}

int main(int argc, char *argv[])
{
	const char *filenames[] = {
		"equal-sums.in",
		"equal-sums.out",
	};
	int num_tests;

	for (int i = 1; i < argc && i <= 2; i++)
		filenames[i - 1] = argv[i];

	if (freopen(filenames[0], "rt", stdin) == NULL) {
		fprintf(stderr, "Could not reopen stdin\n");
		return 1;
	}
	if (freopen(filenames[1], "wt", stdout) == NULL) {
		fprintf(stderr, "Could not reopen stdout\n");
		return 1;
	}

	if (scanf("%d", &num_tests) != 1)
		return 1;
	for (int i = 0; i < num_tests; i++)
		solve_problem(i + 1);

	return 0;
}
