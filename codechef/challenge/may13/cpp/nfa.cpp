#include <algorithm>
#include <limits>
#include <stack>
#include <string>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#define FORMAT_SIZE 1000
#define BUFFER_SIZE 2000
#define NUM_STATES (1 << 13)

enum token_type {
	ID, TO, OR, OPTIONAL, TIMES, DIGIT, LETTER, EXACTLY,
	UPTO, LETTERS, DIGITS, LPAR, RPAR, INVALID,
};

enum regop_type {
	REGOP_AND, REGOP_OR, REGOP_OPTIONAL, REGOP_UPPER,
	REGOP_DIGIT, REGOP_CHAR, REGOP_RANGE, REGOP_MATCH,
};

struct regex_op {

	regex_op(regop_type rt) : rt(rt), inf_ch(0), sup_ch(0) { }

	regex_op(regop_type rt, char ch) : rt(rt), inf_ch(ch), sup_ch(0) { }

	regex_op(regop_type rt, char inf_ch, char sup_ch)
		: rt(rt), inf_ch(inf_ch), sup_ch(sup_ch) { }

	regop_type rt;
	char inf_ch, sup_ch;

};

using namespace std;

char format[FORMAT_SIZE + 10];
char str[BUFFER_SIZE + 10];

vector<string> tokens;
token_type symbols[FORMAT_SIZE];
size_t position;
bool invalid_syntax;

template <class T>
struct sparse_set {

	vector<T> elements;
	size_t positions[NUM_STATES];

	bool contains(T elem) const
	{
		if (elem >= NUM_STATES) {
			fprintf(stderr, "Not enough states available\n");
			exit(EXIT_FAILURE);
		}
		size_t pos = positions[elem];
		return (pos < elements.size() && elements[pos] == elem);
	}

	bool insert(T elem)
	{
		bool res = !contains(elem);
		if (res) {
			positions[elem] = elements.size();
			elements.push_back(elem);
		}
		return res;
	}

	typename vector<T>::size_type size() const
	{
		return elements.size();
	}

	bool empty() const
	{
		return elements.empty();
	}

	void clear()
	{
		elements.clear();
	}

};

struct parser_node {

	virtual ~parser_node() { }

	virtual void build_regex_ops(vector<regex_op> &ops) const = 0;

	virtual size_t compute_longest_match() const = 0;

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

	virtual void build_regex_ops(vector<regex_op> &ops) const
	{
		if (tt == ID) {
			if (other_id.empty()) {
				for (size_t i = 0; i < id.size(); i++) {
					ops.push_back(regex_op(REGOP_CHAR, id[i]));
					if (i > 0)
						ops.push_back(regex_op(REGOP_AND));
				}
			} else {
				ops.push_back(regex_op(REGOP_RANGE, id[0], other_id[0]));
			}
		} else if (tt == LPAR) {
			for (size_t i = 0; i < sub_expressions.size(); i++) {
				sub_expressions[i]->build_regex_ops(ops);
				if (i > 0)
					ops.push_back(regex_op(REGOP_AND));
			}
		} else if (tt == UPTO) {
			regop_type rt = is_alpha ? REGOP_UPPER : REGOP_DIGIT;
			for (int i = 0; i < repetitions; i++)
				ops.push_back(regex_op(rt));
			for (int i = 0; i < repetitions; i++) {
				if (i > 0)
					ops.push_back(regex_op(REGOP_AND));
				ops.push_back(regex_op(REGOP_OPTIONAL));
			}
		} else if (tt == EXACTLY) {
			regop_type rt = is_alpha ? REGOP_UPPER : REGOP_DIGIT;
			for (int i = 0; i < repetitions; i++) {
				ops.push_back(regex_op(rt));
				if (i > 0)
					ops.push_back(regex_op(REGOP_AND));
			}
		} else if (tt == LETTER) {
			ops.push_back(regex_op(REGOP_UPPER));
		} else if (tt == DIGIT) {
			ops.push_back(regex_op(REGOP_DIGIT));
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

struct quantifier_node {

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

	virtual void build_regex_ops(vector<regex_op> &ops) const
	{
		bool optional = false;
		int times = 0;
		if (!quantifiers.empty()) {
			optional = quantifiers.front()->optional;
			times = quantifiers.front()->times;
		}
		if (optional) {
			sub_factor->build_regex_ops(ops);
			ops.push_back(regex_op(REGOP_OPTIONAL));
		} else {
			if (times < 1)
				times = 1;
			for (int i = 0; i < times; i++) {
				sub_factor->build_regex_ops(ops);
				if (i > 0)
					ops.push_back(regex_op(REGOP_AND));
			}
		}
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

	virtual void build_regex_ops(vector<regex_op> &ops) const
	{
		for (size_t i = 0; i < or_factors.size(); i++) {
			or_factors[i]->build_regex_ops(ops);
			if (i > 0)
				ops.push_back(regop_type(REGOP_OR));
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

	virtual void build_regex_ops(vector<regex_op> &ops) const
	{
		for (size_t i = 0; i < terms.size(); i++) {
			terms[i]->build_regex_ops(ops);
			if (i > 0)
				ops.push_back(regop_type(REGOP_AND));
		}
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

struct state {

	regex_op rop;
	size_t next[2];

	state(regex_op rop, size_t next, size_t dangling) : rop(rop)
	{
		this->next[0] = next;
		this->next[1] = dangling;
	}

	state(regex_op rop, size_t dangling) : rop(rop)
	{
		this->next[0] = numeric_limits<size_t>::max();
		this->next[1] = dangling;
	}

	state(regex_op rop) : rop(rop)
	{
		for (int i = 0; i < 2; i++)
			this->next[i] = numeric_limits<size_t>::max();
	}

};

struct compiled_regex {

	vector<state> states;
	size_t start_index;

	compiled_regex() : start_index(0) { }

};
/*
void regex_postfix(vector<char> &q, const char *str)
{
	stack<char> operators;
	size_t len = strlen(str);

	for (size_t i = 0; i < len; i++) {
		char ch = str[i];
		if (isalnum((unsigned char) ch) || ch == '-') {
			q.push_back(ch);
		} else if (ch == '?') {
			q.push_back(ch);
			while (i + 1 < len && str[i + 1] == '?')
				i++;
			while (!operators.empty() && (operators.top() == '?' || operators.top() == '.')) {
				q.push_back(operators.top());
				operators.pop();
			}
		} else if (ch == '|') {
			while (!operators.empty()) {
				char next_ch = operators.top();
				if (next_ch != '?' && next_ch != '.' && next_ch != '|')
					break;
				q.push_back(next_ch);
				operators.pop();
			}
			operators.push(ch);
		} else if (ch == '(') {
			operators.push(ch);
		} else if (ch == ')') {
			while (!operators.empty()) {
				char next_ch = operators.top();
				operators.pop();
				if (next_ch != '(')
					q.push_back(next_ch);
				else
					break;
			}
		}
		if (str[i] != '(' && str[i] != '|' && i + 1 < len) {
			if (str[i + 1] != ')' && str[i + 1] != '?' && str[i + 1] != '|') {
				operators.push('.');
			}
		}
	}

	while (!operators.empty()) {
		q.push_back(operators.top());
		operators.pop();
	}
}
*/
void redirect_dangling(vector<state> &states, size_t p, size_t next)
{
	while (p != numeric_limits<size_t>::max()) {
		size_t next_p = states[p].next[1];
		states[p].next[1] = next;
		p = next_p;
	}
}

void add_epsilon_states(sparse_set<size_t> &next_set, const vector<state> &states, size_t index)
{
	regop_type rt = states[index].rop.rt;
	next_set.insert(index);
	if (rt == REGOP_OR || rt == REGOP_OPTIONAL) {
		for (int i = 0; i < 2; i++) {
			size_t next_index = states[index].next[i];
			if (next_index != numeric_limits<size_t>::max() && !next_set.contains(next_index))
				add_epsilon_states(next_set, states, next_index);
		}
	}
}

size_t compile_regex(vector<state> &states, const vector<regex_op> &ops)
{
	stack< pair<size_t, size_t> > state_stack;

	for (vector<regex_op>::const_iterator it = ops.begin(); it != ops.end(); ++it) {
		switch (it->rt) {
		case REGOP_AND: {
			pair<size_t, size_t> snd = state_stack.top();
			state_stack.pop();
			pair<size_t, size_t> fst = state_stack.top();
			state_stack.pop();
			redirect_dangling(states, fst.second, snd.first);
			state_stack.push(pair<size_t, size_t>(fst.first, snd.second));
			break;
		}

		case REGOP_OR: {
			pair<size_t, size_t> snd_entry = state_stack.top();
			state_stack.pop();
			pair<size_t, size_t> fst_entry = state_stack.top();
			state_stack.pop();
			size_t index = states.size();
			state_stack.push(pair<size_t, size_t>(index, fst_entry.second));
			states.push_back(state(*it, fst_entry.first, snd_entry.first));
			size_t p = fst_entry.second;
			while (states[p].next[1] != numeric_limits<size_t>::max()) {
				p = states[p].next[1];
			}
			states[p].next[1] = snd_entry.second;
			break;
		}

		case REGOP_OPTIONAL: {
			pair<size_t, size_t> entry = state_stack.top();
			state_stack.pop();
			size_t index = states.size();
			state_stack.push(pair<size_t, size_t>(index, entry.second));
			states.push_back(state(*it, entry.first, numeric_limits<size_t>::max()));
			size_t p = entry.second;
			while (states[p].next[1] != numeric_limits<size_t>::max()) {
				p = states[p].next[1];
			}
			states[p].next[1] = index;
			break;
		}

		case REGOP_UPPER:
		case REGOP_DIGIT:
		case REGOP_CHAR:
		case REGOP_RANGE: {
			size_t index = states.size();
			states.push_back(state(*it));
			state_stack.push(pair<size_t, size_t>(index, index));
			break;
		}

		default:
			break;
		}
	}

	size_t start_index = numeric_limits<size_t>::max();
	size_t match_index = states.size();
	states.push_back(state(regex_op(REGOP_MATCH)));
	while (!state_stack.empty()) {
		pair<size_t, size_t> entry = state_stack.top();
		state_stack.pop();
		start_index = entry.first;
		redirect_dangling(states, entry.second, match_index);
	}

	return start_index;
}

void build_compiled_regex(compiled_regex &cr, const vector<regex_op> &ops)
{
	cr.start_index = compile_regex(cr.states, ops);
}

bool matches_simple_regex(const regex_op &rop, int ch)
{
	switch (rop.rt) {
	case REGOP_UPPER:
		return isupper(ch);

	case REGOP_DIGIT:
		return isdigit(ch);

	case REGOP_CHAR:
		return ch == rop.inf_ch;

	case REGOP_RANGE:
		return ch >= rop.inf_ch && ch <= rop.sup_ch;

	default:
		return false;
	}
}

sparse_set<size_t> state_sets[2];

bool matches_regex(compiled_regex &cr, const char *mstr)
{
	sparse_set<size_t> *prev_set = &state_sets[0];
	sparse_set<size_t> *next_set = &state_sets[1];
	size_t len = strlen(mstr);

	prev_set->clear();
	next_set->clear();
	add_epsilon_states(*next_set, cr.states, cr.start_index);
	for (size_t i = 0; i < len; i++) {
		swap(prev_set, next_set);
		if (prev_set->empty())
			return false;
		int ch = (unsigned char) mstr[i];
		next_set->clear();
		for (size_t i = 0; i < prev_set->size(); i++) {
			size_t index = prev_set->elements[i];
			const regex_op &rop = cr.states[index].rop;
			if (matches_simple_regex(rop, ch)) {
				for (int j = 0; j < 2; j++) {
					size_t next_index = cr.states[index].next[j];
					if (next_index != numeric_limits<size_t>::max() && !next_set->contains(next_index)) {
						add_epsilon_states(*next_set, cr.states, next_index);
					}
				}
			}
		}
	}

	for (size_t i = 0; i < next_set->size(); i++)
		if (cr.states[next_set->elements[i]].rop.rt == REGOP_MATCH)
			return true;

	return false;
}

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
const char * range();

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
		vector<regex_op> ops;
		compiled_regex cr;

		root->build_regex_ops(ops);
		build_compiled_regex(cr, ops);

		for (int i = 0; i < q; i++) {
			if (fgets(str, sizeof(str), stdin) == NULL)
				return 1;
			p = trim_line(str);
			if (matches_regex(cr, p)) {
				printf("Valid-ID\n");
			} else {
				printf("Invalid-ID\n");
			}
		}
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
