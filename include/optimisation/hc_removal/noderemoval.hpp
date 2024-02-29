#ifndef NODEREMOVAL_HPP
#define NODEREMOVAL_HPP

#include <vector>
#include <sstream>
#include <cmath>
#include <random>

#include "../../structures/route.hpp"
#include "../optimisation_structures/alnsoptimisation.hpp"
#include "../optimisation_structures/infonode.hpp"
#include "../optimisation_structures/costcoord.hpp"

namespace homecare {
    
class NodeRemoval {
private:
    static const int MIN_ROUTE_LENGTH;

protected:
    ALNSOptimisation& m_removalOps;
    const int MIN_N_NODES;
    static const int NO_INDEX;

    int chooseRandomRoute(RoutesOpt&);
    int chooseRandomNode(RoutesOpt&, int);

public:
    NodeRemoval(ALNSOptimisation&, int = 2);
    virtual ~NodeRemoval() {}
    virtual void removeNodes(int) = 0;
};

} 


#endif