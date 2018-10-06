#pragma once
#include <string>

using namespace std;

//-----------------------------------------------------//
// Class definition : SLinkedList                       
//-----------------------------------------------------//

template <typename E> class SLinkedList;

template <typename E>
class SNode {				// singly linked list node
private:
  E elem;				// linked list element value
  SNode<E>* next;			// next item in the list
  friend class SLinkedList<E>;		// provide SLinkedList access
};

template <typename E>
class SLinkedList {			// a singly linked list
public:
  SLinkedList();			// empty list constructor
  ~SLinkedList();			// destructor
  bool empty() const;			// is list empty?
  const E& front() const;		// return front element
  void addFront(const E& e);		// add to front of list
  void removeFront();			// remove front item list
  const E* find(const string& s);
  void addSorted(const E& e);		
  void print(void);			
private:
  SNode<E>* head;			// head of the list
};

template <typename E>
SLinkedList<E>::SLinkedList() : head(NULL) { }	// constructor

template <typename E>
SLinkedList<E>::~SLinkedList() {		// destructor
  while (!empty())
    removeFront();
}

template <typename E>
bool SLinkedList<E>::empty() const {		// is list empty?
  return head == NULL;
}

template <typename E>
const E& SLinkedList<E>::front() const	{	// return front element
  return head->elem;
}

template <typename E>
void SLinkedList<E>::addFront(const E& e) {	// add to front of list
  SNode<E>* v = new SNode<E>;			// create new node
  v->elem = e;					// store data
  v->next = head;				// head now follows v
  head = v;					// v is now the head
}

template <typename E>
void SLinkedList<E>::removeFront() {		// remove front item
  SNode<E>* old = head;				// save current head
  head = old->next;				// skip over old head
  delete old;					// delete the old head
}

template <typename E>
const E* SLinkedList<E>::find(const string& s) {	
  SNode<E>* v = head;

  if (empty() == true)
    return NULL;

  do {
    if (s.compare(v->elem.getName()) == 0)
      return &v->elem;
    v = v->next;
  } while (v != NULL);

  return NULL;
}

template <typename E>
void SLinkedList<E>::print(void) {	
  SNode<E>* n = head;
  while (n != NULL) {
    cout << n->elem << " -> ";
    n = n->next;
  }
  cout << "NULL" << endl;
  return;
}

//-----------------------------------------------------//
// Homework!!
//-----------------------------------------------------//
template <typename E>
void SLinkedList<E>::addSorted(const E& e) {	
  return;
}

