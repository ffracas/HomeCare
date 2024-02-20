#ifndef KRUSKALGRAPH_HPP
#define KRUSKALGRAPH_HPP

#include <vector>
#include <iostream>

namespace homecare {

#define edge std::pair<int, int>

class KruskalGraph {
private:
  std::vector<std::pair<int, edge> > G;     // graph
  std::vector<std::pair<int, edge> > T;     // mst
  int *parent;
  int N;                                    // number of nodes in graph
  
  void addIfNotPresent(std::vector<int>&, int);

public:
  KruskalGraph(int);
  void addWeightedEdge(int, int, int);
  int find_set(int);
  void mergeSet(int, int);
  void generateKruskalTree();
  void print();
  std::vector<int> getKruskalRank(int);
};
    
} 


#endif