#ifndef CLUSTERREMOVAL_HPP
#define CLUSTERREMOVAL_HPP

#include <vector>
#include <iostream>
#include <string>

#include "noderemoval.hpp"
#include "../optimisation_structures/kruskalgraph.hpp"

namespace homecare {

class ClusterRemoval : public NodeRemoval {
private:
    //const int m_minClusterDim = 7;
public:
    ClusterRemoval();
    ~ClusterRemoval();
    void removeNodes(int) override;
    void generateGraph(ScheduleOptimiser&, int, KruskalGraph &);
};

}

#endif