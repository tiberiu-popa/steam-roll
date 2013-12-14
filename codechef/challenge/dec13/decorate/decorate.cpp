#include <algorithm>
#include <vector>
#include <cstdio>
#include <cctype>
#include <cstring>

#define BUFFER_SIZE 10
#define MAX_M 100000
#define MAX_NUM_BITS 18
#define MAX_PRIME 24

using namespace std;

const unsigned long long uint_limit = 1000000000;

struct merge_entry {

	pair<int, int> values;
	int position;

	bool operator<(const merge_entry &rhs) const
	{
		return values < rhs.values;
	}

	bool operator==(const merge_entry &rhs) const
	{
		return values == rhs.values;
	}

};

unsigned int muladdmod(unsigned int x, unsigned int y, unsigned int z, unsigned int p)
{
	unsigned long long prod = x;
	prod = (prod * y + z) % p;
	return (unsigned int) prod;
}

class biguint {

private:

public:
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
				result.v[i + j] = (unsigned int) sum;
			}
			if (carry)
				result.v[i + rhs.v.size()] = (unsigned int) carry;
		}
		if (result.v.back() == 0)
			result.v.resize(result.v.size() - 1);

		return result;
	}

	biguint & operator/= (unsigned int rhs)
	{
		if (!is_zero()) {
			unsigned long long result = 0;
			for (size_t i = v.size(); i > 0;) {
				--i;
				result = result * uint_limit + v[i];
				v[i] = (unsigned int) (result / rhs);
				result %= rhs;
			}
			while (!v.empty() && v.back() == 0)
				v.pop_back();
		}
		return *this;
	}

	unsigned int operator% (unsigned int rhs) const
	{
		unsigned int result = 0;
		if (is_zero())
			return result;

		for (size_t i = v.size(); i > 0;) {
			--i;
			result = muladdmod(result, uint_limit, v[i], rhs);
		}

		return result;
	}

	biguint pow(int e)
	{
		biguint result = 1;
		biguint base = *this;

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

	bool operator<= (const biguint &rhs) const
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
		return true;
	}

	bool operator== (const biguint &rhs) const
	{
		size_t lhs_size = v.size();
		size_t rhs_size = rhs.v.size();
		if (lhs_size != rhs_size)
			return false;
		for (size_t i = lhs_size; i > 0;) {
			--i;
			if (v[i] != rhs.v[i])
				return false;
		}
		return true;
	}

	bool operator!= (const biguint &rhs) const
	{
		return !(*this == rhs);
	}

	void read_from_string(const char *s)
	{
		size_t len = strlen(s);
		const unsigned int base = 1000000000;
		const size_t num_digits = 9;

		v.clear();
		for (size_t i = 0; i < len; i += num_digits) {
			size_t limit = min(len, i + num_digits);
			unsigned int x = 0;
			for (size_t j = i; j < limit; j++) {
				unsigned int ch = (unsigned int) (s[j] - '0');
				x = 10 * x + ch;
			}
			if (limit == i + num_digits) {
				*this = *this * base + x;
			} else {
				unsigned int factor = 1;
				for (size_t j = i; j < len; j++)
					factor *= 10;
				*this = *this * factor + x;
			}
		}
	}

	void read_directly_from_string(const char *s)
	{
		const size_t num_digits = 9;
		size_t len = strlen(s);
		size_t i;

		v.clear();
		for (i = len; i > num_digits;) {
			i -= num_digits;
			unsigned int x = 0;
			for (size_t j = i; j < i + num_digits; j++) {
				unsigned int ch = (unsigned int) (s[j] - '0');
				x = 10 * x + ch;
			}
			v.push_back(x);
		}
		if (i > 0) {
			unsigned int x = 0;
			for (size_t j = 0; j < i; j++) {
				unsigned int ch = (unsigned int) (s[j] - '0');
				x = 10 * x + ch;
			}
			v.push_back(x);
		}
	}

	void write_to_file(FILE *f) const
	{
		if (v.empty()) {
			fprintf(f, "0\n");
		} else {
			bool first = true;
			for (size_t i = v.size(); i > 0;) {
				--i;
				if (first) {
					first = false;
					fprintf(f, "%u", v[i]);
				} else {
					fprintf(f, "%09u", v[i]);
				}
			}
			fprintf(f, "\n");
		}
	}

};

static const int INSERTION_THRESHOLD = 16;

typedef pair<int, int> element_type;
element_type merge_temp[(MAX_M + 2) / 2];

size_t v[2 * MAX_M + 1];
char seq[MAX_M + 10];
char str[BUFFER_SIZE + 10];

vector<int> primes;
bool prime[MAX_PRIME + 1];

int prefix[MAX_NUM_BITS][MAX_M];
merge_entry merger[MAX_M];
int rmq[2][MAX_M][MAX_NUM_BITS];
int revpos[MAX_M];

static void __mergesort(element_type *v, element_type *tmp, int lo, int hi);
static void __merge(element_type *v, element_type *tmp, int lo, int mid, int hi);

char *trim_line(char *str)
{
	return strtok(str, "\r\n");
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

int ipow(int x, int e)
{
	int result = 1;
	int base = x;

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

template<typename T>
T i_lsqrt(T n)
{
	int e = get_lower_exp(n);
	T left = 1, right = ((T) 1) << (e / 2 + 1);
	while (left < right) {
		T mid = left + (right - left + 1) / 2;
		T sqm = mid * mid;
		if (sqm == n)
			return mid;
		else if (sqm < n)
			left = mid;
		else
			right = mid - 1;
	}
	return left;
}

void build_primes(int limit)
{
	int sqlim = i_lsqrt(limit);

	primes.push_back(2);
	fill(prime + 2, prime + limit + 1, true);
	for (int j = 4; j <= limit; j += 2)
		prime[j] = false;

	for (int i = 3; i <= sqlim; i += 2)
		if (prime[i]) {
			primes.push_back(i);
			for (int j = i * i; j <= limit; j += i)
				prime[j] = false;
		}
	for (int i = (sqlim + 1) | 1; i <= limit; i += 2)
		if (prime[i])
			primes.push_back(i);
}

int is_prime(int n)
{
	if (n <= MAX_PRIME) {
		return prime[MAX_PRIME];
	} else {
		for (size_t i = 0; i < primes.size(); i++) {
			int p = primes[i];
			if (p * p > n)
				break;
			if (n % p == 0)
				return 0;
		}
		return 1;
	}
}

void compute_factors(int n, vector< pair<int, int> > &factors)
{
	for (vector<int>::iterator it = primes.begin(); n > 1 && it != primes.end(); ++it) {
		int p = *it;
		int e = 0;
		while (n % p == 0) {
			++e;
			n /= p;
		}
		if (e > 0)
			factors.push_back(make_pair(p, e));
	}
	if (n > 1)
		factors.push_back(make_pair(n, 1));
}

void insertion_sort(element_type *v, int n)
{
	element_type value;
	int i, j;

	for (i = 1; i < n; i++) {
		value = v[i];
		for (j = i - 1; j >= 0 && value < v[j]; j--)
			v[j + 1] = v[j];
		v[j + 1] = value;
	}
}

void mergesort(element_type *v, int n)
{
	if (n < INSERTION_THRESHOLD) {
		insertion_sort(v, n);
		return;
	}

	/* Call the real mergesort routine */
	__mergesort(v, merge_temp, 0, n - 1);
}

void manacher(const char *seq, size_t n, int &num_elems)
{
	size_t palindrome_length = 0;
	num_elems = 0;
	for (size_t i = 0; i <= n;) {
		while (i > palindrome_length && seq[i] == seq[i - palindrome_length - 1]) {
			palindrome_length += 2;
			i++;
		}
		v[num_elems++] = palindrome_length;
		int found = 0;
		for (size_t j = palindrome_length; j > 0;) {
			j--;
			size_t k = 2 * (i - palindrome_length) + j;
			if (v[k] == j) {
				palindrome_length = j;
				found = 1;
				break;
			}
			v[num_elems++] = min(j, v[k]);
		}
		if (!found) {
			palindrome_length = 1;
			i++;
		}
	}
}

int lcp(int x, int y, int k, int n)
{
	int result = 0;
	if (x == y) {
		result = n - x;
	} else {
		for (int i = k - 1; i >= 0 && x < n && y < n; i--)
			if (prefix[i][x] == prefix[i][y]) {
				int offset = 1 << i;
				x += offset;
				y += offset;
				result += offset;
			}
	}
	return result;
}

void build_rmq(int k, int n)
{
	for (int i = 0; i < n; i++) {
		rmq[0][i][0] = (i == n - 1) ? 0 : lcp(merger[i].position, merger[i + 1].position, k, n);
		for (int j = 1; j < MAX_NUM_BITS - 1; j++) {
			int offset = 1 << j;
			if (i >= offset - 1) {
				rmq[0][i][j] = min(rmq[0][i][j - 1], rmq[0][i - offset / 2][j - 1]);
			} else {
				break;
			}
		}
	}
	for (int i = n - 1; i >= 0; i--) {
		rmq[1][i][0] = rmq[0][i][0];
		for (int j = 1; j < MAX_NUM_BITS - 1; j++) {
			int offset = 1 << j;
			if (i + offset <= n) {
				rmq[1][i][j] = min(rmq[1][i][j - 1], rmq[1][i + offset / 2][j - 1]);
			} else {
				break;
			}
		}
	}
}

int query_rmq(int left, int right)
{
	if (left > right)
		swap(left, right);
	--right;
	int e = get_lower_exp(right - left + 1);
	return min(rmq[0][right][e], rmq[1][left][e]);
}

void build_suffix_array(int n)
{
	for (int i = 0; i < n; i++)
		prefix[0][i] = seq[i] - 'a';

	int k = 0;
	for (int diff = 1; diff < n; k++, diff <<= 1) {
		for (int i = 0; i < n; i++) {
			merger[i].values.first = prefix[k][i];
			merger[i].values.second = i + diff < n ? prefix[k][i + diff] : -1;
			merger[i].position = i;
		}
		sort(merger, merger + n);
		for (int i = 0; i < n; i++) {
			const merge_entry &elem = merger[i];
			if (i > 0 && merger[i] == merger[i - 1])
				prefix[k + 1][elem.position] = prefix[k + 1][merger[i - 1].position];
			else
				prefix[k + 1][elem.position] = i;
		}
	}

	for (int i = 0; i < n; i++)
		revpos[merger[i].position] = i;

	build_rmq(k, n);
}

int solve_problem()
{
	int m, n;

	if (fgets(seq, sizeof(seq), stdin) == NULL)
		return 1;
	trim_line(seq);

	if (fgets(str, sizeof(str), stdin) == NULL)
		return 1;
	n = atoi(str);

	size_t length = strlen(seq);
	int num_elems;
	manacher(seq, length, num_elems);

	m = (int) length;
	build_suffix_array(m);

	vector< pair<int, int> > u;
	int num_palindromes = 0;
	for (int i = 1, j = 0; i < num_elems; i += 2, j++) {
		int pos = revpos[j];
		int left = 0;
		int right = pos;
		int val = (int) ((v[i] + 1) / 2);
		while (left < right) {
			int mid = left + (right - left) / 2;
			int q = query_rmq(mid, pos);
			if (q >= val)
				right = mid;
			else
				left = mid + 1;
		}
		u.push_back(make_pair(left, val));
	}
	mergesort(&u[0], (int) u.size());

	num_palindromes += u[0].second;
	for (size_t i = 1; i < u.size(); i++) {
		int x = u[i - 1].first;
		int y = u[i].first;
		if (x == y) {
			num_palindromes += u[i].second - u[i - 1].second;
		} else {
			int q = query_rmq(x, y);
			if (u[i - 1].second < q)
				q = u[i - 1].second;
			if (u[i].second > q)
				num_palindromes += u[i].second - q;
		}
	}

	u.clear();
	for (int i = 0, j = 0; i < num_elems; i += 2, j++) {
		int val = (int) (v[i] / 2);
		if (val > 0) {
			int pos = revpos[j];
			int left = 0;
			int right = pos;
			while (left < right) {
				int mid = left + (right - left) / 2;
				int q = query_rmq(mid, pos);
				if (q >= val)
					right = mid;
				else
					left = mid + 1;
			}
			u.push_back(make_pair(left, val));
		}
	}
	mergesort(&u[0], (int) u.size());

	if (!u.empty())
		num_palindromes += u[0].second;
	for (size_t i = 1; i < u.size(); i++) {
		int x = u[i - 1].first;
		int y = u[i].first;
		if (x == y) {
			num_palindromes += u[i].second - u[i - 1].second;
		} else {
			int q = query_rmq(x, y);
			if (u[i - 1].second < q)
				q = u[i - 1].second;
			if (u[i].second > q)
				num_palindromes += u[i].second - q;
		}
	}

	vector< pair<int, int> > factors;
	compute_factors(n, factors);

	vector<int> divisors;
	divisors.push_back(1);
	for (size_t j = 0; j < factors.size(); j++) {
		size_t sz = divisors.size();
		int x = factors[j].first;
		for (int l = 1; l <= factors[j].second; l++) {
			for (size_t k = 0; k < sz; k++)
				divisors.push_back(divisors[k] * x);
			x *= factors[j].first;
		}
	}
	sort(divisors.begin(), divisors.end());

	biguint s = 0;
	biguint base((unsigned int) num_palindromes);
	for (size_t i = 0; i < divisors.size(); i++) {
		int d = divisors[i];
		int phi = d;
		for (size_t j = 0; j < factors.size(); j++) {
			int p = factors[j].first;
			if (d % p == 0)
				phi = (phi / p) * (p - 1);
		}
		s += base.pow(n / d) * (unsigned int) phi;
	}
	s /= (unsigned int) n;

	if (n % 2) {
		s += base.pow((n + 1) / 2);
	} else {
		biguint t = base.pow(n / 2) * (base + 1);
		t /= 2;
		s += t;
	}
	s /= 2;
	s.write_to_file(stdout);

	return 0;
}

int main()
{
	build_primes(MAX_PRIME);
	solve_problem();
	return 0;
}

/**
 * Actual recursive merge sort function.
 *
 * The algorithm goes as follows: we split the vector in half and then we sort
 * the two halves separately; the final step consists of merging the sorted
 * halves into one big sorted array; as a minor optimization, when the size of
 * the array becomes too small, we switch to the useful insertion sort.
 */
void __mergesort(element_type *v, element_type *tmp, int lo, int hi)
{
	int mid;

	if (hi - lo < INSERTION_THRESHOLD) {
		insertion_sort(v + lo, hi - lo + 1);
		return;
	}

	mid = lo + ((hi - lo) >> 1);

	__mergesort(v, tmp, lo, mid);
	__mergesort(v, tmp, mid + 1, hi);

	__merge(v, tmp, lo, mid, hi);
}

void __merge(element_type *v, element_type *tmp,
	int lo, int mid, int hi)
{
	int length;
	int i, j, k;

	length = mid - lo + 1;

	/*
	 * Equivalent with:
	 *     for(i = 0; i < length; i++)
	 *         tmp[i] = v[i + lo];
	 */
	memcpy(tmp, v + lo, ((size_t) length) * sizeof(element_type));

	i = 0;
	j = mid + 1;

	while (i < length && !(v[j] < tmp[i]))
		i++;

	k = lo + i;

	while (i < length && j <= hi) {
		if (tmp[i] < v[j])
			v[k++] = tmp[i++];
		else
			v[k++] = v[j++];
	}

	/*
	 * Equivalent with:
	 *     while(i < length) {
	 *         v[k] = tmp[i];
	 *         i++;
	 *         k++;
	 *     }
	 */
	if (j > hi)
		memcpy(v + k, tmp + i, ((size_t) (length - i)) * sizeof(element_type));
}
