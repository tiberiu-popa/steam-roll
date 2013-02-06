#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_M 50
#define MAX_LENGTH 101

int s[MAX_LENGTH][MAX_LENGTH];

void generate_tokens(const char *line, int *tokens, int *p_num_tokens)
{
	size_t i, len;
	int num_tokens;

	len = strlen(line);
	num_tokens = 0;
	for (i = 0; i < len; i++)
		if (line[i] == '(') {
			if (i > 0 && line[i - 1] == ':')
				tokens[num_tokens++] = 2;
			else
				tokens[num_tokens++] = 0;
		} else if (line[i] == ')') {
			if (i > 0 && line[i - 1] == ':')
				tokens[num_tokens++] = 3;
			else
				tokens[num_tokens++] = 1;
		}
	*p_num_tokens = num_tokens;
}

int main()
{
	int tokens[MAX_LENGTH];
	char line[MAX_LENGTH + 1];
	int m, i, j, k, l;
	int left, right;
	int num_cases = 0;
	int num_tokens;

	if (fgets(line, sizeof(line), stdin) == NULL)
		return 1;

	m = atoi(line);

	while (fgets(line, sizeof(line), stdin) != NULL && num_cases < m) {
		++num_cases;
		generate_tokens(line, tokens, &num_tokens);
		for (i = 0; i < num_tokens; i++)
			for (j = 0; j < num_tokens; j++)
				s[i][j] = 0;
		for (k = 0; k < num_tokens; k++)
			for (i = 0; i + k < num_tokens; i++) {
				j = i + k;
				if (k == 0) {
					s[i][j] = (tokens[i] >= 2);
				} else {
					left = tokens[i] & 1;
					right = tokens[j] & 1;
					if (left == 0 && right == 1 && (k == 1 || s[i + 1][j - 1])) {
						s[i][j] = 1;
					} else {
						for (l = i; l < j; l++)
							if (s[i][l] && s[l + 1][j]) {
								s[i][j] = 1;
								break;
							}
					}
				}
			}
		printf("Case #%d: %s\n", num_cases, s[0][num_tokens - 1] ? "YES" : "NO");
	}

	return 0;
}
