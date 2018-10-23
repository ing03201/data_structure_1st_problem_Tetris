#include <iostream>
#include <deque>

using namespace std;

#define TYPE_PAREN 1
#define TYPE_OPERATOR 2
#define TYPE_NUMBER 3
#define EXIT_SUCCESS 0

class Token {
public:
  Token(int t, int v) : type(t), value(v) {}
  int type;
  int value;
  friend ostream& operator<<(ostream& out, const Token& obj){
    if (obj.type == TYPE_PAREN)
      out << "token(" << obj.type << ",'" << (char) obj.value << "')";
    else if (obj.type == TYPE_OPERATOR)
      out << "token(" << obj.type << ",'" << (char) obj.value << "')";
    else if (obj.type == TYPE_NUMBER)
      out << "token(" << obj.type << "," << obj.value << ")";
    return out;
  }
};

deque<Token *> tokenDeque;

int parse(char *s) {
  Token *token;
  int type = 0, value = 0;
  
  while (*s) {
    int value = 0;
    if (*s == '(') { type = TYPE_PAREN; value = '('; s++; }
    else if (*s == ')') { type = TYPE_PAREN; value = ')'; s++; }
    else if (*s == '+') { type = TYPE_OPERATOR; value = '+'; s++; }
    else if (*s == '-') { type = TYPE_OPERATOR; value = '-'; s++; }
    else if (*s == '*') { type = TYPE_OPERATOR; value = '*'; s++; }
    else if (*s == '/') { type = TYPE_OPERATOR; value = '/'; s++; }
    else if (isdigit(*s)) {
      type = TYPE_NUMBER;
      while (*s && isdigit(*s))
	value = value * 10 + *s++ - '0';
    }

    if (type == 0) {
      cout << "wrong expression!" << endl;
      return -1;
    }
      
    token = new Token(type, value);
    tokenDeque.push_back(token);
  }
  
  return 0;
}

void print_tokens(void) {
  Token *p;

  for (p = tokenDeque.front(); !tokenDeque.empty(); p = tokenDeque.front()) {
    cout << *p << endl;
    tokenDeque.pop_front();
  }
}

/* Home Work*/
void calc(deque<int> *n, deque<int> *o){
	deque<int>::iterator i = n->begin();
	deque<int>::iterator j = o->begin();
	int temp1, temp2;
	while(*j ==  '(' && i != n->end() && o->empty()){
		switch(*j){
			case ')': break;
			case '(': break;
			case '+': temp1 = *i; n->pop_front(); temp2 = *i; n->pop_front(); n->push_front(temp1+temp2); break;
			case '-': temp1 = *i; n->pop_front(); temp2 = *i; n->pop_front(); n->push_front(temp1-temp2); break;
			case '*': temp1 = *i; n->pop_front(); temp2 = *i; n->pop_front(); n->push_front(temp1*temp2); break;
			case '/': temp1 = *i; n->pop_front(); temp2 = *i; n->pop_front(); n->push_front(temp1/temp2); break;
			default: break;
		}
		o->pop_front();
		j++;
	}
}
void tokenstack(void){
	deque<int> number;
	deque<int> oper;

	for(int i = 0; i < tokenDeque.size(); i++){
		int t = tokenDeque[i]->type;
		int v = tokenDeque[i]->value;
		switch(t){
			case TYPE_PAREN:
				if(v == ')'){
					i++;
					calc(&number, &oper);
				}
				break;
			case TYPE_OPERATOR:
				if(v == '*' || v == '/'&&tokenDeque[i+1]->type == TYPE_NUMBER){
					i++;
					number.push_front(v);
					calc(&number,&oper);
				}
				oper.push_front(v);
				break;
			case TYPE_NUMBER:
				number.push_front(v);
				break;
			default : break;
		}
	}
	calc(&number, &oper);
}

int main(int argc, char *argv[])
{
  istream& input_stream = cin;
  string input_expr;
	
  cout << "Type any arithmetic express with no spaces such as (l00+200)*300-400/100" << endl;
  while (true) {	
    cout << "> ";
    if (!getline(input_stream, input_expr))
      break;

    if (parse((char *)input_expr.c_str()) < 0)
      continue;

    print_tokens();
  }
  return EXIT_SUCCESS;
}
