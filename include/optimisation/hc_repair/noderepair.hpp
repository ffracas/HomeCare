#ifndef NODEREPAIR_HPP
#define NODEREPAIR_HPP

#include "../optimisation_structures/alnsoptimisation.hpp"
#include "../optimisation_structures/infonode.hpp"
#include "../optimisation_structures/costcoord.hpp"
#include "../optimisation_structures/routesopt.hpp"

namespace homecare {

class NodeRepair {
protected:
    ALNSOptimisation& m_repairOps;
    const static int NOT_ASSIGNED;

public:
    NodeRepair(ALNSOptimisation&);
    ~NodeRepair();
    virtual int repairNodes() = 0;
};

}

#endif