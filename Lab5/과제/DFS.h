#pragma once
#include <iostream> 
#include "Graph.h" 
 
using namespace std; 
 
class DepthFirstSearch { 
 protected: 
  typedef Graph::Vertex Vertex; 
  typedef Graph::Edge Edge; 
  typedef std::list<Graph::Vertex> VertexList; 
  typedef std::list<Graph::Edge> EdgeList; 
 protected: 
  Graph& graph; 
  Vertex start; 
  bool done;  // flag of search done 
 protected: 
  void initialize(); 
  void dfsTraversal(Vertex& v, Vertex& target, VertexList& path); 
  virtual void traverseDiscovery(const Edge& e, const Vertex& from) {     } 
  virtual void traverseBack(const Edge& e, const Vertex& from) { } 
  virtual void finishVisit(const Vertex& v) {} 
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
  DepthFirstSearch(Graph& g);  
  void findPath(Vertex& s, Vertex& t, VertexList& path); 
  Graph& getGraph() {return graph;} 
  void showConnectivity(); 
 private: 
  VertexStatus* pVrtxStatus; 
  EdgeStatus** ppEdgeStatus; 
  int** ppConnectivity; 
}; 
