#include <iostream>
#include <deque>
#include <string>

using namespace std;

#define TYPE_PAREN 1
#define TYPE_OPERATOR 2
#define TYPE_NUMBER 3

class Token {
public:
	Token(int t, int v, int p) : type(t), value(v), priority(p) {}
	int type;
	int value;
	int priority;

	friend ostream& operator<<(ostream& out, const Token& obj) {
		if (obj.type == TYPE_PAREN)
			out << "token(" << obj.type << ",'" << (char)obj.value << "')";
		else if (obj.type == TYPE_OPERATOR)
			out << "token(" << obj.type << ",'" << (char)obj.value << "')";
		else if (obj.type == TYPE_NUMBER)
			out << "token(" << obj.type << "," << obj.value << ")";
		return out;
	}
};

deque<Token *> tokenDeque;
deque<double> numberDeque;
deque<Token *> operatorDeque;

int parse(char *s) {
	Token *token;
	int type = 0, priority = 0;

	while (*s) {
		int value = 0;
		if (*s == '(') { type = TYPE_PAREN; value = '('; priority = 5; s++; }
		else if (*s == ')') { type = TYPE_PAREN; value = ')';  priority = 5; s++; }
		else if (*s == '+') { type = TYPE_OPERATOR; value = '+'; priority = 4; s++; }
		else if (*s == '-') { type = TYPE_OPERATOR; value = '-'; priority = 4; s++; }
		else if (*s == '*') { type = TYPE_OPERATOR; value = '*'; priority = 3; s++; }
		else if (*s == '/') { type = TYPE_OPERATOR; value = '/'; priority = 3; s++; }
		else if (isdigit(*s)) {
			type = TYPE_NUMBER;
			priority = 0;
			while (*s && isdigit(*s))
				value = value * 10 + *s++ - '0';
		}

		if (type == 0) {
			cout << "wrong expression!" << endl;
			return -1;
		}
		token = new Token(type, value, priority);
		tokenDeque.push_back(token);
	}
	return 0;
}
double cal(Token *token)
{
	int result = 0;
	int num[2];

	for (int i = 0; i < 2; ++i)
	{
		num[i] = numberDeque.back();
		numberDeque.pop_back();
	}

	switch (token->value)
	{
	case '+':
		result = num[0] + num[1];
		break;
	case '-':
		result = num[1] - num[0];
		break;
	case '*':
		result = num[0] * num[1];
		break;
	case '/':
		result = num[1] / num[0];
		break;
	}

	numberDeque.push_back(result);

	return result;

}
double Calculate(deque<Token *> tokenDeque)
{
	Token* token;
	Token *toptoken;
	double number[2] = { 0,0 };

	while (!tokenDeque.empty())
	{
		token = tokenDeque.front();
		tokenDeque.pop_front();
		switch (token->type) {
		case TYPE_NUMBER:
			numberDeque.push_back(token->value);
			break;
		case TYPE_OPERATOR:
			if (operatorDeque.empty())
				operatorDeque.push_back(token);
			else
			{
				toptoken = operatorDeque.back();
				while (toptoken->priority <= token->priority)
				{
					cal(toptoken);
					operatorDeque.pop_back();
					if (operatorDeque.empty()) break;
					toptoken = operatorDeque.back();
				}
				operatorDeque.push_back(token);
			}
			break;
		case TYPE_PAREN:

			if (token->value == '(')
				operatorDeque.push_back(token);
			if (token->value == ')')
			{
				while (!operatorDeque.empty() && operatorDeque.back()->value != '(')
				{
					token = operatorDeque.back();
					operatorDeque.pop_back();
					cal(token);
				}
				operatorDeque.pop_back();
			}
			break;
		}
	}

	while (!operatorDeque.empty())
	{
		token = operatorDeque.back();
		operatorDeque.pop_back();
		cal(token);
	}
	return numberDeque.back();
}
void print_tokens(void) {
	Token *p;

	for (p = tokenDeque.front(); !tokenDeque.empty(); p = tokenDeque.front()) {
		cout << *p << endl;
		tokenDeque.pop_front();
	}
}
int main(int argc, char *argv[])
{
	istream& input_stream = cin;
	string input_expr;
	double result = 0;

	cout << "Type any arithmetic express with no spaces such as (l00+200)*300-400/100" << endl;
	while (true) {
		cout << "> ";
		if (!getline(input_stream, input_expr))
			break;

		if (parse((char *)input_expr.c_str()) < 0)
			continue;

		result = Calculate(tokenDeque);

		printf("%f\n", result);

		tokenDeque.clear();
	}
	return EXIT_SUCCESS;
}

