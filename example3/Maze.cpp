//----------------------------------------------------------//
// Taken from https://github.com/steffanc/MazeBot
// Edited by khkim for readability
//----------------------------------------------------------//
#include <cstdlib> 
#include <iostream>
#include <math.h>
#include <string>
#include <fstream>
//#define __STL_DEQUE__  compiler directive
#ifdef __STL_DEQUE_ // if ㅁ defined xx => xx 가 정의되어있으면 아래 명령어를 선택적 컴파일 하자._
// gcc 명령어로 할땐 g++ -0 Maze.exe -D _STL_DEQUE__  Maze.cpp  => define 명령어와 동일 
#include <deque> // Standard Template Library _STL_DEQUE가 정의되어있을때
#else //  _STL_DEQUE 가 정의 되지 않을 때
	// taken from "Data Structures and Algorithms in C++ (2nd Edition)"
#include "LinkedDeque.h" // 나만의 deque 클래스를 정의 해서 사용할 때 = STL deque와 다르게 사용하기 위해
#define deque LinkedDeque
#define push_back insertBack
#define pop_back removeBack
#define push_front insertFront
#define pop_front removeFront
#endif

using namespace std;

//----------------------------------------------------------//
// Class Definition
//----------------------------------------------------------//
class Point {
private:
  int depth; // 
  Point *parent; //
public:
  int cy, cx;   
  Point(): cy(0), cx(0), depth(0), parent(0) { }
  Point(int cy, int cx, int depth, Point * parent) {	
    this->cy = cy;
    this->cx = cx;
    this->depth = depth;
    this->parent = parent;
  }
  ~Point() { delete parent; }
  int getDepth() const { return depth; };
  Point *getParent() const { return parent; };
  friend ostream& operator<<(ostream& out, const Point& obj){
    out << "p(" << obj.cy << "," << obj.cx << ")";
    return out;
  }
};

//----------------------------------------------------------//
// Global Variables
//----------------------------------------------------------//
static char data_struct;

static const int FREE 		= 0; 	// Empty space
static const int OCCUPIED	= 1;	// Occupied space
static const int START 		= 8;	// Starting position
static const int GOAL 		= 9;	// Goal position
static const int OPEN 		= 6;	// Node added to open queue but not expanded yet
static const int CLOSED 	= 7;	// After node is expanded and explores other nodes around it

int mapHeight, mapWidth;
int **mazeMap;		// Two dimensional array for mapping the original maze
int **mazeRoute; 	// Two dimensional array for keeping track of open and closed nodes
int nMoves = 0;		// Total number of moves made
int maxOpenQSize = 0; 	// Keep track of maximum open queue size to report on memory performance
deque<Point *> openDeque;
deque<Point *> closedDeque;	

//----------------------------------------------------------//
// Map related Functions
//----------------------------------------------------------//
void ClearMaps() {
  for (int y = 0; y < mapHeight; y++) {
    for (int x = 0; x < mapWidth; x++) {
      mazeRoute[y][x] = mazeMap[y][x];
    }
  }
  openDeque.clear();
  closedDeque.clear();
  nMoves = 0;
  maxOpenQSize = 0;
}

void FreeMaps() {
  for (int y = 0; y < mapHeight; y++) delete[] mazeMap[y];
  delete[] mazeMap;
  mazeMap = NULL;

  for (int y = 0; y < mapHeight; y++) delete[] mazeRoute[y];
  delete[] mazeRoute;
  mazeRoute = NULL;
}

void InitMaps(char *fname, Point& start, Point& goal) {
  ifstream mapFile; // 새로운 맵을 만들기 위한 것
  mapFile.open(fname, ios::in);
  if (!mapFile.is_open()) {
    cout << "Unable to open file";
    exit(1);
  }

  mapFile >> mapHeight;
  mapFile >> mapWidth;

  mazeMap = new int*[mapHeight]; // 지도를 만들기 위함
  mazeRoute = new int*[mapHeight]; // 지도를 만들때 필요한 연산을 위한 배열 
  for (int y = 0; y < mapHeight; y++) {
    mazeMap[y] = new int[mapWidth];
    mazeRoute[y] = new int[mapWidth];
  }
  
  for (int y = 0; y < mapHeight; y++) {
    for (int x = 0; x < mapWidth; x++) {
      mapFile >> mazeMap[y][x]; // map파일에 있는 원소를 불러들인다.
      mazeRoute[y][x] = mazeMap[y][x];
      if (mazeMap[y][x] == START) {
	start.cy = y;
	start.cx = x;
      } else if (mazeMap[y][x] == GOAL) {
	goal.cy = y;
	goal.cx = x;
      }
    }
  } 
  mapFile.close();
  return;
}

//----------------------------------------------------------//
// Search Function (Algorithm Part)
//----------------------------------------------------------//
void PrintRoute() {
  cout << '*';
  for (int x = 0; x < mapWidth; x++)
    cout << x%10;
  cout << endl;
  
  for (int y = 0; y < mapHeight; y++) {
    cout << y%10;
    for (int x = 0; x < mapWidth; x++) {
      if(mazeRoute[y][x] == OCCUPIED) cout << "■";
      else if(mazeRoute[y][x] == START) cout << "S";
      else if(mazeRoute[y][x] == GOAL) cout << "G";
      else if(mazeRoute[y][x] == OPEN) cout << "O";
      else if(mazeRoute[y][x] == CLOSED) cout << "C";
      else cout << " ";
    }
    cout << endl;
  }
}

void PrintMap() {
  Point *p;
  deque<Point *> path;
  for (p = closedDeque.back(); p->getParent() != 0; p = p->getParent())
    path.push_front(p);
  path.push_front(p);
  cout << '*';
  for (int x = 0; x < mapWidth; x++)
    cout << x%10;
  cout << endl;
  
  for (int y = 0; y < mapHeight; y++) {
    cout << y%10;
    for (int x = 0; x < mapWidth; x++) {
      if(mazeMap[y][x] == OCCUPIED) cout << "■";
      else if(mazeMap[y][x] == START) cout << "S";
      else if(mazeMap[y][x] == GOAL) cout << "G";
      else {
	bool on_the_path = false;
	for(int k = 0; k < path.size(); k++) {
	  if(path[k]->cy == y && path[k]->cx == x) on_the_path = true;
	}
	if(on_the_path) cout << ".";
	else cout << " ";
      }
    }
    cout << endl;
  }
    
  cout << "\nComplete path: " << endl;
  for (int k = 0; k < path.size(); k++){
    cout << *path[k];  //cout << *path.elem(k);
    if(k < path.size()-1) cout << " -> ";
  }
  cout << endl << endl;
	
  cout << "Path Length (in points) : " << path.size() << endl;
  cout << "Total Moves (in points) : " << nMoves-1 << endl; // Make sure to not count the initial state
  cout << "Maximum Reached Length of Open Queue: " << maxOpenQSize << endl;
  cout << "Final Length of Open Queue (O): " << openDeque.size() << endl;
  cout << "Final Length of Closed Queue (C): " << closedDeque.size() << endl;
  cout << "Total Number of Explored States (O+C): " << openDeque.size()+closedDeque.size() << endl;
}

bool never_visited(int flag) {
  return (flag != OCCUPIED && flag != CLOSED && flag != OPEN);
}


void StepNext_w_Queue(int cy, int cx) { // queue로 검색을 확장한다. 
  int depth = (closedDeque.back()->getDepth())+1;
  Point *parent = closedDeque.back();
  // 탐색하지않은 4 방향의 좌표를 등록-> 사실 3개 등록됨. 이유 : 기존에 검토한 좌표는 제외한다. 
  if (never_visited(mazeRoute[cy][cx-1])) { // 서
    mazeRoute[cy][cx-1] = OPEN;// 
    openDeque.push_back(new Point(cy, cx-1, depth, parent));
    cout << "openDeque.push_back(" << cy << "," << cx-1 << "," << depth << ")" << endl;
  }
  if (never_visited(mazeRoute[cy-1][cx])) { // 북
    mazeRoute[cy-1][cx] = OPEN;
    openDeque.push_back(new Point(cy-1, cx, depth, parent));
    cout << "openDeque.push_back(" << cy-1 << "," << cx << "," << depth << ")" << endl;
  }
  if (never_visited(mazeRoute[cy][cx+1])) { // 동
    mazeRoute[cy][cx+1] = OPEN;
    openDeque.push_back(new Point(cy, cx+1, depth, parent));
    cout << "openDeque.push_back(" << cy << "," << cx+1 << "," << depth << ")" << endl;
  }
  if (never_visited(mazeRoute[cy+1][cx])) { // 남
    mazeRoute[cy+1][cx] = OPEN;
    openDeque.push_back(new Point(cy+1, cx, depth, parent));
    cout << "openDeque.push_back(" << cy+1 << "," << cx << "," << depth << ")" << endl;
  }
  maxOpenQSize = (openDeque.size() > maxOpenQSize) ? openDeque.size() : maxOpenQSize;
}

void StepNext_w_Stack(int cy, int cx) {// stack으로 자료를 입력한다.
  int depth = (closedDeque.back()->getDepth())+1;
  Point *parent = closedDeque.back();
  if (never_visited(mazeRoute[cy][cx-1])) {
    mazeRoute[cy][cx-1] = OPEN;
    openDeque.push_front(new Point(cy, cx-1, depth, parent));
    cout << "openDeque.push_front(" << cy << "," << cx-1 << "," << depth << ")" << endl;
  }
  if (never_visited(mazeRoute[cy-1][cx])) {
    mazeRoute[cy-1][cx] = OPEN;
    openDeque.push_front(new Point(cy-1, cx, depth, parent));
    cout << "openDeque.push_front(" << cy-1 << "," << cx << "," << depth << ")" << endl;
  }
  if (never_visited(mazeRoute[cy][cx+1])) {
    mazeRoute[cy][cx+1] = OPEN;
    openDeque.push_front(new Point(cy, cx+1, depth, parent));
    cout << "openDeque.push_front(" << cy << "," << cx+1 << "," << depth << ")" << endl;
  }
  if (never_visited(mazeRoute[cy+1][cx])) {
    mazeRoute[cy+1][cx] = OPEN;
    openDeque.push_front(new Point(cy+1, cx, depth, parent));
    cout << "openDeque.push_front(" << cy+1 << "," << cx << "," << depth << ")" << endl;
  }
  maxOpenQSize = (openDeque.size() > maxOpenQSize) ? openDeque.size() : maxOpenQSize;
}

void StepNext(int cy, int cx) {
  if (data_struct == 'q') StepNext_w_Queue(cy, cx);
  else if (data_struct == 's') StepNext_w_Stack(cy, cx);
}

bool FindRoute(const Point& start, const Point& goal) { // deque를 이용하여 경로 탐색을 하기위함
  // 두개의 deque를 만듬 open deque(검토 대상을 등록), closed deque(검토가 끝난 객체)
  int cy = 0, cx = 0;
  bool found = false; 
  maxOpenQSize = 1;
  mazeRoute[start.cy][start.cx] = OPEN;
  openDeque.push_back(new Point(start.cy, start.cx, 0, 0));
  cout << "openDeque.push_back(" << start.cy << "," << start.cx << "," << 0 << ")" << endl;

  PrintRoute();
	
  // Keep searching until an goal is determined or no solution is found
  while(openDeque.size() != 0) { // open deque의 원소가 0이 아니면 -> 검토할 point가 없을 때
	  //탐색 시작
    cy = openDeque.front()->cy; // open deque의 머리에 삽입  
    cx = openDeque.front()->cx;
    mazeRoute[cy][cx] = CLOSED;	// Current position has now been opened and explored / 검토는 아래부분에서 한다. __ 논리적으로는 아래에 위치하는게 맞음
    nMoves++; 					
    if(mazeMap[cy][cx] == GOAL) { //goal에 도달하면	       
      closedDeque.push_back(openDeque.front()); // 꼬리부분을 삽입
      Point *p = openDeque.front();
      cout << "closedDeque.push_back(" << p->cy << "," << p->cx << "," << p->getDepth() << ")" << endl;
      cout << "openDeque.pop_front(" << p->cy << "," << p->cx << "," << p->getDepth() << ")" << endl;
      openDeque.pop_front(); // 머리에 등록하면 깊이우선탐색, 꼬리에 등록하면 너비우선탐색이 된다.
      found = true;
      break; 					
    } else { 			
      // move the first element from openDeque to closedDeque
      closedDeque.push_back(openDeque.front());// open deque의 가장 마지막에 입력된 원소를 검토한다. 
      Point *p = openDeque.front();
      cout << "closedDeque.push_back(" << p->cy << "," << p->cx << "," << p->getDepth() << ")" << endl;
      cout << "openDeque.pop_front(" << p->cy << "," << p->cx << "," << p->getDepth() << ")" << endl;
      openDeque.pop_front(); 	
      StepNext(cy, cx); // check surrounding positions, 현재좌표를 중심으로 서북동남 좌표를 opendeque에 등록
	}
    cout << endl; PrintRoute(); cout << endl;
    getchar();
  }
  return found;
}


//----------------------------------------------------------//
// Main Function
//----------------------------------------------------------//
#include <string.h>

int main(int argc, char *argv[]) {
  Point start, goal;

  if (argc != 3) {
    cout << "usage: " << argv[0] << " {q|s} mapfile" << endl;
    exit(1);
  }
  
  if (!strcmp(argv[1], "q")) data_struct = 'q';
  else if (!strcmp(argv[1], "s")) data_struct = 's';
  else exit(1);
  
  InitMaps(argv[2], start, goal);
  bool found = FindRoute(start, goal);// 모든 경로를 탐색했을 때
  if (!found) {// 못찾았을 때
    cout << "This maze has no solution!" << endl;
    return 1;
  }// 찾을 수 있는 경로 중에 하나를 보여줌, 최단 경로를 보여주진 않는다.
  // 최단경로를 확률적으로 결정 -> A-STAR 를 이

  PrintMap();
  //ClearMaps();
  FreeMaps();
  return 0;
}
