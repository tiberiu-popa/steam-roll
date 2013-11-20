#include <algorithm>
#include <functional>
#include <vector>
#include <cstdio>

using namespace std;

int solve_problem()
{
	vector<int> primes;
	vector<int> numbers;
	int n;

	if (scanf("%d", &n) != 1)
		return 1;

	int m = 2 * n * n;
	primes.push_back(2);
	primes.push_back(3);
	primes.push_back(5);
	primes.push_back(7);
	primes.push_back(11);
	primes.push_back(13);
	primes.push_back(17);
	primes.push_back(19);

	int prod = 1;
	for (size_t i = 0; i < primes.size(); i++) {
		prod *= primes[i];
		if (prod > m) {
			primes.resize(i);
			break;
		}
	}

	size_t sz = (size_t) n;
	for (size_t i = 1; i < primes.size(); i++) {
		numbers.clear();
		numbers.push_back(1);
		for (size_t j = 0; j <= i; j++) {
			int p = primes[j];
			size_t num_elems = numbers.size();
			for (size_t k = 0; k < num_elems; k++) {
				int x = numbers[k];
				while ((x *= p) <= m) {
					numbers.push_back(x);
				}
			}
		}
		if (numbers.size() >= sz)
			break;
	}

	sort(numbers.begin(), numbers.end(), greater<int>());
	numbers.resize(sz);
	for (size_t i = 0; i < numbers.size(); i++) {
		if (i > 0)
			printf(" ");
		printf("%d", numbers[i]);
	}
	printf("\n");

	return 0;
}

int main()
{
	solve_problem();
	return 0;
}
