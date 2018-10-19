#pragma once
#include "DLinkedList.h"

using namespace std;

//-----------------------------------------------------//
// Class definition : LinkedDeque                       
//-----------------------------------------------------//
class DequeException {
 public:
  DequeException(): message("Deque Exception") { }
  DequeException(string msg): message(msg) {}
  string getMessage() const { return message; }
 protected:
  string message;
};

template <typename E>
class LinkedDeque {				// deque as doubly linked list
 public:
  LinkedDeque();				// constructor
  int size() const;				// number of items in the deque
  bool empty() const;				// is the deque empty?
  const E& front() const throw(DequeException); 	// the first element
  const E& back() const throw(DequeException);	// the last element
  const E& elem(int k) const throw(DequeException); // the k-th element  
  const E& operator[](int k) const throw(DequeException); // the k-th element
  void insertFront(const E& e);		// insert new first element
  void insertBack(const E& e);		// insert new last element
  void removeFront() throw(DequeException);	// remove first element
  void removeBack() throw(DequeException);	// remove last element
  void clear() throw(DequeException);		// remove all elements
 private:					// member data
  DLinkedList<E> D;				// linked list of elements
  int n;					// number of elements
};

template <typename E>
LinkedDeque<E>::LinkedDeque() { n = 0; }

template <typename E>
int LinkedDeque<E>::size() const { return n; }

template <typename E>
bool LinkedDeque<E>::empty() const { return D.empty(); }

template <typename E>
const E& LinkedDeque<E>::front() const throw(DequeException) {
  if (D.empty())
    throw DequeException("front of empty queue");

  return D.front();
}

template <typename E>
const E& LinkedDeque<E>::back() const throw(DequeException) {
  if (D.empty())
    throw DequeException("back of empty queue");

  return D.back();
}

template <typename E>
const E& LinkedDeque<E>::elem(int k) const throw(DequeException) {
  if (D.empty())
    throw DequeException("front of empty queue");

  return D.elem(k);
}

template <typename E>
const E& LinkedDeque<E>::operator[](int k) const throw(DequeException) {
  if (D.empty())
    throw DequeException("front of empty queue");

  return D.elem(k);
}

template <typename E>
void LinkedDeque<E>::insertFront(const E& e) {
  D.addFront(e);
  n++;
}

template <typename E>
void LinkedDeque<E>::insertBack(const E& e) {
  D.addBack(e);
  n++;
}

template <typename E>
void LinkedDeque<E>::removeFront() throw(DequeException) {
  if (empty())
    throw DequeException("removeFront of empty deque");
  D.removeFront();
  n--;
}

template <typename E>
void LinkedDeque<E>::removeBack() throw(DequeException)  {
  if (empty())
    throw DequeException("removeBack of empty deque");
  D.removeBack();
  n--;
}

template <typename E>
void LinkedDeque<E>::clear() throw(DequeException)  {
  if (empty())
    throw DequeException("clear of empty deque");

  while (!empty()) {
    D.removeBack();
    n--;
  }
}

