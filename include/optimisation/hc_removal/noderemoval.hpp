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
protected:
    ALNSOptimisation& m_removalOps;

    int chooseRandomRoute();
    int chooseRandomNode(int);

public:
    NodeRemoval(ALNSOptimisation&);
    virtual ~NodeRemoval() {}
    virtual void removeNodes(int) = 0;
};

} 


#endif