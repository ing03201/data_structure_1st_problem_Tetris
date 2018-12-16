#pragma once

#include <list>
#include <vector>
#include <iostream>
#include <iomanip>
#include <climits>
#include <string>

#define PLUS_INF INT_MAX/2
enum VertexStatus {UN_VISITED, VISITED, VRTX_NOT_FOUND}; // vertex 상태에 세가지 종류, 미방문, 방문, 리스트에 없을때
enum EdgeStatus {DISCOVERY, BACK, CROSS, EDGE_UN_VISITED, EDGE_VISITED, EDGE_NOT_FOUND}; 

using namespace std;  

class Graph { // Graph based on Adjacency Matrix 
 public:      
  class Vertex;      
  class Edge;
 public:      
  class Vertex {                
    friend ostream& operator<<(ostream& fout, Vertex v) {
      fout << v.getName(); 
      return fout;           
    }      
 public:           
  Vertex() : name(), ID(-1) {}
  Vertex(string n, int id, VertexStatus vs) : name(n), ID(id), vtxStatus(vs) { }
  Vertex(char *pN, int id, VertexStatus vs) : name(string(pN)), ID(id), vtxStatus(vs) { } 
  Vertex(int id) : ID(id) {}  
    string getName() { return name;} 
    int getID() {return ID;} 
    void setName(string c_name) { name = c_name;}      
    void setID(int id) { ID = id;}    
    bool operator==(Vertex v) {return ((ID == v.getID()) && (name == v.getName())); }   
    bool operator!=(Vertex v) {return ((ID != v.getID()) || (name != v.getName())); }          
    void setVtxStatus(VertexStatus vs) { vtxStatus = vs;}          
    VertexStatus getVtxStatus() { return vtxStatus; } 
 private:           
    string name; 
    int ID;    
    VertexStatus vtxStatus; 
 };  // end class Vertex public:  typedef std::list<Graph::Vertex> VtxList; 
 public:      
  typedef std::list<Vertex> VtxList; 

 public:
  class Edge {           
    friend ostream& operator<<(ostream& fout, Edge& e) {                
      fout << "Edge(" << e.getVertex_1() << ", " << e.getVertex_2();     
      fout  << ", d(" << setw(2) << e.getDistance() << "))";             
      return fout;          
    }
    bool operator>(Edge E1){ return this->getDistance() > E1.getDistance();}   
  public:  
  Edge() : pVrtx_1(NULL), pVrtx_2(NULL), distance(PLUS_INF) {}    
  Edge(Vertex v1, Vertex v2, int d) : vrtx_1(v1), vrtx_2(v2), distance(d), pVrtx_1(NULL), pVrtx_2(NULL), edgeStatus(EDGE_UN_VISITED) { } 
    void endVertices(VtxList& vtxList) {                
      vtxList.push_back(*pVrtx_1);    
      vtxList.push_back(*pVrtx_2);      
    }        
    Vertex opposite(Vertex v) {               
      if (v == *pVrtx_1)  
      	return *pVrtx_2;  
      else if (v == *pVrtx_2)   
	      return *pVrtx_1;             
      else {                     
	      //cout << "Error in opposite()" << endl;  
	      return Vertex(NULL);              
      }        
    }          
    Vertex getVertex_1() {return vrtx_1;}      
    Vertex getVertex_2() {return vrtx_2;}      
    Vertex* getpVrtx_1() {return pVrtx_1;}     
    Vertex* getpVrtx_2() {return pVrtx_2;}     
    int getDistance() {return distance;}       
    void setpVrtx_1(Vertex* pV) {pVrtx_1 = pV;}  
    void setpVrtx_2(Vertex* pV) {pVrtx_2 = pV;}   
    void setDistance(int d) {distance = d;}       
    bool operator!=(Edge e) { return ((*pVrtx_1 != e.getVertex_1()) || (*pVrtx_2 != e.getVertex_2()));} 
    bool operator==(Edge e) { return ((*pVrtx_1 == e.getVertex_1()) && (*pVrtx_2 == e.getVertex_2()));}       
    void setEdgeStatus(EdgeStatus es) {edgeStatus = es;}          
    EdgeStatus getEdgeStatus() {return edgeStatus;}
    bool isIncidentOn(Vertex V) { return ((V == getVertex_1()) || (V == getVertex_2())); }

  private:
    Vertex vrtx_1;      
    Vertex vrtx_2;       
    Vertex* pVrtx_1;        
    Vertex* pVrtx_2;        
    int distance;        
    EdgeStatus edgeStatus; 
  };
 public:        
  //typedef std::list<Graph::Edge> EdgeList;
  //typedef std::list<Vertex> VtxList; 
  typedef std::list<Edge> EdgeList;    
  typedef std::list<Vertex>::iterator VtxItor;
  typedef std::list<Edge>::iterator EdgeItor; 
  Graph() : pVrtxArray(NULL), pAdjLstArray(NULL) {}  // default constructor 
  Graph(int num_nodes) : pVrtxArray(NULL), pAdjLstArray(NULL) {
    typedef Edge* EdgePtr; 
    num_vertices = num_nodes;          
    pVrtxArray = new Graph::Vertex[num_vertices];    
    for (int i=0; i<num_nodes; i++) {             
      pVrtxArray[i] = NULL;    
    }
    pAdjLstArray = new EdgeList[num_vertices];    
    for (int i=0; i<num_vertices; i++) {          
      pAdjLstArray[i].clear();      
    }  
  }      
  void vertices(VtxList& vtxLst);  
  void edges(EdgeList&);     
  bool isAdjacentTo(Vertex v, Vertex w);   
  void insertVertex(Vertex& v);     
  void insertEdge(Edge& e);  
  void eraseEdge(Edge e);    
  void eraseVertex(Vertex v);  
  int getNumVertices() { return num_vertices; }   
  void incidentEdges(Vertex v, EdgeList& edges); 
  Vertex* getpVrtxArray() { return pVrtxArray;} 
  EdgeList* getpAdjLstArray() { return pAdjLstArray;} 
  void printGraph(); 
 private:     
  Vertex* pVrtxArray;   
  EdgeList* pAdjLstArray;   
  int num_vertices;
};
