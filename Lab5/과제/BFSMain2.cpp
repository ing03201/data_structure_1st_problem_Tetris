#include <iostream> 
#include <string> 
//#include "Vertex.h" 
//#include "Edge.h" 
#include "Graph.h" 
#include "BFS.h" 
#define NUM_NODES 9 
#define NUM_EDGES 38 

typedef Graph::Vertex Vertex; 
typedef Graph::Edge Edge; 
typedef std::list<Graph::Vertex> VtxList; 
typedef std::list<Graph::Edge> EdgeList; 
typedef std::list<Graph::Vertex>::iterator VtxItor; 
typedef std::list<Graph::Edge>::iterator EdgeItor; 

int main() { 
  Vertex v[NUM_NODES] = { // Topology of Figure 13.6 (p. 608)   
    Vertex("BWI",  0, UN_VISITED),       Vertex("JFK",  1, UN_VISITED),  
    Vertex("PVD",  2, UN_VISITED),       Vertex("BOS",  3, UN_VISITED), 
    Vertex("ORD",  4, UN_VISITED),       Vertex("MIA",  5, UN_VISITED), 
    Vertex("DFW",  6, UN_VISITED),       Vertex("SFO",  7, UN_VISITED), 
    Vertex("LAX",  8, UN_VISITED)
  };

  Graph::Edge edges[NUM_EDGES] = {
    Edge(v[0], v[1], 184),    Edge(v[1], v[0], 184), 
    Edge(v[0], v[4], 621),    Edge(v[4], v[0], 621), 
    Edge(v[0], v[5], 946),    Edge(v[5], v[0], 946), 
    Edge(v[1], v[2], 144),    Edge(v[2], v[1], 144), 
    Edge(v[1], v[3], 187),    Edge(v[3], v[1], 187), 
    Edge(v[1], v[4], 740),    Edge(v[4], v[1], 740), 
    Edge(v[1], v[5], 1090),   Edge(v[5], v[1], 1090), 
    Edge(v[1], v[6], 1391),   Edge(v[6], v[1], 1391), 
    Edge(v[2], v[4], 849),    Edge(v[4], v[2], 849), 
    Edge(v[3], v[4], 867),    Edge(v[4], v[3], 867), 
    Edge(v[3], v[5], 1258),   Edge(v[5], v[3], 1258), 
    Edge(v[3], v[7], 2704),   Edge(v[7], v[3], 2704), 
    Edge(v[4], v[6], 802),    Edge(v[6], v[4], 802), 
    Edge(v[4], v[7], 1846),   Edge(v[7], v[4], 1846), 
    Edge(v[5], v[6], 1121),   Edge(v[6], v[5], 1121), 
    Edge(v[5], v[8], 2342),   Edge(v[8], v[5], 2342), 
    Edge(v[6], v[7], 1464),   Edge(v[7], v[6], 1464), 
    Edge(v[6], v[8], 1235),   Edge(v[8], v[6], 1235), 
    Edge(v[7], v[8], 337),    Edge(v[8], v[7], 337)
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
  bfsGraph.findShortestPath(v[0], v[8], path); 
  cout << "Path found by BFS (shortest) from " << v[0] << " to " << v[8] << " : "; 
  for (VtxItor vItor = path.begin(); vItor != path.end(); ++vItor) 
    cout << *vItor << "    "; 
  cout << endl; 

  return 0;
}
