#include <algorithm>
#include <vector>
#include <cmath>
#include <cstdio>
#include <cstring>

#define MAX_N 12
#define MAX_M 8
#define MAX_K 12

using namespace std;

const unsigned long long uint_limit = 1ULL << 32;

class biguint {

private:

	vector<unsigned int> v;

public:

	biguint() { }

	biguint(unsigned int value)
	{
		if (value != 0)
			v.push_back(value);
	}

	biguint(const biguint &rhs)
	{
		v = rhs.v;
	}

	bool is_zero() const
	{
		return v.empty();
	}

	biguint& operator= (const biguint &rhs)
	{
		v = rhs.v;
		return *this;
	}

	biguint& operator+= (const biguint &rhs)
	{
		*this = *this + rhs;
		return *this;
	}

	biguint& operator*= (const biguint &rhs)
	{
		*this = *this * rhs;
		return *this;
	}

	biguint operator+ (const biguint &rhs) const
	{
		biguint result;
		size_t lhs_size = v.size();
		size_t rhs_size = rhs.v.size();
		size_t min_size = min(lhs_size, rhs_size);

		unsigned int carry = 0;
		for (size_t i = 0; i < min_size; i++) {
			unsigned long long sum = v[i];
			sum += rhs.v[i];
			sum += carry;
			if (sum >= uint_limit) {
				sum -= uint_limit;
				carry = 1;
			} else {
				carry = 0;
			}
			result.v.push_back((unsigned int) sum);
		}

		for (size_t i = min_size; i < lhs_size; i++) {
			unsigned long long sum = v[i];
			sum += carry;
			if (sum >= uint_limit) {
				sum -= uint_limit;
				carry = 1;
			} else {
				carry = 0;
			}
			result.v.push_back((unsigned int) sum);
		}

		for (size_t i = min_size; i < rhs_size; i++) {
			unsigned long long sum = rhs.v[i];
			sum += carry;
			if (sum >= uint_limit) {
				sum -= uint_limit;
				carry = 1;
			} else {
				carry = 0;
			}
			result.v.push_back((unsigned int) sum);
		}

		if (carry)
			result.v.push_back(carry);

		return result;
	}

	biguint operator* (const biguint &rhs) const
	{
		biguint result;
		if (is_zero() || rhs.is_zero())
			return result;

		result.v.resize(v.size() + rhs.v.size());
		for (size_t i = 0; i < v.size(); i++) {
			unsigned long long carry = 0;
			for (size_t j = 0; j < rhs.v.size(); j++) {
				unsigned long long prod = v[i];
				prod *= rhs.v[j];
				prod += carry;
				unsigned long long sum = result.v[i + j];
				sum += prod % uint_limit;
				carry = prod / uint_limit;
				if (sum >= uint_limit) {
					sum -= uint_limit;
					carry++;
				}
				result.v[i + j] = sum;
			}
			if (carry)
				result.v[i + rhs.v.size()] = carry;
		}
		if (result.v.back() == 0)
			result.v.resize(result.v.size() - 1);

		return result;
	}

	bool operator< (const biguint &rhs) const
	{
		size_t lhs_size = v.size();
		size_t rhs_size = rhs.v.size();
		if (lhs_size != rhs_size)
			return lhs_size < rhs_size;
		for (size_t i = lhs_size; i > 0;) {
			--i;
			if (v[i] != rhs.v[i])
				return v[i] < rhs.v[i];
		}
		return false;
	}

	bool operator> (const biguint &rhs) const
	{
		return rhs < *this;
	}

};

const int primes[] = { 2, 3, 5, 7 };
unsigned int comb[MAX_N + 1][MAX_N + 1];
int pexp[MAX_K][4];
int temp_v[MAX_M - 1];

int perms[48620][MAX_M - 1];
int num_perms;

void build_comb(int n)
{
	comb[0][0] = 1;
	for (int i = 1; i <= n; i++) {
		comb[i][0] = 1;
		for (int j = 1; j <= i; j++)
			comb[i][j] = comb[i - 1][j - 1] + comb[i - 1][j];
	}
}

void generate(int num_elems, int total_sum, int index, int partial_sum)
{
	if (index < num_elems) {
		int inf_sum = (index < num_elems - 1) ? partial_sum : total_sum;
		for (int i = inf_sum; i <= total_sum; i++) {
			int j = i - partial_sum;
			temp_v[index] = j;
			generate(num_elems, total_sum, index + 1, i);
		}
	} else {
		for (int i = 0; i < num_elems; i++)
			perms[num_perms][i] = temp_v[i];
		num_perms++;
	}
}

unsigned int compute_perm_count(const int *v, int vsize, int num_elems)
{
	unsigned int perm_count = 1;
	for (int i = 0; i < vsize; i++) {
		perm_count *= comb[num_elems][v[i]];
		num_elems -= v[i];
	}
	return perm_count;
}

int solve_problem(int num_case)
{
	int r, n, m, k;
	int p[MAX_K];
	int best_v[MAX_M - 1];

	if (scanf("%d %d %d %d", &r, &n, &m, &k) != 4)
		return 1;

	num_perms = 0;
	generate(m - 1, n, 0, 0);

	printf("Case #%d:\n", num_case);
	for (int line = 0; line < r; line++) {
		for (int j = 0; j < k; j++)
			if (scanf("%d", &p[j]) != 1)
				return 2;

		for (int j = 0; j < k; j++) {
			int x = p[j];
			for (int u = 0; u < 4; u++) {
				pexp[j][u] = 0;
				while (x % primes[u] == 0) {
					pexp[j][u]++;
					x /= primes[u];
				}
			}
		}

		biguint best_prob = 0;
		for (int pos = 0; pos < num_perms; pos++) {
			int *v = perms[pos];
			biguint total_prob = 1;
			for (int i = 0; i < k; i++) {
				unsigned int prob = 1;
				if (5 <= m) {
					prob *= comb[v[3]][pexp[i][2]];
				}
				if (7 <= m) {
					prob *= comb[v[5]][pexp[i][3]];
				}
				if (m < 6) {
					prob *= comb[v[1]][pexp[i][1]];
					unsigned int term = 0;
					for (int j = 0; j <= pexp[i][0]; j += 2) {
						int left = pexp[i][0] - j;
						term += comb[v[0]][left] * comb[v[2]][j / 2];
					}
					prob *= term;
				} else {
					int sup6 = min(pexp[i][0], pexp[i][1]);
					unsigned int term = 0;
					for (int j = 0; j <= sup6; j++) {
						unsigned int factor = 0;
						int sup2 = pexp[i][0] - j;
						if (8 <= m) {
							for (int a = 0; a <= sup2; a += 3)
								for (int b = a; b <= sup2; b += 2)
									factor += comb[v[0]][sup2 - b] * comb[v[2]][(b - a) / 2] * comb[v[6]][a / 3];
						} else {
							for (int a = 0; a <= sup2; a += 2) {
								int left = sup2 - a;
								factor += comb[v[0]][left] * comb[v[2]][a / 2];
							}
						}
						term += factor * comb[v[4]][j] * comb[v[1]][pexp[i][1] - j];
					}
					prob *= term;
				}
				total_prob *= prob;
				if (total_prob.is_zero())
					break;
			}
			total_prob *= compute_perm_count(v, m - 1, n);
			if (total_prob > best_prob) {
				memcpy(best_v, v, ((unsigned int) (m - 1)) * sizeof(*v));
				best_prob = total_prob;
			}
		}

		for (int j = 0; j < m - 1; j++)
			for (int u = 0; u < best_v[j]; u++)
				printf("%d", j + 2);
		printf("\n");
	}

	return 0;
}

int main()
{
	int num_tests;

	if (freopen("good-luck.in", "rt", stdin) == NULL) {
		fprintf(stderr, "Could not reopen stdin\n");
		return 1;
	}
	if (freopen("good-luck.out", "wt", stdout) == NULL) {
		fprintf(stderr, "Could not reopen stdout\n");
		return 1;
	}

	if (scanf("%d", &num_tests) != 1)
		return 1;
	build_comb(MAX_N);
	for (int i = 0; i < num_tests; i++)
		solve_problem(i + 1);

	return 0;
}
