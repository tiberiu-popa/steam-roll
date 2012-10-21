#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
#include <cstring>
#include <ctime>

//#define INFOARENA_DEBUG
#define NUM_DIGITS 8

using namespace std;

template<class S, class T, class U>
struct triple {

	S first;
	T second;
	U third;

	triple(S first, T second, U third) :
		first(first), second(second), third(third) { }

	bool operator< (triple &other) {
		if (first > other.first)
			return true;
		if (first == other.first) {
			if (second > other.second)
				return true;
			if (second == other.second)
				return third > other.third;
		}
		return false;
	}

};

struct bignum {

	unsigned int digits[NUM_DIGITS];

	bignum()
	{
		memset(digits, 0, sizeof(digits));
	}

	bignum(unsigned int value)
	{
		memset(digits + 1, 0, sizeof(digits) - sizeof(*digits));
		digits[0] = value;
	}

	bool operator== (const bignum &rhs) const
	{
		return memcmp(digits, rhs.digits, sizeof(digits)) == 0;
	}

	bool operator!= (const bignum &rhs) const
	{
		return !(*this == rhs);
	}

	bool operator< (const bignum &rhs) const
	{
		for (int i = NUM_DIGITS - 1; i >= 0; i--) {
			unsigned int left_digit = digits[i];
			unsigned int right_digit = rhs.digits[i];
			if (left_digit != right_digit)
				return left_digit < right_digit;
		}
		return false;
	}

	bignum & operator*= (unsigned int rhs)
	{
		const unsigned int mask = 0;
		unsigned int carry = 0;
		for (int i = 0; i < NUM_DIGITS; i++) {
			unsigned long long value = digits[i];
			value *= rhs;
			value += carry;
			digits[i] = value & ~mask;
			carry = value >> 32;
		}
		return *this;
	}

	const pair<bignum, unsigned int> divmod(unsigned int rhs) const
	{
		const unsigned int mask = 0;
		bignum quotient;
		unsigned int carry = 0;

		int i = NUM_DIGITS - 1;
		while (i >= 0 && digits[i] == 0)
			i--;
		for (; i >= 0; i--) {
			unsigned long long value = carry;
			value = (value << 32) | digits[i];
			unsigned long long q = value / rhs;
			quotient.digits[i] = q & ~mask;
			carry = (unsigned int) (value - q * rhs);
		}
		return pair<bignum, unsigned int>(quotient, carry);
	}

	friend ostream & operator<< (ostream &os, const bignum &number);

};

const int primes[] = {
	2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37,
	41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89,
	97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151,
	157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223,
	227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281,
	283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359,
	367, 373, 379, 383, 389, 397, 401, 409, 419, 421, 431, 433,
	439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499, 503,
	509, 521, 523, 541, 547, 557, 563, 569, 571, 577, 587, 593,
	599, 601, 607, 613, 617, 619, 631, 641, 643, 647, 653, 659,
	661, 673, 677, 683, 691, 701, 709, 719, 727, 733, 739, 743,
	751, 757, 761, 769, 773, 787, 797, 809, 811, 821, 823, 827,
	829, 839, 853, 857, 859, 863, 877, 881, 883, 887, 907, 911,
	919, 929, 937, 941, 947, 953, 967, 971, 977, 983, 991, 997,
	1009, 1013, 1019, 1021, 1031, 1033, 1039, 1049, 1051, 1061, 1063, 1069,
	1087, 1091, 1093, 1097, 1103, 1109, 1117, 1123, 1129, 1151, 1153, 1163,
	1171, 1181, 1187, 1193, 1201, 1213, 1217, 1223, 1229, 1231, 1237, 1249,
	1259, 1277, 1279, 1283, 1289, 1291, 1297, 1301, 1303, 1307, 1319, 1321,
	1327, 1361, 1367, 1373, 1381, 1399, 1409, 1423, 1427, 1429, 1433, 1439,
	1447, 1451, 1453, 1459, 1471, 1481, 1483, 1487, 1489, 1493, 1499, 1511,
	1523, 1531, 1543, 1549, 1553, 1559, 1567, 1571, 1579, 1583, 1597, 1601,
	1607, 1609, 1613, 1619, 1621, 1627, 1637, 1657, 1663, 1667, 1669, 1693,
	1697, 1699, 1709, 1721, 1723, 1733, 1741, 1747, 1753, 1759, 1777, 1783,
	1787, 1789, 1801, 1811, 1823, 1831, 1847, 1861, 1867, 1871, 1873, 1877,
	1879, 1889, 1901, 1907, 1913, 1931, 1933, 1949, 1951, 1973, 1979, 1987,
	1993, 1997, 1999, 2003, 2011, 2017, 2027, 2029, 2039, 2053, 2063, 2069,
	2081, 2083, 2087, 2089, 2099, 2111, 2113, 2129, 2131, 2137, 2141, 2143,
	2153, 2161, 2179, 2203, 2207, 2213, 2221, 2237, 2239, 2243, 2251, 2267,
	2269, 2273, 2281, 2287, 2293, 2297, 2309, 2311, 2333, 2339, 2341, 2347,
	2351, 2357, 2371, 2377, 2381, 2383, 2389, 2393, 2399, 2411, 2417, 2423,
	2437, 2441, 2447, 2459, 2467, 2473, 2477, 2503, 2521, 2531, 2539, 2543,
	2549, 2551, 2557, 2579, 2591, 2593, 2609, 2617, 2621, 2633, 2647, 2657,
	2659, 2663, 2671, 2677, 2683, 2687, 2689, 2693, 2699, 2707, 2711, 2713,
	2719, 2729, 2731, 2741, 2749, 2753, 2767, 2777, 2789, 2791, 2797, 2801,
	2803, 2819, 2833, 2837, 2843, 2851, 2857, 2861, 2879, 2887, 2897, 2903,
	2909, 2917, 2927, 2939, 2953, 2957, 2963, 2969, 2971, 2999, 3001, 3011,
	3019, 3023, 3037, 3041, 3049, 3061, 3067, 3079, 3083, 3089, 3109, 3119,
	3121, 3137, 3163,
};

ostream & operator<< (ostream &os, const bignum &number)
{
	int i = NUM_DIGITS - 1;
	while (i > 0 && number.digits[i] == 0)
		i--;
	for (; i >= 0; i--) {
		cout << number.digits[i];
		if (i > 0)
			cout << ',';
	}
	return os;
}

int read_input(const char *filename)
{
	int n;

	ifstream f(filename);
	f >> n;
	f.close();

	return n;
}

void write_output(const char *filename, const vector<int> &result, int n)
{
	ofstream f(filename);

	for (vector<int>::const_iterator it = result.begin(); it != result.end(); ++it) {
		if (it != result.begin())
			f << ' ';
		f << n * (*it);
		//f << *it;
	}
	f << endl;

	f.close();
}

int get_smallest_prime_divisor(int n)
{
	size_t num_primes = sizeof(primes) / sizeof(*primes);

	for (size_t i = 0; i < num_primes; i++) {
		int prime = primes[i];
		if (n % prime == 0)
			return prime;
	}

	return 1;
}

int get_largest_exponent(int n, int p)
{
	int m = 0;
	while (n > 0) {
		n /= p;
		m++;
	}
	return m;
}

void find_largest_lcm(int n, vector<int> &result)
{
	vector< pair<bignum, int> > a, b;
	vector< pair<bignum, int> > &prev = a, &next = b;
	size_t num_primes = sizeof(primes) / sizeof(*primes);

	prev.push_back(pair<bignum, int>(1, 0));
	int prime_sum = 0;
	for (size_t i = 0; i < num_primes; i++) {
		next.clear();
		int prime = primes[i];
		prime_sum += prime;
		if (prime_sum > 2 * n)
			break;
		int k = get_largest_exponent(n, prime);
		vector<int> powers;
		int prod = 1;
		for (int i = 0; i < k; i++) {
			powers.push_back(prod);
			prod *= prime;
		}
		vector< triple<pair<bignum, int>, size_t, int> > v;
		pair<bignum, int> primordial = prev.front();
		for (int i = 0; i < k; i++) {
			int term = powers[i];
			pair<bignum, int> adjusted = primordial;
			adjusted.first *= term;
			if (i > 0)
				adjusted.second += term;
			v.push_back(triple<pair<bignum, int>, size_t, int>(adjusted, (size_t) 0, i));
		}
		make_heap(v.begin(), v.end());
		while (!v.empty()) {
			pop_heap(v.begin(), v.end());
			triple<pair<bignum, int>, size_t, int> elem = v.back();
			v.pop_back();
			pair<bignum, int> candidate = elem.first;
			while (!next.empty()) {
				pair<bignum, int> last_pair = next.back();
				if (last_pair.second >= candidate.second)
					next.pop_back();
				else
					break;
			}
			next.push_back(candidate);
			size_t index = elem.second + 1;
			if (index < prev.size()) {
				int p_exp = elem.third;
				pair<bignum, int> next_pair = prev[index];
				if (p_exp > 0)
					next_pair.second += powers[p_exp];
				if (next_pair.second <= n) {
					next_pair.first *= powers[p_exp];
					v.push_back(triple<pair<bignum, int>, size_t, int>(next_pair, index, p_exp));
					push_heap(v.begin(), v.end());
				}
			}
		}
		swap(prev, next);
		/*
		cout << "p=" << prime << " - k=" << k << " ; " << endl;
		for (vector< pair<bignum, int> >::iterator i = prev.begin(); i != prev.end(); ++i) {
			cout << i->first << ' ' << i->second << endl;
		}
		cout << endl;
		*/
	}

	//cout << "prev size: " << prev.size() << endl;

	pair<bignum, int> &answer = prev.back();
	if (n == answer.second && answer.first == answer.second) {
		prev.pop_back();
		answer = prev.back();
	}
	bignum factor = answer.first;
	int sum = n;
	for (size_t i = 0; i < num_primes && factor != 1; i++) {
		int prod = 1;
		int prime = primes[i];
		while (true) {
			pair<bignum, unsigned int> qr = factor.divmod(prime);
			if (qr.second == 0) {
				prod *= prime;
				factor = qr.first;
			} else {
				if (prod > 1) {
					sum -= prod;
					result.push_back(prod);
				}
				break;
			}
		}
	}
	while (sum > 0) {
		result.push_back(1);
		sum--;
	}
	//sort(result.begin(), result.end());
}

int main()
{
#ifdef INFOARENA_DEBUG
	long ce, cs = clock();
#endif
	vector<int> result;
	int n, p;

	n = read_input("nummst.in");
	p = get_smallest_prime_divisor(n);
	find_largest_lcm(p, result);
	write_output("nummst.out", result, n / p);

#ifdef INFOARENA_DEBUG
	ce = clock();
	cout << "time: " << (double) (ce - cs) / CLOCKS_PER_SEC << endl;
#endif

	return 0;
}
