#ifndef CLUSTERREMOVAL_HPP
#define CLUSTERREMOVAL_HPP

#include "noderemoval.hpp"
#include "relatedremoval.hpp"
#include "../optimisation_structures/kruskalgraph.hpp"

namespace homecare {

// Creating shortcut for an integer pair 
typedef std::pair<int, int> iPair; 

class ClusterRemoval : public NodeRemoval {
private:
    static const int MIN_N_NODES;
public:
    ClusterRemoval(ALNSOptimisation&);
    ~ClusterRemoval();
    void removeNodes(int) override;
    void generateGraph(int, KruskalGraph &);
};

}

#endif