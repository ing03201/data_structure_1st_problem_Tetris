#pragma once // 헤더파일 중복 입력 방지
#include <string>
#include <iostream>

// 배열 사용의 예시

/*
  Score = { GameEntry1,  GameEntry2,  GameEntry3 ...  GameEntryN } 으로 정의
  단, Scores 객체 안에서 GameEntry들은 score를 기준으로 내림차순으로 정렬되어야한다.
*/
using namespace std;

class GameEntry {
public:
  GameEntry(const string& n="", int s=0); // 생성자
  string getName() const;			
  int getScore() const;
  friend ostream& operator<<(ostream& out, const GameEntry& obj);  
  // (name, score)를 화면에 출력하기 위한 프렌드 함수
private:
  string name;					
  int score;					
};

GameEntry::GameEntry(const string& n, int s): name(n), score(s) { }

string GameEntry::getName() const { return name; }
// const : 변수를 변경하지 않을 때!
int GameEntry::getScore() const { return score; }

ostream& operator<<(ostream& out, const GameEntry& obj){
  out << "GameEntry(" << obj.name << ", " << obj.score << ")";
  return out;
}
