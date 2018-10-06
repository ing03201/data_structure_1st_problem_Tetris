#pragma once
#include <string>
#include <iostream>

using namespace std;

class GameEntry {
public:
  GameEntry(const string& n="", int s=0);
  string getName() const;			
  int getScore() const;
  friend ostream& operator<<(ostream& out, const GameEntry& obj);  
private:
  string name;					
  int score;					
};

GameEntry::GameEntry(const string& n, int s): name(n), score(s) { }

string GameEntry::getName() const { return name; }

int GameEntry::getScore() const { return score; }

ostream& operator<<(ostream& out, const GameEntry& obj){
  out << "GameEntry(" << obj.name << ", " << obj.score << ")";
  return out;
}





