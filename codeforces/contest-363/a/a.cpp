#include <cstdio>

#define BUFFER_SIZE 8

using namespace std;

char str[BUFFER_SIZE + 1];

void print_digit(int m)
{
	if (m >= 5) {
		str[0] = '-';
		str[1] = 'O';
		m -= 5;
	} else {
		str[0] = 'O';
		str[1] = '-';
	}
	for (int i = 0; i < 5; i++)
		str[3 + i] = (i != m) ? 'O' : '-';
	printf("%s\n", str);
}

int solve_problem()
{
	int n;

	if (scanf("%d", &n) != 1)
		return 1;

	str[2] = '|';
	str[BUFFER_SIZE] = '\0';
	do {
		int m = n % 10;
		print_digit(m);
		n /= 10;
	} while (n > 0);

	return 0;
}

int main()
{
	solve_problem();
	return 0;
}
