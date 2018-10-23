#include <iostream>
#include <vector>
#include <list>


using namespace std;

///////////////////////////////////////////////////////
// Point2D class
///////////////////////////////////////////////////////
class Point2D {
private:
  int y;
  int x;
public:
  Point2D(): y(0), x(0) {}
  Point2D(int _y, int _x): y(_y), x(_x) {}
  int getX() const { return x; }
  int getY() const { return y; }
  void setX(int _x) { x = _x; }
  void setY(int _y) { y = _y; }
  friend ostream& operator<<(ostream& out, const Point2D& obj);
};

ostream& operator<<(ostream& out, const Point2D& obj){
  out << "(" << obj.x << ", " << obj.y << ")";
  return out;
}

///////////////////////////////////////////////////////
// VectorCompleteTree class 
///////////////////////////////////////////////////////

template <typename E>
class VectorCompleteTree { // 벡터 완전 이진트리
private:					// member data
  std::vector<E> V;				// tree contents
public:					// publicly accessible types
  typedef typename std::vector<E>::iterator Position; // a position in the tree
protected:					// protected utility functions
  Position pos(int i)				// map an index to a position
  { return V.begin() + i; }
  int idx(const Position& p) const		// map a position to an index
  { return p - V.begin(); }
public:
  VectorCompleteTree() : V(1) {}		// constructor
  int size() const				{ return V.size() - 1; }
  Position left(const Position& p)		{ return pos(2*idx(p)); }
  Position right(const Position& p)		{ return pos(2*idx(p) + 1); }
  Position parent(const Position& p)		{ return pos(idx(p)/2); }
  bool hasLeft(const Position& p) const	{ return 2*idx(p) <= size(); }
  bool hasRight(const Position& p) const 	{ return 2*idx(p) + 1 <= size(); }
  bool isRoot(const Position& p) const	{ return idx(p) == 1; }
  Position root()				{ return pos(1); }
  Position last()				{ return pos(size()); }
  void addLast(const E& e)			{ V.push_back(e); }
  void removeLast()				{ V.pop_back(); }
  void swap(const Position& p, const Position& q)
  { E e = *q; *q = *p; *p = e; }
};

///////////////////////////////////////////////////////
// HeapPriorityQueue class
///////////////////////////////////////////////////////

template <typename E, typename C>
class HeapPriorityQueue { 
public:
  int size() const; 				// number of elements
  bool empty() const;  			// is the queue empty?
  void insert(const E& e);			// insert element
  const E& min();				// minimum element
  void removeMin();				// remove minimum
private: 
  VectorCompleteTree<E> T;			// priority queue contents
  C isLess;					// less-than comparator
    						// shortcut for tree position
  typedef typename VectorCompleteTree<E>::Position Position;
};

template <typename E, typename C>		// insert element
void HeapPriorityQueue<E,C>::insert(const E& e) {
  T.addLast(e);				// add e to heap
  Position v = T.last();			// e's position
  while (!T.isRoot(v)) {			// up-heap bubbling
    Position u = T.parent(v);
    if (!isLess(*v, *u)) break;		// if v in order, we're done
    T.swap(v, u);				// ...else swap with parent
    v = u;
  }
}

template <typename E, typename C>		// remove minimum
void HeapPriorityQueue<E,C>::removeMin() {
  if (size() == 1)				// only one node?
    T.removeLast();				// ...remove it
  else {
    Position u = T.root();			// root position
    T.swap(u, T.last());			// swap last with root
    T.removeLast();				// ...and remove last
    while (T.hasLeft(u)) { 			// down-heap bubbling
      Position v = T.left(u);
      if (T.hasRight(u) && isLess(*(T.right(u)), *v))
	v = T.right(u);			// v is u's smaller child
      if (isLess(*v, *u)) {			// is u out of order?
	T.swap(u, v);				// ...then swap
	u = v;
      }
      else break;				// else we're done
    }
  }
}

template <typename E, typename C>		// number of elements
int HeapPriorityQueue<E,C>::size() const
{ return T.size(); }

template <typename E, typename C>		// is the queue empty?
bool HeapPriorityQueue<E,C>::empty() const
{ return size() == 0; }

template <typename E, typename C>		// minimum element
const E& HeapPriorityQueue<E,C>::min() 
{ return *(T.root()); }			// return reference to root element


///////////////////////////////////////////////////////
// LeftRight comparator class
///////////////////////////////////////////////////////

class LeftRight {
public:
  bool operator()(const Point2D& p, const Point2D& q) const
  { return p.getX() < q.getX(); }
};


///////////////////////////////////////////////////////
// HeapSort algorithm part
///////////////////////////////////////////////////////

void TopDownHeapSort(list<Point2D> &pl) {
  Point2D p;
  int n = pl.size();
  HeapPriorityQueue<Point2D, LeftRight> T;

  if (pl.empty())
    return;

  list<Point2D>::iterator it;
  for (it = pl.begin(); it != pl.end(); ++it)
    T.insert(*it);

  pl.clear();
  for (int i = 0; i < n; i++) {
    p = T.min();
    pl.push_back(p);
    T.removeMin();
  }
  return;
}

int main(int argc, char *argv[]) {
  list<Point2D> pl;
  Point2D *p;
  int y, x;
  int n = 15; // n should be 2^(h+1) - 1

  cout << "before heapsort: " << endl;
  for (int i = 0; i < n; i++) {
    y = rand() % 100;
    x = rand() % 100;
    p = new Point2D(y, x);
    cout << *p << endl;
    pl.push_back(*p);
  }

  TopDownHeapSort(pl);

  cout << "after heapsort: " << endl;
  list<Point2D>::iterator it;
  for (it = pl.begin(); it != pl.end(); ++it) {
    cout << *it << endl;
  }
  
  //BottomUpHeap(pl);
  
  return 0;
}
