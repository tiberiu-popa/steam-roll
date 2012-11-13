#include <fstream>
#include <string>
#include <stack>
#include <vector>
#include <cctype>

using namespace std;

void read_input(const char *filename, string &text)
{
	ifstream f(filename);
	f >> text;
	f.close();
}

void write_output(const char *filename, int result)
{
	ofstream f(filename);
	f << result << endl;
	f.close();
}

void apply_op(char op, stack<int> &output)
{
	int y = output.top();
	output.pop();
	int x = output.top();
	output.pop();

	switch (op) {
	case '+':
		output.push(x + y);
		break;

	case '-':
		output.push(x - y);
		break;

	case '*':
		output.push(x * y);
		break;

	case '/':
		output.push(x / y);
		break;

	default:
		break;
	}
}

void solve_problem(const string &text, int &result)
{
	stack<unsigned char> middle;
	stack<int> output;

	for (size_t i = 0; i < text.size(); i++) {
		unsigned char ch = (unsigned char) text[i];
		if (isdigit(ch)) {
			int x = ch - '0';
			for (;;) {
				ch = (unsigned char) text[i + 1];
				if (isdigit(ch)) {
					x = 10 * x + (ch - '0');
					i++;
				} else {
					break;
				}
			}
			output.push(x);
		} else if (ch == '(') {
			middle.push(ch);
		} else if (ch == ')') {
			for (;;) {
				char op = middle.top();
				middle.pop();
				if (op == '(')
					break;
				apply_op(op, output);
			}
		} else {
			if (ch == '*' || ch == '/') {
				while (!middle.empty()) {
					char op = middle.top();
					if (op == '*' || op == '/') {
						middle.pop();
						apply_op(op, output);
					} else {
						break;
					}
				}
			} else {
				while (!middle.empty()) {
					char op = middle.top();
					if (op != '(' && op != ')') {
						middle.pop();
						apply_op(op, output);
					} else {
						break;
					}
				}
			}
			middle.push(ch);
		}
	}
	while (!middle.empty()) {
		char op = middle.top();
		middle.pop();
		apply_op(op, output);
	}

	result = output.top();
}

int main()
{
	int result;
	string text;

	read_input("evaluare.in", text);
	solve_problem(text, result);
	write_output("evaluare.out", result);

	return 0;
}
