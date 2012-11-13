#include <fstream>
#include <string>
#include <vector>

using namespace std;

template<class T>
void print_vector(ostream &os, const vector<T> &v, int limit)
{
	int k = 0;
	for (typename vector<T>::const_iterator it = v.begin();
			k < limit && it != v.end(); ++it, ++k) {
		if (it != v.begin())
			os << ' ';
		os << *it;
	}
	os << endl;
}

void read_input(const char *filename, string &text, string &pattern)
{
	ifstream f(filename);
	f >> pattern >> text;
	f.close();
}

void write_output(const char *filename, const vector<int> &result)
{
	ofstream f(filename);

	f << result.size() << endl;
	print_vector(f, result, 1000);

	f.close();
}

size_t advance_char(const vector<size_t> &prefix, const string &pattern,
	size_t k, char ch)
{
	size_t pattern_size = pattern.size();

	while (k >= 1 && (k == pattern_size || pattern[k] != ch))
		k = prefix[k - 1];
	if (k < pattern_size && pattern[k] == ch)
		k++;

	return k;
}

void compute_prefix(const string &pattern, vector<size_t> &prefix)
{
	size_t k = 0;
	prefix.push_back(k);
	for (size_t i = 1; i < pattern.size(); i++) {
		k = advance_char(prefix, pattern, k, pattern[i]);
		prefix.push_back(k);
	}
}

void solve_problem(const string &text, const string &pattern,
	vector<int> &result)
{
	vector<size_t> prefix;

	compute_prefix(pattern, prefix);
	for (size_t i = 0, k = 0; i < text.size(); i++) {
		k = advance_char(prefix, pattern, k, text[i]);
		if (k == pattern.size()) {
			result.push_back(i + 1 - pattern.size());
			k = prefix[k - 1];
		}
	}
}

int main()
{
	vector<int> result;
	string text, pattern;

	read_input("strmatch.in", text, pattern);
	solve_problem(text, pattern, result);
	write_output("strmatch.out", result);

	return 0;
}
