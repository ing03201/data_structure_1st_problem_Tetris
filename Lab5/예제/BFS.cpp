#include <iostream> 
#include <iomanip> 
#include <list> 
#include <algorithm> 
#include <queue>
#include "Graph.h" 
#include "BFS.h" 
 
using namespace std; 
 
typedef Graph::Vertex Vertex; 
typedef Graph::Edge Edge; 
typedef std::list<Graph::Vertex> VertexList; 
typedef std::list<Graph::Vertex>::iterator VertexItor; 
typedef std::list<Graph::Edge> EdgeList; 
typedef std::list<Graph::Edge>::iterator EdgeItor; 
 
//template<typename G> 
void BreadthFirstSearch::initialize() { 
  VertexList verts; 
  graph.vertices(verts); 
  for (VertexItor pv = verts.begin(); pv != verts.end(); ++pv) 
    { 
      unvisit(*pv); 
    }
  EdgeList edges; 
  graph.edges(edges); 
  for (EdgeItor pe = edges.begin(); pe != edges.end(); ++pe) 
    unvisit(*pe); 
 
  done = false; 
} 
 
void BreadthFirstSearch::initDistMtrx() { 
  int** ppDistMtrx; 
  int* pLeaseCostMtrx; 
  int num_nodes; 
  Vertex* pVrtxArray; 
  EdgeList* pAdjLstArray; 
  int curVrtx_ID, vrtxID; 
 
  num_nodes = graph.getNumVertices(); 
  pVrtxArray = graph.getpVrtxArray(); 
  pAdjLstArray = graph.getpAdjLstArray(); 
      
  ppDistMtrx = getppDistMtrx(); 
  for (int i=0; i<num_nodes; i++)  { 
    curVrtx_ID = pVrtxArray[i].getID(); 
    EdgeItor pe = pAdjLstArray[curVrtx_ID].begin(); 
    while (pe != pAdjLstArray[curVrtx_ID].end())   { 
      vrtxID = ((*pe).getVertex_2()).getID(); 
      ppDistMtrx[curVrtx_ID][vrtxID] = (*pe).getDistance(); 
      pe++; 
    } 
    ppDistMtrx[curVrtx_ID][curVrtx_ID] = 0; 
  } 
} 
 
void BreadthFirstSearch::printDistMtrx() { 
  int** ppDistMtrx; 
  int num_nodes; 
  int dist; 
 
  num_nodes = graph.getNumVertices(); 
  ppDistMtrx = getppDistMtrx(); 
 
  cout << "    |"; 
  for (int i=0; i<num_nodes; i++) { 
    cout << setw(5) << (char)( i + 'A'); 
  } 
  cout << endl;
  cout << "----+"; 
  for (int i=0; i<num_nodes; i++) { 
    cout << "-----"; 
  } 
  cout << endl; 
 
  for (int i=0; i<num_nodes; i++) { 
    cout << setw(2) << (char)( i + 'A') << "  |"; 
    for (int j=0; j<num_nodes; j++) { 
      dist = ppDistMtrx[i][j]; 
      if (dist == PLUS_INF) 
	cout << "  +oo"; 
      else 
	cout << setw(5) << dist; 
    } 
    cout << endl; 
  } 
  cout << endl; 
}


// HomeWork 
/*
  1. 거리 배열과 경로배열을 -1로 초기화
  2. 현재 노드에서 연결된 노드들을 우선순위 큐에 넣는다. 우선순위 큐이므로 최단거리로 정렬됨.(시작점일 경우 시작노드를 큐에 넣음)
  3. 현재 노드와 연결된 노드들 사이의 최단거리를 찾아서 거리 배열과 경로배열을 채운다.
  4. (3) ~ (4) 을 모든 노드를 방문할 때 까지 반복한다.
*/
/* Graph 헤더 파일에 구현되어있는 함수
bool operator>(Edge E1){
  return this->getDistance() > E1.getDistance();
}
*/
enum BFS_PROCESS_STATUS {NOT_SELECTED, SELECTED}; 
void BreadthFirstSearch::bfsTraversal(Vertex& s, Vertex& target, VertexList& path) { 
  int** ppDistMtrx; // 거리배열
  int* pLeastCost; // 최소 비용 배열
  int num_nodes, num_selected; 
  int* pPrev; // 이전 
  int minID, minCost; // 최소 id 최소 비용
  BFS_PROCESS_STATUS* pBFS_Process_Stat; // BFS 과정 상태

  Vertex* pVrtxArray;  // vertex array
  Vertex vrtx, *pPrevVrtx, v; // vertex, 이전 노드, 현재노드 
  Edge e; 
  int start_vrtxid, target_vrtxid, curVrtx_ID, vrtxID;  // 시작 vertex id, 끝 vertex id , 현재 vertex id, vertex id 
  EdgeList* pAdjLstArray; 

<<<<<<< HEAD
  priority_queue<Edge, int> PQ;

=======
>>>>>>> master
  pVrtxArray = graph.getpVrtxArray();  // Vertex 
  pAdjLstArray = graph.getpAdjLstArray(); 
  start_vrtxid = start.getID(); // 시작 노드
  target_vrtxid = target.getID(); // 끝 노드 
 
  num_nodes = graph.getNumVertices(); // 노드 갯수 
  ppDistMtrx = getppDistMtrx(); // 거리 행렬
  
  
  pLeastCost = new int[num_nodes]; // 최단경로 거리 
  pPrev = new int[num_nodes];  // 
  pBFS_Process_Stat = new BFS_PROCESS_STATUS[num_nodes]; // BFS 프로세스 상태 배열
  // initialize L(n) = w(start, n); 
  // 시작점 처리
  for (int i=0; i< num_nodes; i++) { // 시작점 처리 
    pLeastCost[i] = ppDistMtrx[start_vrtxid][i]; 
    pPrev[i] = start_vrtxid; 
    pBFS_Process_Stat[i] = NOT_SELECTED; 
  } 
  pBFS_Process_Stat[start_vrtxid] = SELECTED; 
  num_selected = 1; 
      
  
  int round = 0; 
 
  while (num_selected < num_nodes) { // 노드 갯수가 선택한 노드의 갯수보다 작을 때까지 루프 돌리기
    round++; 
    cout << "=== round " << round << " ==== " << endl;  
    // find current node with LeastCost 
    minID = -1; 
    minCost = PLUS_INF; 
    for (int i=0; i<num_nodes; i++)  { 
      if ((pLeastCost[i] < minCost) && (pBFS_Process_Stat[i] != SELECTED)) { 
	      minID = i; 
	      minCost = pLeastCost[i]; 
      } 
    }
    //현재 vertex리스트를 이용하여 최소 코스트를 출력한다.
	  cout << "Vertex (" << pVrtxArray[minID] << ") with least cost = " << minCost << endl;
	
    if (minID == -1) { // 연결된 노드가 하나도 없을 때 
      cout << "Error in FindShortestPath() -- target is not connected to the start !!" << endl; 
      break; 
    } 
    else { // 연결된 노드가 있을 때
      pBFS_Process_Stat[minID] = SELECTED; // 첫 노드의 방문상태를 선택상태로 한다. 
      num_selected++; // 선택된 노드의 갯수가 늘어난다.
      if (minID == target_vrtxid) { // 최소 노드의 이름이 도착점의 이름과 같다면 
	      cout << "reached to the target node !!" << endl; 
      	cout << "Least Cost = " << minCost << endl; 
<<<<<<< HEAD
      	vrtxID = minID; // 현재노드의 이름을 최소 노드이름으로 한다.
=======
      	vrtxID = minID; // 노드의 이름을 최소 노드이름으로 한다.
>>>>>>> master
    	  do { // 시작노드의 이름과 현재 노드의 이름이 틀릴 때까지
  	      vrtx = pVrtxArray[vrtxID]; // 
	        path.push_front(vrtx); 
	        vrtxID = pPrev[vrtxID]; 
        } while (vrtxID != start_vrtxid); 
	      vrtx = pVrtxArray[vrtxID]; 
	      path.push_front(vrtx); // start node 
	      break; 
      } 
      ///
    }
		   
    //int pLS, ppDistMtrx_i; 
    for (int i=0; i<num_nodes; i++) { 
      //pLS = pLeastCost[i]; 
      //ppDistMtrx_i = ppDistMtrx[minID][i];  
      if ( (pBFS_Process_Stat[i] != SELECTED) && (pLeastCost[i] > (pLeastCost[minID] + ppDistMtrx[minID][i]))) { 
      	pPrev[i] = minID; 
      	pLeastCost[i] = pLeastCost[minID] + ppDistMtrx[minID][i]; 
      } 
    } 
 
    // print out the pLeastCost[] for debugging 
    // 디버깅용 파트
    for (int i=0; i<num_nodes; i++) { 
      cout << (char)(i+'A') << setw(5);
      if(pLeastCost[i] != PLUS_INF) {
      	cout << pLeastCost[i] << setw(1) << ", "; 
      }
      else {
      	cout << "+oo" << setw(1) << ", ";
      }
    } 
    cout << endl; 
  }  // end while() 
 // 디버깅할 때 출력하는 부분
}

void BreadthFirstSearch::findShortestPath(Vertex &s, Vertex &target, VertexList& path) { 
  initialize(); 
  path.clear(); 
 
  start = s; 
  initDistMtrx(); 
  printDistMtrx(); 
  bfsTraversal(start, target, path); 
} 
 
void BreadthFirstSearch::visit(Vertex& v)  {  
  Graph::Vertex* pVtx; 
 
  int vtx_ID = v.getID(); 
  int numNodes = getGraph().getNumVertices(); 
      
 
  if (vtx_ID >= 0 && vtx_ID < numNodes) { 
    pVtx = getGraph().getpVrtxArray(); 
    pVtx[vtx_ID].setVtxStatus(VISITED); 
  } 
  else 
    cout << "Vertex (" << v << ") ID is out-of-range (" << numNodes << ")" << endl; 
}

void BreadthFirstSearch::visit(Edge& e)  { 
  EdgeItor eItor; 
  Graph::EdgeList* pEdgeLst; 
 
  Vertex vtx_1, vtx_2; 
  int vtx_1_ID, vtx_2_ID; 
  int numNodes = getGraph().getNumVertices(); 
 
  vtx_1 = e.getVertex_1(); 
  vtx_1_ID = vtx_1.getID(); 
 
  if (vtx_1_ID >= 0 && vtx_1_ID < numNodes) { 
    pEdgeLst = getGraph().getpAdjLstArray(); 
    eItor = find(pEdgeLst[vtx_1_ID].begin(), pEdgeLst[vtx_1_ID].end(), e); 
    if (eItor == pEdgeLst[vtx_1_ID].end()) { 
      cout << "Edge (" << e << " not found from the graph !!" << endl; 
    } else { 
      (*eItor).setEdgeStatus(EDGE_VISITED); 
    } 
  } else {
    cout << "Vertex (" << vtx_1 << ") ID of Edge (" ; 
	cout << e << ") is out-of-range (" << numNodes << ")" << endl; 
  }
}

void BreadthFirstSearch::unvisit(Vertex& v)  {  
  int vtx_ID = v.getID(); 
  int numNodes = getGraph().getNumVertices(); 
  Vertex* pVtx = getGraph().getpVrtxArray(); 
 
  if (vtx_ID >= 0 && vtx_ID < numNodes) 
    pVtx[vtx_ID].setVtxStatus(UN_VISITED); 
  else 
    cout << "Vertex (" << v << ") ID is out-of-range (" << numNodes << ")" << endl; 
} 
 
void BreadthFirstSearch::unvisit(Edge& e)  { 
  EdgeItor eItor; 
 
  Vertex vtx_1, vtx_2; 
  int vtx_1_ID, vtx_2_ID; 
  int numNodes = getGraph().getNumVertices(); 
  EdgeList* pAdjLstArray; 
      
  vtx_1 = e.getVertex_1(); 
  vtx_1_ID = vtx_1.getID();
	  
  if (vtx_1_ID >= 0 && vtx_1_ID < numNodes) { 
    pAdjLstArray = getGraph().getpAdjLstArray(); 
    eItor = find(pAdjLstArray[vtx_1_ID].begin(), pAdjLstArray[vtx_1_ID].end(), e); 
    if (eItor == pAdjLstArray[vtx_1_ID].end())  { 
      cout << "Edge (" << e << " not found from the graph !!" << endl; 
    } else { 
      (*eItor).setEdgeStatus(EDGE_UN_VISITED); 
    } 
  } else {
    cout << "Vertex (" << vtx_1 << ") ID of Edge (" ; 
	cout << e << ") is out-of-range (" << numNodes << ")" << endl; 
  }
} 
 
bool BreadthFirstSearch::isVisited(Vertex& v)  {  
  Vertex* pVtx; 
 
  int vtx_ID = v.getID(); 
  int numNodes = getGraph().getNumVertices(); 
  Vertex* pVrtxArray; 
 
  if (vtx_ID >= 0 && vtx_ID < numNodes) { 
    pVrtxArray = getGraph().getpVrtxArray(); 
    pVtx = &(pVrtxArray[vtx_ID]); 
    return (pVtx->getVtxStatus() == VISITED);  
  } else { 
    cout << "Vertex (" << v << ") ID is out-of-range (" << numNodes << ")" << endl; 
    return false; 
  } 
}

bool BreadthFirstSearch::isVisited(Edge& e)  {  
  EdgeItor eItor; 
  Vertex vtx_1, vtx_2; 
  int vtx_1_ID, vtx_2_ID; 
  int numNodes = getGraph().getNumVertices(); 
  EdgeList* pAdjLstArray; 
  EdgeStatus eStat; 
      
  vtx_1 = e.getVertex_1(); 
  vtx_1_ID = vtx_1.getID(); 
  if (vtx_1_ID >= 0 && vtx_1_ID < numNodes) { 
    pAdjLstArray = getGraph().getpAdjLstArray(); 
    eItor = find(pAdjLstArray[vtx_1_ID].begin(), pAdjLstArray[vtx_1_ID].end(), e); 
    if (eItor == pAdjLstArray[vtx_1_ID].end())  { 
      cout << "Edge (" << e << " not found from the graph !!" << endl; 
      return false; 
    } else { 
      eStat = (*eItor).getEdgeStatus(); 
      if ((eStat == EDGE_VISITED) || (eStat == DISCOVERY) || (eStat == BACK)) 
	return true; 
      else  
	return false; 
    } 
  } else { 
    cout << "Vertex (" << vtx_1 << ") ID of Edge (" << e ; 
    cout << ") is out-of-range (" << numNodes << ")" << endl; 
  } 
  return false;  
}

void BreadthFirstSearch::setEdgeStatus(Edge& e, EdgeStatus es) { 
  EdgeItor eItor; 
  int vtx1id = e.getVertex_1().getID(); 
  int vtx2id = e.getVertex_1().getID(); 
 
  EdgeList* pAdjLstArray; 
  pAdjLstArray = getGraph().getpAdjLstArray(); 
 
  eItor = pAdjLstArray[vtx1id].begin(); 
  while (eItor != pAdjLstArray[vtx1id].end()) { 
    if (*eItor == e) 
      break; 
    else 
      eItor++; 
  } 
  if (eItor != pAdjLstArray[vtx1id].end()) 
    (*eItor).setEdgeStatus(es); 
  else 
    cout << "Edge (" << e << ") was not found from AdjacencyList" << endl; 
}

EdgeStatus BreadthFirstSearch::getEdgeStatus(Edge& e) { 
  EdgeItor eItor; 
  int vtx1id = e.getVertex_1().getID(); 
  int vtx2id = e.getVertex_1().getID(); 
  EdgeList* pAdjLstArray; 
  pAdjLstArray = getGraph().getpAdjLstArray(); 
 
  eItor = pAdjLstArray[vtx1id].begin(); 
  while (eItor != pAdjLstArray[vtx1id].end()) { 
    if ( *eItor == e) 
      break; 
  } 
  if (eItor != pAdjLstArray[vtx1id].end()) 
    return (*eItor).getEdgeStatus(); 
  else { 
    cout << "Edge (" << e << ") was not found from AdjacencyList" << endl; 
    return EDGE_NOT_FOUND; 
  } 
}
