// Taken from the slides made by Prof. Youngtak Kim at Yeungnam Univ.
#include <iostream>
#include <list>

using namespace std;

typedef int Elem;					// base element type
class LinkedBinaryTree {
protected:
  struct Node {					// a node of the tree
    Elem    elt;					// element value
    Node*   par;					// parent
    Node*   left;					// left child
    Node*   right;					// right child
    Node() : elt(), par(NULL), left(NULL), right(NULL) { } // constructor
    Node(Elem e) : elt(e), par(NULL), left(NULL), right(NULL) { } // constructor
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
  LinkedBinaryTree();					// constructor
  int size() const;					// number of nodes
  bool empty() const;					// is tree empty?
  Position root() const;				// get the root
  PositionList positions() const;  			// list of nodes
  void addRoot();					// add root to empty tree
  void expandExternal(const Position& p);		// expand external node
  Position removeAboveExternal(const Position& p);	// remove p and parent
  // housekeeping functions (destructor, copy constructor, assign operator overloading) omitted...
  LinkedBinaryTree::Position addElementInOrder(Position p, Position paren, const Elem& elem);
  void printTree(Position p);
protected: 						// local utilities
  void preorder(Node* v, PositionList& pl) const;	// preorder utility
private:
  Node* _root;					// pointer to the root
  int n;						// number of nodes
};

LinkedBinaryTree::LinkedBinaryTree()			// constructor
  : _root(NULL), n(0) { }

int LinkedBinaryTree::size() const			// number of nodes
{ return n; }

bool LinkedBinaryTree::empty() const			// is tree empty?
{ return size() == 0; }

LinkedBinaryTree::Position LinkedBinaryTree::root() const // get the root
{ return Position(_root); }

void LinkedBinaryTree::addRoot()			// add root to empty tree
{ _root = new Node; n = 1; }

// list of all nodes
LinkedBinaryTree::PositionList LinkedBinaryTree::positions() const {
  PositionList pl;
  preorder(_root, pl);					// preorder traversal
  return PositionList(pl);				// return resulting list
}

// expand external node
void LinkedBinaryTree::expandExternal(const Position& p) {
  Node* v = p.v;					// p's node
  v->left = new Node;					// add a new left child
  v->left->par = v;					// v is its parent
  v->right = new Node;				// and a new right child
  v->right->par = v;					// v is its parent
  n += 2;						// two more nodes
}

LinkedBinaryTree::Position				// remove p and parent
LinkedBinaryTree::removeAboveExternal(const Position& p) {
  Node* w = p.v;  Node* v = w->par;			// get p's node and parent
  Node* sib = (w == v->left ?  v->right : v->left);
  if (v == _root) {					// child of root?
    _root = sib;					// ...make sibling root
    sib->par = NULL;
  }
  else {
    Node* gpar = v->par;				// w's grandparent
    if (v == gpar->left) gpar->left = sib; 		// replace parent by sib
    else gpar->right = sib;
    sib->par = gpar;
  }
  delete w; delete v;					// delete removed nodes
  n -= 2;						// two fewer nodes
  return Position(sib);
}

// preorder traversal
void LinkedBinaryTree::preorder(Node* v, PositionList& pl) const {
  pl.push_back(Position(v));				// add this node
  if (v->left != NULL)					// traverse left subtree
    preorder(v->left, pl);
  if (v->right != NULL)					// traverse right subtree
    preorder(v->right, pl);
}


LinkedBinaryTree::Position
LinkedBinaryTree::addElementInOrder(Position p, Position paren, const Elem& elem) {
  LinkedBinaryTree::Position newPos;
  
  if (p.isNULL()) {
    p.v = new Node(elem);
    if (paren.v == NULL)
      _root = p.v;
    p.v->par = paren.v;
    cout << "Node(" << *p << ") created" << endl;
    n++;
    return LinkedBinaryTree::Position(p.v);
  }
  else if (elem < *p) {
    cout << "leftOf(" << *p << ") taken" << endl;
    newPos = addElementInOrder(p.left(), p, elem);
    if (newPos.isNULL()) return newPos;
    p.v->left = newPos.v;
    return LinkedBinaryTree::Position(p.v);
  }
  else if (elem > *p) {
    cout << "rightOf(" << *p << ") taken" << endl;
    newPos = addElementInOrder(p.right(), p, elem);
    if (newPos.isNULL()) return newPos;
    p.v->right = newPos.v;
    return LinkedBinaryTree::Position(p.v);
  }
  else {
    cout << "Duplicated element!!" << endl;
    return LinkedBinaryTree::Position(NULL);
  }
}

void LinkedBinaryTree::printTree(Position p) {
  if (p.isNULL()) return;
  cout << *p << " ";
  printTree(p.left());
  printTree(p.right());
}

int main(int argc, char *argv[]) {
  LinkedBinaryTree t;
  int v = 100;

#if 0
  cout << "size=" << t.size() << endl;
  t.addRoot(); // Never call it more than once!!! -> memory leak
  cout << "size=" << t.size() << endl;
  LinkedBinaryTree::Position p = t.root();

  t.expandExternal(p);
  cout << "size=" << t.size() << endl;

  LinkedBinaryTree::PositionList pl = t.positions();
  list<LinkedBinaryTree::Position>::iterator it;
  for (it = pl.begin(); it != pl.end(); it++) {
    LinkedBinaryTree::Position p = *it;
    *p = v;
    v += 100;
    cout << *p << endl;
  }
#endif
  
  for (int i = 0; i < 10; i++) {
    v = rand() % 100;
    cout << "size = " << t.size() << " ************************************" << endl;
    cout << "Adding " << v << " into LinkedBinaryTree" << endl;
    t.addElementInOrder(t.root(), LinkedBinaryTree::Position(NULL), v);
  }
  cout << "size = " << t.size() << " ************************************" << endl;
  cout << "Tree traversal: ";
  t.printTree(t.root()); cout << endl;

  LinkedBinaryTree::PositionList pl = t.positions();
  list<LinkedBinaryTree::Position>::iterator it;
  cout << "Tree traversal: ";
  for (it = pl.begin(); it != pl.end(); it++) {
    LinkedBinaryTree::Position p = *it;
    cout << *p << " ";
  }
  cout << endl;
  
  return 0;
}
