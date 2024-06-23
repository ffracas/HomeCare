#ifndef KRUSKALGRAPH_HPP
#define KRUSKALGRAPH_HPP

#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <utility>

namespace homecare {

using edge = std::pair<int, int>;

class KruskalGraph {
private:
  std::vector<std::pair<int, edge>> graph;      // graph
  std::vector<std::pair<int, edge>> mst;        // mst
  std::vector<int> parent;
  //int nNodes;                                   // number of nodes in graph
  
  void addIfNotPresent(std::vector<int>&, int);

public:
  KruskalGraph(int);
  void addWeightedEdge(int, int, int);
  int findSet(int);
  void mergeSet(int, int);
  void generateKruskalTree();
  void print();
  std::vector<int> getKruskalRank(int);
};
    
} 


#endif