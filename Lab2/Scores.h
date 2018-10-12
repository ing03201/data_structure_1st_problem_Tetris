#pragma once
#include <stdexcept>
#include "GameEntry.h"

using namespace std;

/*
GameEntry 객체를 받아
배열을 할당해서 배열을 관리한다.
*/

class Scores {				
public:
  Scores(int maxEnt = 10);		// 생성자
  ~Scores();				          // 소멸자
  void add(const GameEntry& e);		
  GameEntry remove(int i) throw(out_of_range);
private:
  int maxEntries;			
  int numEntries;			
  GameEntry* entries;			// GameEntry 배열의 첫원소의 주소
};

Scores::Scores(int maxEnt) {		
  maxEntries = maxEnt;			
  entries = new GameEntry[maxEntries];	
  numEntries = 0;			
}

Scores::~Scores() {			
  delete[] entries;
}

void Scores::add(const GameEntry& e) {	
  int newScore = e.getScore();		
  if (numEntries == maxEntries) {	
    if (newScore <= entries[maxEntries-1].getScore())
      return;				
  }
  else numEntries++;			
  
  int i = numEntries-2; 		
  while ( i >= 0 && newScore > entries[i].getScore() ) {
    entries[i+1] = entries[i];		
    i--;
  }
  entries[i+1] = e;			
}

GameEntry Scores::remove(int i) throw(out_of_range) {
  if ((i < 0) || (i >= numEntries))	
    throw out_of_range("Invalid index");
  GameEntry e = entries[i];		
  for (int j = i+1; j < numEntries; j++)
    entries[j-1] = entries[j];		
  numEntries--;				
  return e;				
}


