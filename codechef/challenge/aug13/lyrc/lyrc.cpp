#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#define BUFFER_SIZE 50000
#define MAX_W 500
#define MAX_N 100
#define NUM_CHARACTERS 63

using namespace std;

struct trie_node {

	vector<int> ids;
	trie_node *next[NUM_CHARACTERS];
	trie_node *suffix;
	int num_matches;

	trie_node() : suffix(NULL), num_matches(0)
	{
		memset(next, 0, sizeof(next));
	}

};

char str[BUFFER_SIZE + 10];
char word[BUFFER_SIZE + 10];

char *trim_line(char *str)
{
	return strtok(str, "\r\n");
}

int char_to_int(int c)
{
	if (c >= 'a' && c <= 'z')
		return (c - 'a');
	else if (c >= 'A' && c <= 'Z')
		return 26 + (c - 'A');
	else if (c >= '0' && c <= '9')
		return 52 + (c - '0');
	else
		return 62;
}

int int_to_char(int c)
{
	if (c < 26)
		return 'a' + c;
	else if (c < 52)
		return 'A' + (c - 26);
	else if (c < 62)
		return '0' + (c - 52);
	else
		return '-';
}

void add_word(trie_node *root, const char *word, int id)
{
	while (*word) {
		int c = char_to_int(*word);
		if (root->next[c] == NULL) {
			root->next[c] = new trie_node;
		}
		root = root->next[c];
		++word;
	}
	root->ids.push_back(id);
}

void free_trie(trie_node *root)
{
	for (int i = 0; i < NUM_CHARACTERS; i++)
		if (root->next[i] != NULL) {
			free_trie(root->next[i]);
			delete root->next[i];
			root->next[i] = NULL;
		}
}

int solve_problem()
{
	trie_node *root = new trie_node;
	int w, n;

	if (fgets(str, sizeof(str), stdin) == NULL)
		return 1;
	trim_line(str);
	w = atoi(str);

	for (int i = 0; i < w; i++) {
		if (fgets(word, sizeof(word), stdin) == NULL)
			return 1;
		trim_line(word);
		add_word(root, word, i);
	}

	vector<trie_node *> q;
	root->suffix = root;
	q.push_back(root);
	for (size_t i = 0; i < q.size(); i++) {
		for (int j = 0; j < NUM_CHARACTERS; j++) {
			trie_node *next_node = q[i]->next[j];
			if (next_node != NULL) {
				trie_node *p = q[i]->suffix;
				while (p != root && p->next[j] == NULL) {
					p = p->suffix;
				}
				if (p->next[j] != NULL && p->next[j] != next_node)
					p = p->next[j];
				next_node->suffix = p;
				q.push_back(next_node);
			}
		}
	}
	root->suffix = NULL;

	if (fgets(str, sizeof(str), stdin) == NULL)
		return 1;
	trim_line(str);
	n = atoi(str);

	for (int i = 0; i < n; i++) {
		if (fgets(str, sizeof(str), stdin) == NULL)
			return 1;
		trim_line(str);
		trie_node *node = root;
		for (const char *s = str; *s; ++s) {
			int c = char_to_int(*s);
			while (node->next[c] == NULL && node != root)
				node = node->suffix;
			if (node->next[c] != NULL)
				node = node->next[c];
			node->num_matches++;
		}
	}

	for (size_t i = q.size(); i > 0;) {
		--i;
		if (q[i]->suffix != NULL)
			q[i]->suffix->num_matches += q[i]->num_matches;
	}

	int result[MAX_W];
	for (size_t i = 0; i < q.size(); i++) {
		for (size_t j = 0; j < q[i]->ids.size(); j++)
			result[q[i]->ids[j]] = q[i]->num_matches;
	}
	for (int i = 0; i < w; i++)
		printf("%d\n", result[i]);

	free_trie(root);
	delete root;

	return 0;
}

int main()
{
	solve_problem();
	return 0;
}
