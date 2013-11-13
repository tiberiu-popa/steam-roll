#include <limits>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#define BUFFER_SIZE 1000008

using namespace std;

char str[BUFFER_SIZE + 10];

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

long long ll_mul(long long x, long long y)
{
	return x * y;
}

int solve_problem()
{
	vector< pair<int, size_t> > decr;
	vector< pair<int, size_t> > incr;
	long long result = 0;
	int k;

	if (fgets(str, sizeof(str), stdin) == NULL)
		return 1;

	char *p = strtok(str, " \r\n");
	k = atoi(p);
	p = strtok(NULL, " \r\n");

	size_t num_chars = strlen(p);
	size_t idx = 0;
	int max_decr_sum = 0;
	int s = 0;
	decr.push_back(make_pair(s, numeric_limits<size_t>::max()));
	for (size_t i = 0; i < num_chars; i++) {
		if (p[i] == '0') {
			s -= 2;
			while (idx < decr.size() && s < decr[idx].first)
				++idx;
			if (idx == decr.size())
				decr.push_back(make_pair(s, i));
		} else {
			s += 3;
			while (idx > 0 && s >= decr[idx - 1].first)
				--idx;
			if (s > max_decr_sum)
				max_decr_sum = s;
		}
		long long cand = (long long) (i - decr[idx].second);
		if (cand > result)
			result = cand;
	}

	s = 0;
	max_decr_sum = 0;
	decr.clear();
	for (size_t i = 0; i < num_chars; i++) {
		if (p[i] == '0') {
			s -= 2;
		} else {
			s += 3;
		}
		if (s > max_decr_sum)
			max_decr_sum = s;
		while (!decr.empty() && s >= decr.back().first)
			decr.pop_back();
		decr.push_back(make_pair(s, i));
	}

	int max_incr_sum = 0;
	s = 0;
	for (size_t i = num_chars; i > 0;) {
		--i;
		if (p[i] == '0') {
			s -= 2;
		} else {
			s += 3;
		}
		if (s > max_incr_sum)
			max_incr_sum = s;
		while (!incr.empty() && s >= incr.back().first)
			incr.pop_back();
		incr.push_back(make_pair(s, i));
	}

	if (s >= 0) {
		result = ll_mul(k, (long long) num_chars);
	} else {
		if (k >= 2) {
			long long mt = min(k - 2, (max_decr_sum + max_incr_sum) / -s);
			for (long long t = mt; t >= mt - 1 && t >= 0; t--) {
				long long cand = 0;
				long long score = t * -s;
				size_t j = decr.size();
				for (size_t i = 0; i < incr.size(); i++) {
					while (j > 0 && incr[i].first + decr[j - 1].first < score)
						--j;
					if (j == 0)
						break;
					long long val = (long long) (num_chars - incr[i].second + decr[j - 1].second + 1);
					if (val > cand)
						cand = val;
				}
				cand += ((long long) num_chars) * t;
				if (cand > result)
					result = cand;
			}
		}
	}
	printf("%lld\n", result);

	return 0;
}

int main()
{
	solve_problem();
	return 0;
}
