#include <iostream>
#include <deque>

using namespace std;

#define EXIT_SUCCESS 0
/*
#define PLUS 1 // plus minus
#define MINUS 2 // product division
#define
*/
#define TYPE_PAREN 1
#define TYPE_OPERATOR 2
#define TYPE_NUMBER 3


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


/* Home Work*/
int calc(deque<int> *n, deque<int> *o){
	deque<int>::iterator i = o->begin();
	int temp1, temp2;
	int total = 0;
	while(!o->empty()){
		temp1 = n->front();
		n->pop_front();
		temp2 = n->front();
		n->pop_front();
		switch(*i){
			case '(': break;
			case '+': total = temp1 + temp2; break;
			case '-': total = temp1 - temp2; break;
			case '*': total = temp1 * temp2; break;
			case '/': total = temp1 / temp2; break;
			default: break;
		}
		if(total != 0){
			n->push_front(total);
		}
		o->pop_front();
		i++;
	}
	if(o->empty() || n->size() == 1)
		total = n->front();
	return total;
	
}
int tokenstack(void){
	deque<int> number;
	deque<int> oper;
	int result;
	for(unsigned int i = 0; i < tokenDeque.size(); i++){
		int t = tokenDeque[i]->type;
		int v = tokenDeque[i]->value;
		switch(t){
			case TYPE_PAREN:
				if(v == ')')
					result = calc(&number, &oper);
					number.push_front(result);
				if(v == '(')
					oper.push_front(v);
				break;
			case TYPE_OPERATOR:
				oper.push_front(v);
				if(v == '*' || v == '/'){
					i++;
					if(tokenDeque[i]->type == TYPE_NUMBER){
						v = tokenDeque[i]->type;
						number.push_front(v);
						result = calc(&number, &oper);
						number.push_front(result);
					}
						
				}
				break;
			case TYPE_NUMBER:
				number.push_front(v);
				break;
			default : break;
		}
		
	}
	result = calc(&number, &oper);
	return result;
}
/*
int priority(int k)
{
	switch(k){
		case '+': return PM; break;
		case '-': return PM; break;
		case '*': return PD; break;
		case '/': return PD; break;
		default : return 0; break;
	}
}
*/
void print_tokens(void) {
  Token *p;
  int result = tokenstack();
  for (p = tokenDeque.front(); !tokenDeque.empty(); p = tokenDeque.front()) {
    cout << *p << endl;
    tokenDeque.pop_front();
  }
  cout << "Calculate result = " << result << endl;
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
