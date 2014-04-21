#include <algorithm>
#include <map>
#include <string>
#include <vector>
#include <cmath>
#include <cstdio>
#include <cstring>

#define BUFFER_SIZE 10000
#define MAX_N 100
#define DELIMITERS " ,.;:'\"()[]{}-"

using namespace std;

char str[BUFFER_SIZE + 10];
map<string, size_t> word_index;
vector<string> slist[2][MAX_N];
vector<int> v[2][MAX_N];
vector<double> score[2][MAX_N];
double nr[2][MAX_N];
int result[MAX_N];
bool used[MAX_N];

char *trim_line(char *str)
{
	return strtok(str, "\r\n");
}

void add_to_word_index(const char *s, vector<string> &slist)
{
	string os(s);
	if (word_index.find(os) == word_index.end()) {
		size_t sz = word_index.size();
		word_index[os] = sz;
	}
	slist.push_back(os);
}

void parse_string(char *str, vector<string> &slist)
{
	char *p = strtok(str, DELIMITERS);
	while (p != NULL) {
		size_t len = strlen(p);
		for (size_t i = 0; i < len; i++)
			p[i] = tolower(p[i]);
		add_to_word_index(p, slist);
		p = strtok(NULL, DELIMITERS);
	}
}

template<typename T>
T sqr(T n)
{
	return n * n;
}

int solve_problem()
{
	int n;

	if (fgets(str, sizeof(str), stdin) == NULL)
		return 1;
	n = atoi(str);

	for (int i = 0; i < n; i++) {
		if (fgets(str, sizeof(str), stdin) == NULL)
			return 1;
		trim_line(str);
		parse_string(str, slist[0][i]);
	}

	if (fgets(str, sizeof(str), stdin) == NULL)
		return 1;

	for (int i = 0; i < n; i++) {
		if (fgets(str, sizeof(str), stdin) == NULL)
			return 1;
		trim_line(str);
		parse_string(str, slist[1][i]);
	}

	for (int i = 0; i < 2; i++)
		for (int j = 0; j < n; j++) {
			v[i][j].resize(word_index.size());
			for (vector<string>::iterator it = slist[i][j].begin(); it != slist[i][j].end(); ++it)
				++v[i][j][word_index[*it]];
		}

	vector<double> idf;
	const double c_lg = log(2 * n);
	for (size_t k = 0; k < word_index.size(); k++) {
		int num_elems = 0;
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < n; j++)
				num_elems += (v[i][j][k] != 0);
		idf.push_back(c_lg - log(num_elems));
	}

	for (int i = 0; i < 2; i++)
		for (int j = 0; j < n; j++)
			for (size_t k = 0; k < v[i][j].size(); k++)
				score[i][j].push_back(v[i][j][k] * idf[k]);

	for (int i = 0; i < 2; i++)
		for (int j = 0; j < n; j++) {
			double norm = 0;
			for (size_t k = 0; k < score[i][j].size(); k++)
				norm += sqr(score[i][j][k]);
			nr[i][j] = sqrt(norm);
		}

	vector< pair< double, pair<int, int> > > u;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			int scalar = 0;
			for (size_t k = 0; k < score[1][j].size(); k++)
				scalar += score[0][i][k] * score[1][j][k];
			double val = abs(scalar) / (nr[0][i] * nr[1][j]);
			u.push_back(make_pair(val, make_pair(i, j)));
		}
	}

	sort(u.begin(), u.end());
	for (size_t k = u.size(); k > 0;) {
		--k;
		int i = u[k].second.first;
		int j = u[k].second.second;
		if (result[i] == 0 && !used[j]) {
			result[i] = j;
			used[j] = true;
		}
	}

	for (int i = 0; i < n; i++)
		printf("%d\n", 1 + result[i]);

	return 0;
}

int main()
{
	solve_problem();
	return 0;
}
