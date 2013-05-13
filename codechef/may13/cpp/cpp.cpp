#include <algorithm>
#include <string>
#include <sstream>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <regex.h>

#define FORMAT_SIZE 1000
#define BUFFER_SIZE 2000

enum token_type {
	ID, TO, OR, OPTIONAL, TIMES, DIGIT, LETTER, EXACTLY,
	UPTO, LETTERS, DIGITS, LPAR, RPAR, INVALID,
};

using namespace std;

char format[FORMAT_SIZE + 10];
char str[BUFFER_SIZE + 10];

vector<string> tokens;
token_type symbols[FORMAT_SIZE];
size_t position;
bool invalid_syntax;

struct simple_node {

	virtual ~simple_node() { }

};

struct parser_node : public simple_node {

	virtual void build_regex_string(ostringstream &os) const = 0;

	virtual size_t compute_longest_match() const
	{
		return 0;
	}

};

struct repeatable_node : public parser_node {

	token_type tt;
	vector<parser_node *> sub_expressions;
	string id;
	string other_id;
	int repetitions;
	bool is_alpha;

	repeatable_node(token_type tt) : tt(tt), repetitions(0), is_alpha(false) { }

	virtual ~repeatable_node()
	{
		for (vector<parser_node *>::iterator it = sub_expressions.begin();
				it != sub_expressions.end(); ++it) {
			delete *it;
		}
	}

	virtual void build_regex_string(ostringstream &os) const
	{
		if (tt == ID) {
			if (other_id.empty()) {
				os << id;
			} else {
				os << '[' << id << '-' << other_id << ']';
			}
		} else if (tt == LPAR) {
			os << '(';
			for (size_t i = 0; i < sub_expressions.size(); i++)
				sub_expressions[i]->build_regex_string(os);
			os << ')';
		} else if (tt == UPTO) {
			const char *s = is_alpha ? "[[:upper:]]" : "[[:digit:]]";
			if (repetitions > 0)
				os << s << "{0," << repetitions << '}';
		} else if (tt == EXACTLY) {
			const char *s = is_alpha ? "[[:upper:]]" : "[[:digit:]]";
			if (repetitions > 0)
				os << s << '{' << repetitions << '}';
		} else if (tt == LETTER) {
			const char *s = "[[:upper:]]";
			os << s;
		} else if (tt == DIGIT) {
			const char *s = "[[:digit:]]";
			os << s;
		}
	}

	virtual size_t compute_longest_match() const
	{
		if (tt == ID) {
			if (other_id.empty())
				return id.size();
			else
				return 1;
		} else if (tt == LPAR) {
			size_t result = 0;
			for (size_t i = 0; i < sub_expressions.size(); i++) {
				result += sub_expressions[i]->compute_longest_match();
				if (result > BUFFER_SIZE)
					result = BUFFER_SIZE + 1;
			}
			return result;
		} else if (tt == UPTO) {
			return (size_t) repetitions;
		} else if (tt == EXACTLY) {
			return (size_t) repetitions;
		} else if (tt == LETTER) {
			return 1;
		} else if (tt == DIGIT) {
			return 1;
		} else {
			return 0;
		}
	}

};

struct quantifier_node : public simple_node {

	bool optional;
	int times;

	quantifier_node() : optional(true), times(0) { }

	quantifier_node(int times) : optional(false), times(times) { }

};

struct factor_node : public parser_node {

	parser_node *sub_factor;
	vector<quantifier_node *> quantifiers;

	factor_node(parser_node *sub_factor) : sub_factor(sub_factor) { }

	virtual ~factor_node()
	{
		for (vector<quantifier_node *>::iterator it = quantifiers.begin();
				it != quantifiers.end(); ++it) {
			delete *it;
		}
		delete sub_factor;
	}

	virtual void build_regex_string(ostringstream &os) const
	{
		bool optional = false;
		int times = 0;
		if (!quantifiers.empty()) {
			optional = quantifiers.front()->optional;
			times = quantifiers.front()->times;
		}
		if (optional || times > 0)
			os << '(';
		sub_factor->build_regex_string(os);
		if (optional || times > 0)
			os << ')';
		if (optional)
			os << '?';
		if (times > 0)
			os << '{' << times << '}';
	}

	virtual size_t compute_longest_match() const
	{
		int times = 1;
		if (!quantifiers.empty())
			times = quantifiers.front()->times;
		size_t result = ((size_t) max(times, 1)) * sub_factor->compute_longest_match();
		if (result > BUFFER_SIZE)
			result = BUFFER_SIZE + 1;
		return result;
	}

};

struct term_node : public parser_node {

	vector<factor_node *> or_factors;

	virtual ~term_node()
	{
		for (vector<factor_node *>::iterator it = or_factors.begin();
				it != or_factors.end(); ++it) {
			delete *it;
		}
	}

	virtual void build_regex_string(ostringstream &os) const
	{
		if (or_factors.size() <= 1) {
			for (size_t i = 0; i < or_factors.size(); i++)
				or_factors[i]->build_regex_string(os);
		} else {
			os << '(';
			for (size_t i = 0; i < or_factors.size(); i++) {
				if (i > 0)
					os << '|';
				or_factors[i]->build_regex_string(os);
			}
			os << ')';
		}
	}

	virtual size_t compute_longest_match() const
	{
		size_t result = 0;
		for (size_t i = 0; i < or_factors.size(); i++) {
			size_t cand = or_factors[i]->compute_longest_match();
			if (cand > result)
				result = cand;
		}
		return result;
	}

};

struct expr_node : public parser_node {

	vector<term_node *> terms;

	virtual ~expr_node()
	{
		for (vector<term_node *>::iterator it = terms.begin();
				it != terms.end(); ++it) {
			delete *it;
		}
	}

	virtual void build_regex_string(ostringstream &os) const
	{
		for (size_t i = 0; i < terms.size(); i++)
			terms[i]->build_regex_string(os);
	}

	virtual size_t compute_longest_match() const
	{
		size_t result = 0;
		for (size_t i = 0; i < terms.size(); i++) {
			size_t diff = terms[i]->compute_longest_match();
			result += diff;
			if (result > BUFFER_SIZE)
				result = BUFFER_SIZE + 1;
		}
		return result;
	}

};

long parse_count()
{
	const string &token = tokens[position];
	char *endptr;
	long count = strtol(token.c_str(), &endptr, 10);
	if (count < 2 || count > 12)
		invalid_syntax = true;
	if (*endptr)
		invalid_syntax = true;
	for (size_t i = 0; i < token.size(); i++)
		if (!isdigit((unsigned char) token[i])) {
			invalid_syntax = true;
			break;
		}
	if (token.size() > 1 && token[0] == '0')
		invalid_syntax = true;
	return count;
}

char *trim_line(char *str)
{
	return strtok(str, "\r\n");
}

token_type get_token_type(const string &s)
{
	bool id = false;
	bool keyword = false;
	bool par = false;
	for (size_t i = 0; i < s.size(); i++) {
		unsigned char ch = (unsigned char) s[i];
		if (islower(ch)) {
			keyword = true;
		} else if (isupper(ch) || isdigit(ch) || ch == '-') {
			id = true;
		} else if (ch == '(' || ch == ')') {
			par = true;
		} else {
			return INVALID;
		}
	}
	if (id) {
		if (keyword || par)
			return INVALID;
		else
			return ID;
	} else {
		if (s == "to")
			return TO;
		else if (s == "or")
			return OR;
		else if (s == "optional")
			return OPTIONAL;
		else if (s == "times")
			return TIMES;
		else if (s == "digit")
			return DIGIT;
		else if (s == "letter")
			return LETTER;
		else if (s == "exactly")
			return EXACTLY;
		else if (s == "upto")
			return UPTO;
		else if (s == "letters")
			return LETTERS;
		else if (s == "digits")
			return DIGITS;
		else if (s == "(")
			return LPAR;
		else if (s == ")")
			return RPAR;
		else
			return INVALID;
	}
}

void add_token(const char *p)
{
	size_t len = strlen(p);
	size_t start = 0;
	for (size_t i = 0; i < len; i++)
		if (p[i] == '(' || p[i] == ')') {
			if (i > start)
				tokens.push_back(string(p + start, i - start));
			tokens.push_back(string(p + i, 1));
			start = i + 1;
		}
	if (len > start)
		tokens.push_back(string(p + start, len - start));
}

token_type get_next_symbol()
{
	if (position < tokens.size())
		return symbols[position];
	else
		return INVALID;
}

token_type get_second_next_symbol()
{
	if (position + 1 < tokens.size())
		return symbols[position + 1];
	else
		return INVALID;
}

void advance_symbol()
{
	if (position < tokens.size())
		++position;
}

int accept_symbol(token_type tt)
{
	if (get_next_symbol() == tt) {
		advance_symbol();
		return 1;
	} else {
		return 0;
	}
}

int expect_symbol(token_type tt)
{
	if (accept_symbol(tt)) {
		return 1;
	} else {
		invalid_syntax = true;
		return 0;
	}
}

expr_node * expr();
term_node * term();
factor_node * factor();
repeatable_node * repeatable();
quantifier_node * quantifier();
const char *range();

expr_node * expr()
{
	expr_node *node = new expr_node;

	token_type tt = get_next_symbol();
	bool exists = false;
	while (tt == ID || tt == DIGIT || tt == LETTER ||
			tt == EXACTLY || tt == UPTO || tt == LPAR) {
		exists = true;
		node->terms.push_back(term());
		tt = get_next_symbol();
	}

	if (!exists) {
		invalid_syntax = true;
	}

	return node;
}

term_node * term()
{
	term_node *node = new term_node;

	node->or_factors.push_back(factor());
	token_type tt = get_next_symbol();
	while (tt == OR) {
		advance_symbol();
		node->or_factors.push_back(factor());
		tt = get_next_symbol();
	}

	return node;
}

factor_node * factor()
{
	factor_node *node = new factor_node(repeatable());

	quantifier_node *qnode;
	bool first = true;
	while ((qnode = quantifier()) != NULL) {
		if (first) {
			first = false;
		} else {
			node = new factor_node(node);
		}
		node->quantifiers.push_back(qnode);
	}

	return node;
}

repeatable_node * repeatable()
{
	repeatable_node *node = new repeatable_node(get_next_symbol());

	if (accept_symbol(ID)) {
		node->id = tokens[position - 1];
		const char *s = range();
		if (s != NULL) {
			bool ok = false;
			node->other_id = s;
			if (node->id.size() == 1 && node->other_id.size() == 1) {
				unsigned char lch = (unsigned char) node->id[0];
				unsigned char rch = (unsigned char) node->other_id[0];
				if (lch <= rch) {
					if (isupper(lch) && isupper(rch))
						ok = true;
					else if (isdigit(lch) && isdigit(rch))
						ok = true;
				}
			}
			if (!ok)
				invalid_syntax = true;
		}
	} else if (accept_symbol(LPAR)) {
		node->sub_expressions.push_back(expr());
		expect_symbol(RPAR);
	} else if (accept_symbol(DIGIT)) {
	} else if (accept_symbol(LETTER)) {
	} else if (accept_symbol(EXACTLY)) {
		long count = parse_count();
		node->repetitions = (int) count;
		expect_symbol(ID);
		token_type tt = get_next_symbol();
		if (tt == DIGITS || tt == LETTERS) {
			node->is_alpha = (tt == LETTERS);
			accept_symbol(tt);
		} else {
			expect_symbol(DIGITS);
		}
	} else if (accept_symbol(UPTO)) {
		long count = parse_count();
		node->repetitions = (int) count;
		expect_symbol(ID);
		token_type tt = get_next_symbol();
		if (tt == DIGITS || tt == LETTERS) {
			node->is_alpha = (tt == LETTERS);
			accept_symbol(tt);
		} else {
			expect_symbol(DIGITS);
		}
	} else {
		expect_symbol(ID);
	}

	return node;
}

const char * range()
{
	token_type tt = get_next_symbol();
	if (tt == TO) {
		accept_symbol(tt);
		const char *s = tokens[position].c_str();
		expect_symbol(ID);
		return s;
	} else {
		return NULL;
	}
}

quantifier_node * quantifier()
{
	token_type tt = get_next_symbol();
	if (tt == OPTIONAL) {
		accept_symbol(tt);
		return new quantifier_node;
	} else if (tt == ID) {
		if (get_second_next_symbol() == TIMES) {
			long count = parse_count();
			accept_symbol(tt);
			expect_symbol(TIMES);
			return new quantifier_node((int) count);
		}
	}

	return NULL;
}

int solve_problem()
{
	char *p;
	int q;

	if (fgets(format, sizeof(format), stdin) == NULL)
		return 1;
	trim_line(format);

	tokens.clear();
	position = 0;
	invalid_syntax = false;

	p = strtok(format, " ");
	while (p != NULL) {
		add_token(p);
		p = strtok(NULL, " ");
	}

	for (size_t i = 0; i < tokens.size(); i++) {
		symbols[i] = get_token_type(tokens[i]);
		if (symbols[i] == INVALID)
			invalid_syntax = true;
	}

	expr_node *root = expr();
	if (position != tokens.size())
		invalid_syntax = true;

	if (fgets(str, sizeof(str), stdin) == NULL)
		return 1;
	q = atoi(str);

	ostringstream os;
	os << '^';
	root->build_regex_string(os);
	os << '$';
	string regex_str = os.str();

	bool invalid_semantics = false;
	if (!invalid_syntax) {
		size_t longest_match = root->compute_longest_match();
		if (longest_match > BUFFER_SIZE)
			invalid_semantics = true;
	}

	if (invalid_syntax || invalid_semantics) {
		for (int i = 0; i < q; i++) {
			if (fgets(str, sizeof(str), stdin) == NULL)
				return 1;
		}
		if (invalid_syntax)
			printf("Format does not conform\n");
		else if (invalid_semantics)
			printf("Matches too long ID\n");
	} else {
		regex_t regex;
		if (regcomp(&regex, regex_str.c_str(), REG_EXTENDED) != 0) {
			fprintf(stderr, "Could not compile regex\n");
			exit(EXIT_FAILURE);
		}
		for (int i = 0; i < q; i++) {
			if (fgets(str, sizeof(str), stdin) == NULL)
				return 1;
			p = trim_line(str);
			if (!regexec(&regex, p, 0, NULL, 0)) {
				printf("Valid-ID\n");
			} else {
				printf("Invalid-ID\n");
			}
		}
		regfree(&regex);
	}
	printf("\n");

	delete root;

	return 0;
}

int main()
{
	int num_tests;

	if (fgets(str, sizeof(str), stdin) == NULL)
		return 1;
	num_tests = atoi(str);
	for (int i = 0; i < num_tests; i++) {
		int ret = solve_problem();
		if (ret != 0) {
			fprintf(stderr, "Read error at case #%d\n", i + 1);
			break;
		}
	}

	return 0;
}
