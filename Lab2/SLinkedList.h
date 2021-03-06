#pragma once
#include <string>

using namespace std;

//-----------------------------------------------------//
// Class definition : SLinkedList                       
//-----------------------------------------------------//

template <typename E> class SLinkedList;
// SLinked 선언이 Snode 선언보다 앞서야 하는 이유?
// SNode에서 SLinked 클래스를 사용하기 때문

template <typename E> // E는 데이터 타입
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

	
	if(this->empty() || e.getScore() > this->front().getScore()){
		addFront(e);
	} 
	
	else {
		SNode<E> *cmp; // 기존에 비교할 노드들
		for(cmp = this->head; cmp->next != NULL; cmp = cmp->next){
			if(cmp->next->elem.getScore() >= e.getScore())
				continue;
			else 
				break;
		}
		SNode<E> *v = new SNode<E>; // 새로입력할 노드
		v->elem = e;
		v->next = cmp->next;
		cmp->next = v;
	}
  return;
}

