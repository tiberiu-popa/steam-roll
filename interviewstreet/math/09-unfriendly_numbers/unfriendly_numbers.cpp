#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <vector>

using namespace std;

template<class T>
void print_vector(const vector<T> &v)
{
	for (typename vector<T>::const_iterator it = v.begin(); it != v.end(); ++it) {
		if (it != v.begin())
			cout << ' ';
		cout << *it;
	}
	cout << endl;
}

template<class T>
void print_set(const set<T> &v)
{
	for (typename set<T>::const_iterator it = v.begin(); it != v.end(); ++it) {
		if (it != v.begin())
			cout << ' ';
		cout << *it;
	}
	cout << endl;
}

template<class T>
void print_set(const char *title, const set<T> &v)
{
	cout << title << ": ";
	print_set(v);
}

template<class T, class S>
void print_map(const map<T, S> &v)
{
	for (typename map<T, S>::const_iterator it = v.begin(); it != v.end(); ++it)
		cout << it->first << ' ' << it->second << endl;
}

long long gcd(long long x, long long y)
{
	if (x < 0)
		x = -x;
	if (y < 0)
		y = -y;
	if (x < y)
		swap(x, y);
	while (y != 0) {
		long long r = x % y;
		x = y;
		y = r;
	}
	return x;
}

void build_primes(vector<int> &primes, long long limit)
{
	primes.push_back(2);
	for (int i = 3; (long long) i * i <= limit; i += 2) {
		bool is_prime = true;
		for (vector<int>::iterator it = primes.begin(); it != primes.end(); ++it) {
			int p = *it;
			if (p * p > i)
				break;
			if (i % p == 0) {
				is_prime = false;
				break;
			}
		}
		if (is_prime)
			primes.push_back(i);
	}
	//print_vector(primes);
}

void solve_problem()
{
	set<long long> unfriendly;
	vector<int> primes;
	map<long long, int> divisors;
	set<long long> all_divisors;
	long long k;
	int n;

	cin >> n >> k;

	for (int i = 0; i < n; i++) {
		long long x;
		cin >> x;
		unfriendly.insert(gcd(x, k));
	}
	//print_set("unfriendly", unfriendly);

	build_primes(primes, k);
	long long m = k;
	for (vector<int>::iterator it = primes.begin(); it != primes.end(); ++it) {
		int p = *it;
		if (p * p > m)
			break;
		int t = 0;
		while (m % p == 0) {
			m /= p;
			t++;
		}
		if (t > 0)
			divisors[p] = t;
	}
	if (m > 1)
		divisors[m] = 1;
	//print_map(divisors);

	all_divisors.insert(1);
	for (map<long long, int>::iterator it = divisors.begin(); it != divisors.end(); ++it) {
		long long p = it->first;
		int t = it->second;
		set<long long> new_divisors;
		long long x = 1;
		for (int i = 0; i < t; i++) {
			x *= p;
			for (set<long long>::iterator old_it = all_divisors.begin(); old_it != all_divisors.end(); ++old_it)
				new_divisors.insert(x * (*old_it));
		}
		all_divisors.insert(new_divisors.begin(), new_divisors.end());
	}
	//print_set("all_divisors", all_divisors);

	int answer = 0;
	for (set<long long>::iterator it = all_divisors.begin(); it != all_divisors.end(); ++it) {
		long long p = *it;
		bool cool = true;
		for (set<long long>::iterator u_it = unfriendly.begin(); u_it != unfriendly.end(); ++u_it)
			if ((*u_it) % p == 0) {
				cool = false;
				break;
			}
		if (cool)
			answer++;
	}
	cout << answer << endl;
}

int main()
{
	solve_problem();
	return 0;
}
