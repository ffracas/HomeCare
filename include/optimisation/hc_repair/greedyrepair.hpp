#ifndef GREEDYREPAIR_HPP
#define GREEDYREPAIR_HPP

#include "noderepair.hpp"

namespace homecare {

class GreedyRepair : public NodeRepair {
private:
    
public:
    GreedyRepair(ALNSOptimisation&);
    ~GreedyRepair();
    int repairNodes() override;
};

} 

#endif