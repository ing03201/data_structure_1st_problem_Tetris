#pragma once

#include "Graph.h"
using namespace std; 
 
class BreadthFirstSearch 
{ 
 protected: 
  typedef Graph::Vertex Vertex; 
  typedef Graph::Edge Edge; 
  typedef std::list<Graph::Vertex> VertexList; 
  typedef std::list<Graph::Edge> EdgeList; 
 protected: 
  Graph& graph; 
  Vertex start; 
  bool done;     // flag of search done 
  int **ppDistMtrx;  // distance matrix
	 
 protected: 
  void initialize(); 
  void bfsTraversal(Vertex& v, Vertex& target, VertexList& path); 
  virtual bool isDone() const { return done;} 
 
  // marking utilities 
  void visit(Vertex& v);   
  void visit(Edge& e);  
  void unvisit(Vertex& v); 
  void unvisit(Edge& e); 
  bool isVisited(Vertex& v); 
  bool isVisited(Edge& e); 
  void setEdgeStatus(Edge& e, EdgeStatus es);  
  EdgeStatus getEdgeStatus(Edge& e); 
	
 public: 
  BreadthFirstSearch(Graph& g) :graph(g) { 
    int num_nodes; 
    num_nodes = g.getNumVertices(); 
 
    // initialize DistMtrx 
    for (int i=0; i<num_nodes; i++) 
      ppDistMtrx = new int*[num_nodes]; 
    for (int i=0; i<num_nodes; i++) 
      ppDistMtrx[i] = new int[num_nodes]; 
 
    for (int i=0; i<num_nodes; i++) { 
      for (int j=0; j<num_nodes; j++)  { 
	ppDistMtrx[i][j] = PLUS_INF; 
      } 
    } 
   } 
  void initDistMtrx(); 
  void printDistMtrx(); 
  void findShortestPath(Vertex& s, Vertex& t, VertexList& path); 
  Graph& getGraph() {return graph;} 
  int** getppDistMtrx() { return ppDistMtrx;} 
};
