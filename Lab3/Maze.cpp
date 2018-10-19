//----------------------------------------------------------//
// Taken from https://github.com/steffanc/MazeBot
// Edited by khkim for readability
//----------------------------------------------------------//
#include <cstdlib>
#include <iostream>
#include <math.h>
#include <string>
#include <fstream>
//#define __STL_DEQUE__
#ifdef __STL_DEQUE__
#include <deque> // Standard Template Library
#else
	// taken from "Data Structures and Algorithms in C++ (2nd Edition)"
#include "LinkedDeque.h"  
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
  Point *parent;
public:
  int cy, cx;   
  int depth;	
  Point(): cy(0), cx(0), depth(0), parent(0) { }
  Point(int cy, int cx, int depth, Point * parent) {	
    this->cy = cy;
    this->cx = cx;
    this->depth = depth;
    this->parent = parent;
  }
  ~Point() { delete parent; }
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
  ifstream mapFile;
  mapFile.open(fname, ios::in);
  if (!mapFile.is_open()) {
    cout << "Unable to open file";
    exit(1);
  }

  mapFile >> mapHeight;
  mapFile >> mapWidth;

  mazeMap = new int*[mapHeight];
  mazeRoute = new int*[mapHeight];
  for (int y = 0; y < mapHeight; y++) {
    mazeMap[y] = new int[mapWidth];
    mazeRoute[y] = new int[mapWidth];
  }
  
  for (int y = 0; y < mapHeight; y++) {
    for (int x = 0; x < mapWidth; x++) {
      mapFile >> mazeMap[y][x];
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


void StepNext_w_Queue(int cy, int cx) {
  int depth = (closedDeque.back()->depth)+1;
  Point *parent = closedDeque.back();
  if (never_visited(mazeRoute[cy][cx-1])) {
    mazeRoute[cy][cx-1] = OPEN;
    openDeque.push_back(new Point(cy, cx-1, depth, parent));
    cout << "openDeque.push_back(" << cy << "," << cx-1 << "," << depth << ")" << endl;
  }
  if (never_visited(mazeRoute[cy-1][cx])) {
    mazeRoute[cy-1][cx] = OPEN;
    openDeque.push_back(new Point(cy-1, cx, depth, parent));
    cout << "openDeque.push_back(" << cy-1 << "," << cx << "," << depth << ")" << endl;
  }
  if (never_visited(mazeRoute[cy][cx+1])) {
    mazeRoute[cy][cx+1] = OPEN;
    openDeque.push_back(new Point(cy, cx+1, depth, parent));
    cout << "openDeque.push_back(" << cy << "," << cx+1 << "," << depth << ")" << endl;
  }
  if (never_visited(mazeRoute[cy+1][cx])) {
    mazeRoute[cy+1][cx] = OPEN;
    openDeque.push_back(new Point(cy+1, cx, depth, parent));
    cout << "openDeque.push_back(" << cy+1 << "," << cx << "," << depth << ")" << endl;
  }
  maxOpenQSize = (openDeque.size() > maxOpenQSize) ? openDeque.size() : maxOpenQSize;
}

void StepNext_w_Stack(int cy, int cx) {
  int depth = (closedDeque.back()->depth)+1;
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

bool FindRoute(const Point& start, const Point& goal) {
  int cy = 0, cx = 0;
  bool found = false; 
  maxOpenQSize = 1;
  mazeRoute[start.cy][start.cx] = OPEN;
  openDeque.push_back(new Point(start.cy, start.cx, 0, 0));
  cout << "openDeque.push_back(" << start.cy << "," << start.cx << "," << 0 << ")" << endl;

  PrintRoute();
	
  // Keep searching until an goal is determined or no solution is found
  while(openDeque.size() != 0) {
    cy = openDeque.front()->cy;
    cx = openDeque.front()->cx;
    mazeRoute[cy][cx] = CLOSED;	// Current position has now been opened and explored
    nMoves++; 					
    if(mazeMap[cy][cx] == GOAL) { 	       
      closedDeque.push_back(openDeque.front());
      Point *p = openDeque.front();
      cout << "closedDeque.push_back(" << p->cy << "," << p->cx << "," << p->depth << ")" << endl;
      cout << "openDeque.pop_front(" << p->cy << "," << p->cx << "," << p->depth << ")" << endl;
      openDeque.pop_front();
      found = true;
      break; 					
    } else { 			
      // move the first element from openDeque to closedDeque
      closedDeque.push_back(openDeque.front());
      Point *p = openDeque.front();
      cout << "closedDeque.push_back(" << p->cy << "," << p->cx << "," << p->depth << ")" << endl;
      cout << "openDeque.pop_front(" << p->cy << "," << p->cx << "," << p->depth << ")" << endl;
      openDeque.pop_front(); 	
      StepNext(cy, cx); // check surrounding positions
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
  bool found = FindRoute(start, goal);
  if (!found) {
    cout << "This maze has no solution!" << endl;
    return 1;
  }

  PrintMap();
  //ClearMaps();
  FreeMaps();
  return 0;
}
