#include <string>
#include <iostream>
#include "GameEntry.h"
#include "Scores.h"

using namespace std;

int main1(int argc, char *argv[]) {
  GameEntry e1("aa", 10), e2("bb", 20),
  	e3("cc", 30), e4("dd", 40); // board 객체는 GameEntry를 최대 3개 수용할수 있다. -> GameEntry를 4개 추가하면?
  Scores board(3);

  board.add(e1);
  board.add(e2);
  board.add(e3);
  board.add(e4);
  try {
    GameEntry e = board.remove(0);    cout << e << endl;
    e = board.remove(0);    cout << e << endl;
    e = board.remove(0);    cout << e << endl;
  }
  catch (out_of_range& e) { // try 구문 안에서 exception 발생시 프로그램 흐름은 여기로 이동한다!
    cout << e.what() << endl;
  }
  return 0;
}

#include "SLinkedList.h"
int main2(int argc, char *argv[]) {
  const GameEntry e1("aa", 10), e2("bb", 20), e3("cc", 30);
  SLinkedList<GameEntry> list;
  list.addFront(e1);
  list.addFront(e2);
  list.addFront(e3);

  const GameEntry *e;
  if ((e = list.find("aa")) != NULL)    cout << *e << endl;
  else cout << "aa: not found";
  if ((e = list.find("bb")) != NULL)    cout << *e << endl;
  else cout << "bb: not found";
  if ((e = list.find("cc")) != NULL)    cout << *e << endl;
  else cout << "cc: not found";
  if ((e = list.find("dd")) != NULL)    cout << *e << endl;
  else cout << "dd: not found";

  return 0;
}

int main2hw(int argc, char *argv[]) {
  const GameEntry e1("aa", 10), e2("bb", 20), e3("cc", 30);
  const GameEntry e4("xx", 25), e5("yy", 15), e6("zz", 5);
  SLinkedList<GameEntry> list;
  list.addSorted(e1);
  list.addSorted(e2);
  list.addSorted(e3);
  list.addSorted(e4);
  list.addSorted(e5);
  list.addSorted(e6);
  list.print();
  return 0;
}

#include "DLinkedList.h"
int main3(int argc, char *argv[]) {
  const GameEntry e1("aa", 10), e2("bb", 20), e3("cc", 30);
  const GameEntry e4("xx", 25), e5("yy", 15), e6("zz", 5);
  DLinkedList<GameEntry> list;
  list.addFront(e1);
  list.addFront(e2);
  list.addFront(e3);
  list.addBack(e4);
  list.addBack(e5);
  list.addBack(e6);
  list.print();
  return 0;
}

int main3hw(int argc, char *argv[]) {
  const GameEntry e1("aa", 10), e2("bb", 20), e3("cc", 30);
  const GameEntry e4("xx", 25), e5("yy", 15), e6("zz", 5);
  DLinkedList<GameEntry> list;
  list.addSorted(e1);
  list.addSorted(e2);
  list.addSorted(e3);
  list.addSorted(e4);
  list.addSorted(e5);
  list.addSorted(e6);
  list.print();
  return 0;
}

int main(int argc, char *argv[]) {
  string s1 = "1";
  string s2 = "2";
  string s2hw = "2hw";
  string s3 = "3";
  string s3hw = "3hw";
  
  if (argc != 2) {
    cout << "usage: " << argv[0] << " [function number]" << endl;
    return 0;
  }

  if (s1.compare(argv[1]) == 0) main1(argc, argv);
  else if (s2.compare(argv[1]) == 0) main2(argc, argv);
  else if (s2hw.compare(argv[1]) == 0) main2hw(argc, argv);
  else if (s3.compare(argv[1]) == 0) main3(argc, argv);
  else if (s3hw.compare(argv[1]) == 0) main3hw(argc, argv);
  else
    cout << "usage: " << argv[0] << " [function number]" << endl;

  return 0;
}
