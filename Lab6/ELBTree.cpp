#include <iostream>
#include <list>
#include <deque>

using namespace std;

//////////////////////////////////////////////////////////////
// Token class definition 
//////////////////////////////////////////////////////////////

#define TYPE_LPAREN 1
#define TYPE_RPAREN 2
#define TYPE_OPERATOR 3
#define TYPE_NUMBER 4

class Token {
public:
  Token(int t, int v) : type(t), value(v) {}
  int type;
  int value;
  friend ostream& operator<<(ostream& out, const Token& obj){
    if (obj.type == TYPE_LPAREN || obj.type == TYPE_RPAREN)
      out << (char) obj.value;
    else if (obj.type == TYPE_OPERATOR)
      out << (char) obj.value;
    else if (obj.type == TYPE_NUMBER)
      out << obj.value;
    return out;
  }
};


int parse(char *s, deque<Token*>& deq) {
  Token *token;
  int type = 0, value = 0;

  while (*s) {
    int value = 0;
    if (*s == '(') { type = TYPE_LPAREN; value = '('; s++; }
    else if (*s == ')') { type = TYPE_RPAREN; value = ')'; s++; }
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
    deq.push_back(token);
  }

  return 0;
}

void printTokens(deque<Token*>& deq) {
  deque<Token*>::iterator it;
  Token *p;

  for (it = deq.begin(); it != deq.end(); it++) {
    p = *it;
    cout << *p;
  }
  cout << endl;
}


//////////////////////////////////////////////////////////////
// LinkedBinaryTree class definition
//////////////////////////////////////////////////////////////

typedef Token* Elem;					// base element type
class LinkedBinaryTree {
protected:
  struct Node {					// a node of the tree
    Elem    elt;					// element value
    Node*   par;					// parent
    Node*   left;					// left child
    Node*   right;					// right child
    Node() : elt(), par(NULL), left(NULL), right(NULL) { } // constructor
    Node(Elem e) : elt(e), par(NULL), left(NULL), right(NULL) { } // constructor
    Node(Node *l, Node *r, Elem e) : elt(e), par(NULL), left(l), right(r) { } // constructor
  };
public:
  class Position {					// position in the tree
  private:
    Node* v;						// pointer to the node
  public:
    Position(Node* _v = NULL) : v(_v) { }		// constructor
    Elem& operator*()					// get element
    { return v->elt; }
    Position left() const				// get left child
    { return Position(v->left); }
    Position right() const				// get right child
    { return Position(v->right); }
    Position parent() const				// get parent
    { return Position(v->par); }
    bool isNULL() const				// null node?
    { return v == NULL; }
    bool isRoot() const				// root of the tree?
    { return v->par == NULL; }
    bool isExternal() const				// an external node?
    { return v->left == NULL && v->right == NULL; }
    friend class LinkedBinaryTree;			// give tree access
  };
  typedef std::list<Position> PositionList;		// list of positions
public:
  LinkedBinaryTree() : _root(NULL), n(0) { }		// constructor
  LinkedBinaryTree(Elem elt)
  { addRoot(elt); }
  LinkedBinaryTree(LinkedBinaryTree& left, LinkedBinaryTree &right, Elem elt)
  { addRoot(left, right, elt); }
  int size() const;					// number of nodes
  bool empty() const;					// is tree empty?
  Position root() const;				// get the root
  PositionList positions() const;  			// list of nodes
  void addRoot();					// add root to empty tree
  void addRoot(Elem elt);	// add root to empty tree
  void addRoot(LinkedBinaryTree& left, LinkedBinaryTree& right, Elem elt);	// add root to empty tree
  void printTree(Position p);
protected: 						// local utilities
  void preorder(Node* v, PositionList& pl) const;	// preorder utility
  void inorder(Node* v, PositionList& pl) const;	// inorder utility
private:
  Node* _root;					// pointer to the root
  int n;						// number of nodes
};

int LinkedBinaryTree::size() const			// number of nodes
{ return n; }

bool LinkedBinaryTree::empty() const			// is tree empty?
{ return size() == 0; }

LinkedBinaryTree::Position LinkedBinaryTree::root() const // get the root
{ return Position(_root); }

void LinkedBinaryTree::addRoot()			// add root to empty tree
{ _root = new Node; n = 1; }

void LinkedBinaryTree::addRoot(Elem elt)			// add root to empty tree
{
  _root = new Node(NULL, NULL, elt);
  n = 1;
}

void LinkedBinaryTree::addRoot(LinkedBinaryTree& left, LinkedBinaryTree& right, Elem elt)			// add root to empty tree
{
  Position lroot = left.root();
  Position rroot = right.root();
  _root = new Node(lroot.v, rroot.v, elt);
  lroot.v->par = _root;
  rroot.v->par = _root;
  n = left.size() + right.size() + 1;
}

// list of all nodes
LinkedBinaryTree::PositionList LinkedBinaryTree::positions() const {
  PositionList pl;
  //preorder(_root, pl);					// preorder traversal
  inorder(_root, pl);					// inorder traversal
  return PositionList(pl);				// return resulting list
}

// preorder traversal
void LinkedBinaryTree::preorder(Node* v, PositionList& pl) const {
  pl.push_back(Position(v));				// add this node
  if (v->left != NULL)					// traverse left subtree
    preorder(v->left, pl);
  if (v->right != NULL)					// traverse right subtree
    preorder(v->right, pl);
}

// in-order traversal
void LinkedBinaryTree::inorder(Node* v, PositionList& pl) const {
  if (v->left != NULL)					// traverse left subtree
    inorder(v->left, pl);
  pl.push_back(Position(v));				// add this node
  if (v->right != NULL)					// traverse right subtree
    inorder(v->right, pl);
}

void LinkedBinaryTree::printTree(Position p) {
  if (p.isNULL()) return;
  Elem t = *p;
  cout << "(";
  printTree(p.left());
  cout << *t;
  printTree(p.right());
  cout << ")";
}

void printTree(LinkedBinaryTree& tree) {
  LinkedBinaryTree::PositionList pl = tree.positions();
  list<LinkedBinaryTree::Position>::iterator it;
  for (it = pl.begin(); it != pl.end(); it++) {
    LinkedBinaryTree::Position p = *it;
    Token *t = *p;
    cout << *t;
  }
  cout << endl;
}

//////////////////////////////////////////////////////////////
// Expression Tree Composition Algorithm
//////////////////////////////////////////////////////////////

Token *findOperator(deque<Token*>& deq, deque<Token*>& consumed) {
  deque<Token*> parenDeq;
  Token *p;

  while (!deq.empty()) {
    p = deq.front();
    deq.pop_front();
    if (p->type == TYPE_LPAREN)
      parenDeq.push_back(p);
    else if (p->type == TYPE_RPAREN) {
      if (deq.empty()) return NULL;
      else parenDeq.pop_back();
    }
    //else if (p->type == TYPE_NUMBER) // do nothing
    else if (p->type == TYPE_OPERATOR) {
      if (parenDeq.empty())
	return p; // found!!
    }
    consumed.push_back(p);
  }

  return NULL;
}

LinkedBinaryTree buildTree(deque<Token*>& deq) {
  deque<Token*> consumed;
  Token *p, *o;

  //printTokens(deq);
  
  p = deq.front(); deq.pop_front(); // remove lparen
  if (p->type == TYPE_LPAREN) {
    p = deq.back(); deq.pop_back(); // remove rparen
    if (p->type == TYPE_RPAREN) {
      LinkedBinaryTree left;
      LinkedBinaryTree right;
      o = findOperator(deq, consumed);
      if (o == NULL) return LinkedBinaryTree();
      cout << "Operator: " << *o << endl;
      cout << "Left: "; printTokens(consumed);
      cout << "Right: "; printTokens(deq);
      left = buildTree(consumed);
      if (left.empty()) return LinkedBinaryTree();
      right = buildTree(deq);
      if (right.empty()) return LinkedBinaryTree();
      return LinkedBinaryTree(left, right, o); // compose a tree
    }
    else
      return LinkedBinaryTree(); // return a null tree
  }
  else if (p->type == TYPE_NUMBER) {
    if (deq.empty())
      return LinkedBinaryTree(p); // return a single-node tree
    else
      return LinkedBinaryTree(); // return a null tree
  }
  else 
    return LinkedBinaryTree(); // return a null tree
}

//////////////////////////////////////////////////////////////
// Main function
//////////////////////////////////////////////////////////////

int main(int argc, char *argv[]) {
  istream& input_stream = cin;

  while (true) {
    string input_expr;
    deque<Token*> tokenDeque;
    Token *t;
    LinkedBinaryTree exprTree;

    cout << "> ";
    if (!getline(input_stream, input_expr))
      break;

    if (parse((char *)input_expr.c_str(), tokenDeque) < 0)
      continue;

    cout << "input: ";
    printTokens(tokenDeque);
    exprTree = buildTree(tokenDeque);
    if (exprTree.empty()) {
      cout << "wrong expression!" << endl;
      continue;
    }
    cout << "output w/  paren: ";
    exprTree.printTree(exprTree.root()); cout << endl;
    cout << "output w/o paren: ";
    printTree(exprTree); cout << endl;    
  }
  return 0;
}
