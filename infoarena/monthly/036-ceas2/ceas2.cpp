#include <fstream>
#include <string>
#include <vector>
#include <cstdio>
#include <cstdlib>

using namespace std;

void read_input(const char *filename, string &source, string &sink)
{
	ifstream f(filename);
	f >> source >> sink;
	f.close();
}

void write_output(const char *filename, bool result,
	const vector<int> &transform)
{
	FILE *f;

	f = fopen(filename, "wt");
	if (f == NULL) {
		fprintf(stderr, "Could not open file: %s\n", filename);
		exit(EXIT_FAILURE);
	}

	fprintf(f, "%s\n", result ? "DA" : "NU");
	if (result) {
		for (size_t i = 0; i < transform.size(); i++) {
			int j = transform[i];
			if (j >= 0 && ((size_t) j) != i)
				fprintf(f, "%c %c\n", (char) (i + 'a'), (char) (j + 'a'));
		}
	}
	fclose(f);
}

bool solve_problem(const string &source, const string &sink,
	vector<int> &transform)
{
	size_t len = source.size();

	if (sink.size() != len)
		return false;

	transform.resize(26, -1);

	for (size_t i = 0; i < len; i++) {
		int before = source[i] - 'a';
		int after = sink[i] - 'a';
		size_t bef_idx = (size_t) before;
		if (transform[bef_idx] < 0) {
			transform[bef_idx] = after;
		} else if (transform[bef_idx] != after) {
			return false;
		}
	}

	return true;
}

int main()
{
	string source, sink;
	vector<int> transform;
	bool result;

	read_input("ceas2.in", source, sink);
	result = solve_problem(source, sink, transform);
	write_output("ceas2.out", result, transform);

	return 0;
}
