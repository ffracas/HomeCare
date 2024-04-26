#ifndef CLUSTERREMOVAL_HPP
#define CLUSTERREMOVAL_HPP

#include "noderemoval.hpp"
#include "relatedremoval.hpp"
#include "../optimisation_structures/kruskalgraph.hpp"

namespace homecare {

// Creating shortcut for an integer pair 
typedef std::pair<int, int> iPair; 

class ClusterRemoval : public NodeRemoval {
public:
    ClusterRemoval(int);
    ~ClusterRemoval();
    void removeNodes(int) override;
    void generateGraph(ScheduleOptimiser&, int, KruskalGraph &);
};

}

#endif