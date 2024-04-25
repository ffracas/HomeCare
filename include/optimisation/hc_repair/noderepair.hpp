#ifndef NODEREPAIR_HPP
#define NODEREPAIR_HPP

#include <sstream>

#include "../optimisation_structures/alnsoptimisation.hpp"
//#include "../optimisation_structures/infonode.hpp" // FIXME: include non-existing file 
#include "../optimisation_structures/costcoord.hpp"

namespace homecare {

class NodeRepair {
protected:
    const static int NOT_ASSIGNED;

public:
    NodeRepair();
    ~NodeRepair();
    virtual int repairNodes() = 0;
};

}

#endif