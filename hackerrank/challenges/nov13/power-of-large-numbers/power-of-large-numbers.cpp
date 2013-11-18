#include <algorithm>
#include <limits>
#include <vector>
#include <cstdio>
#include <cstring>

#define BUFFER_SIZE 200001
#define MOD_PRIME 1000000007

using namespace std;

const unsigned long long uint_limit = 1000000000;

int addmod(int x, int y, int p)
{
	long long sum = x;
	sum += y;
	if (sum >= p)
		sum -= p;
	return (int) sum;
}

int submod(int x, int y, int p)
{
	long long diff = x;
	diff -= y;
	if (diff < 0)
		diff += p;
	return (int) diff;
}

int mulmod(int x, int y, int p)
{
	long long prod = x;
	prod = (prod * y) % p;
	return (int) prod;
}

unsigned int muladdmod(unsigned int x, unsigned int y, unsigned int z, unsigned int p)
{
	unsigned long long prod = x;
	prod = (prod * y + z) % p;
	return (unsigned int) prod;
}

unsigned int shiftaddmod(unsigned int x, int shift, unsigned int y, unsigned int p)
{
	unsigned long long prod = x;
	prod = ((prod << shift) + y) % p;
	return (unsigned int) prod;
}

int sqrmod(int x, int p)
{
	return mulmod(x, x, p);
}

int powmod(int x, int e, int p)
{
	long long result = 1;
	long long base = x;

	if (e > 0) {
		while (true) {
			if (e & 1) {
				result = (result * base) % p;
			}
			e >>= 1;
			if (e <= 0)
				break;
			base = (base * base) % p;
		}
	}

	return (int) result;
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
				result.v[i + j] = sum;
			}
			if (carry)
				result.v[i + rhs.v.size()] = carry;
		}
		if (result.v.back() == 0)
			result.v.resize(result.v.size() - 1);

		return result;
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

};

char str[BUFFER_SIZE + 10];

char *trim_line(char *str)
{
	return strtok(str, "\r\n");
}

int solve_problem()
{
	biguint a, b;
	char *p;

	if (fgets(str, sizeof(str), stdin) == NULL)
		return 1;

	trim_line(str);

	p = strtok(str, " ");
	a.read_directly_from_string(p);

	p = strtok(NULL, " ");
	b.read_directly_from_string(p);

	int result = 0;
	unsigned int x = a % MOD_PRIME;
	if (x != 0) {
		unsigned int y = b % (MOD_PRIME - 1);
		result = powmod((int) x, (int) y, MOD_PRIME);
	}

	printf("%d\n", result);

	return 0;
}

int main()
{
	int num_tests;

	if (fgets(str, sizeof(str), stdin) == NULL)
		return 1;
	num_tests = atoi(str);

	for (int i = 0; i < num_tests; i++)
		solve_problem();

	return 0;
}
