#include <iostream>
#include <limits>
#include <map>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#define BUFFER_SIZE 4000
#define MAX_N 4000

using namespace std;

long long s[MAX_N + 1];
int v[MAX_N];
char str[BUFFER_SIZE + 10];

char *trim_line(char *str)
{
	return strtok(str, "\r\n");
}

int solve_problem()
{
	map<int, long long> numbers;
	int a;

	if (fgets(str, sizeof(str), stdin) == NULL)
		return 1;
	a = atoi(str);

	if (fgets(str, sizeof(str), stdin) == NULL)
		return 1;

	char *p = trim_line(str);
	size_t n = strlen(str);
	for (size_t i = 0; i < n; i++)
		v[i] = p[i] - '0';

	s[0] = 0;
	for (size_t i = 0; i < n; i++)
		s[i + 1] = s[i] + v[i];

	for (size_t i = 0; i < n; i++)
		for (size_t j = i + 1; j <= n; j++)
			++numbers[s[j] - s[i]];

	long long result = 0;
	if (a == 0) {
		long long total = 0;
		for (map<int, long long>::iterator it = numbers.begin(); it != numbers.end(); ++it)
			total += it->second;
		long long other = total - numbers[0];
		result = total * total - other * other;
	} else {
		numbers.erase(0);
		map<int, long long>::reverse_iterator r_it = numbers.rbegin();
		for (map<int, long long>::iterator it = numbers.begin(); it != numbers.end(); ++it) {
			while (r_it != numbers.rend() && it->first * r_it->first > a)
				++r_it;
			if (r_it == numbers.rend())
				break;
			if (it->first * r_it->first == a)
				result += it->second * r_it->second;
		}
	}
	cout << result << endl;

	return 0;
}

int main()
{
	solve_problem();
	return 0;
}
