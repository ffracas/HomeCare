#ifndef NODEREPAIR_HPP
#define NODEREPAIR_HPP

#include <sstream>

#include "../optimisation_structures/alnsoptimisation.hpp"
#include "../optimisation_structures/costcoord.hpp"

namespace homecare {

class NodeRepair {
protected:
    const static int NOT_ASSIGNED;
    ALNSOptimisation* m_data;

public:
    NodeRepair();
    ~NodeRepair();
    virtual int repairNodes() = 0;
};

}

#endif