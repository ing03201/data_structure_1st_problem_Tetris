#include <iostream> 
#include <string> 
//#include "Vertex.h" 
//#include "Edge.h" 
#include "Graph.h" 
#include "BFS.h" 
#define NUM_NODES 16 
#define NUM_EDGES 50 

typedef Graph::Vertex Vertex; 
typedef Graph::Edge Edge; 
typedef std::list<Graph::Vertex> VtxList; 
typedef std::list<Graph::Edge> EdgeList; 
typedef std::list<Graph::Vertex>::iterator VtxItor; 
typedef std::list<Graph::Edge>::iterator EdgeItor; 

int main() { 
  Vertex v[NUM_NODES] = { // Topology of Figure 13.6 (p. 608)   
    Vertex("A",  0, UN_VISITED),       Vertex("B",  1, UN_VISITED),  
    Vertex("C",  2, UN_VISITED),       Vertex("D",  3, UN_VISITED), 
    Vertex("E",  4, UN_VISITED),       Vertex("F",  5, UN_VISITED), 
    Vertex("G",  6, UN_VISITED),       Vertex("H",  7, UN_VISITED), 
    Vertex("I",  8, UN_VISITED),       Vertex("J",  9, UN_VISITED), 
    Vertex("K", 10, UN_VISITED),       Vertex("L", 11, UN_VISITED), 
    Vertex("M", 12, UN_VISITED),       Vertex("N", 13, UN_VISITED), 
    Vertex("0", 14, UN_VISITED),       Vertex("P", 15, UN_VISITED) 
  };

  Graph::Edge edges[NUM_EDGES] = {
    Edge(v[0], v[1], 10),    Edge(v[1], v[0], 10), 
    Edge(v[0], v[4], 10),    Edge(v[4], v[0], 10), 
    Edge(v[0], v[5], 15),    Edge(v[5], v[0], 15), 
    Edge(v[1], v[2], 10),    Edge(v[2], v[1], 10), 
    Edge(v[1], v[5], 10),    Edge(v[5], v[1], 10), 
    Edge(v[2], v[3], 10),    Edge(v[3], v[2], 10), 
    Edge(v[2], v[6], 10),    Edge(v[6], v[2], 10), 
    Edge(v[3], v[6], 15),    Edge(v[6], v[3], 15), 
    Edge(v[3], v[7], 10),    Edge(v[7], v[3], 10), 
    Edge(v[4], v[5], 10),    Edge(v[5], v[4], 10), 
    Edge(v[4], v[8], 10),    Edge(v[8], v[4], 10), 
    Edge(v[5], v[8], 15),    Edge(v[8], v[5], 15), 
    Edge(v[6], v[9], 15),    Edge(v[9], v[6], 15), 
    Edge(v[6], v[10], 10),   Edge(v[10], v[6], 10), 
    Edge(v[6], v[11], 15),   Edge(v[11], v[6], 15), 
    Edge(v[7], v[11], 10),   Edge(v[11], v[7], 10), 
    Edge(v[8], v[9], 10),    Edge(v[9], v[8], 10), 
    Edge(v[8], v[12], 10),   Edge(v[12], v[8], 10), 
    Edge(v[8], v[13], 15),   Edge(v[13], v[8], 15), 
    Edge(v[9], v[10], 10),   Edge(v[10], v[9], 10), 
    Edge(v[10], v[13], 15),  Edge(v[13], v[10], 15), 
    Edge(v[10], v[14], 10),  Edge(v[14], v[10], 10), 
    Edge(v[11], v[15], 10),  Edge(v[15], v[11], 10), 
    Edge(v[12], v[13], 10),  Edge(v[13], v[12], 10), 
    Edge(v[14], v[15], 10),  Edge(v[15], v[14], 10) 
  };
  Graph simpleGraph(NUM_NODES);

  cout << "Inserting vertices .." << endl; 
  for (int i=0; i<NUM_NODES; i++) { 
    simpleGraph.insertVertex(v[i]); 
  } 

  VtxList vtxLst; 
  simpleGraph.vertices(vtxLst); 

  int count = 0; 
  cout << "Inserted vertices: "; 
  for (VtxItor vItor = vtxLst.begin(); vItor != vtxLst.end(); ++vItor) { 
    cout << *vItor << "    "; 
  } 
  cout << endl; 

  cout << "Inserting edges .." << endl; 
  for (int i=0; i<NUM_EDGES; i++) { 
      simpleGraph.insertEdge(edges[i]); 
  }

  cout << "Inserted edges: " << endl; 
  count = 0; 
  EdgeList egLst; 
  simpleGraph.edges(egLst); 

  for (EdgeItor p = egLst.begin(); p != egLst.end(); ++p) { 
    count++; 
    cout << *p << ", "; 
    if (count % 5 == 0) 
      cout << endl; 
  } 
  cout << endl; 

  cout << "Print out Graph based on Adjacency List .." << endl; 
  simpleGraph.printGraph(); 
  /* ==========================================*/ 
  cout << "\nTesting Breadth First Search" << endl; 
  BreadthFirstSearch bfsGraph(simpleGraph); 
  //bfsGraph.initDistMtrx(); 
  //bfsGraph.printDistMtrx();
  VtxList path;
  bfsGraph.findShortestPath(v[0], v[15], path); 
  cout << "Path found by BFS (shortest) from " << v[0] << " to " << v[15] << " : "; 
  for (VtxItor vItor = path.begin(); vItor != path.end(); ++vItor) 
    cout << *vItor << "    "; 
  cout << endl; 

  return 0;
}
