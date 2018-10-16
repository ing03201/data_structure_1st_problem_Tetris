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
  maxEntries = maxEnt; //  엔트리의 최대치를 입력
  entries = new GameEntry[maxEntries];	
  numEntries = 0;			
}

Scores::~Scores() {			
  delete[] entries;
}

void Scores::add(const GameEntry& e) {	
  int newScore = e.getScore();		
  if (numEntries == maxEntries) { // 10명의 엔트리가 모두 차있다면
    if (newScore <= entries[maxEntries-1].getScore()) // 마지막 엔트리의 점수와 비교하여 
      return;	// 기존점수가 크다면 새로운 엔트리의 점수는 버린다.
  }
  else numEntries++; // 10개 원소가 채워지지않음
  
  int i = numEntries-2; // 루프문의 횟수 		
  while ( i >= 0 && newScore > entries[i].getScore() ) { // 기존점수의 insertion sort
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
  numEntries--;	// 엔트리의 최대 갯수를 뺀다.			
  return e;				
}


