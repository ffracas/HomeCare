#ifndef GREEDYREPAIR_HPP
#define GREEDYREPAIR_HPP

#include "noderepair.hpp"

namespace homecare {

class GreedyRepair : public NodeRepair {
private:
    ScheduleOptimiser repairIndependentService(ScheduleOptimiser&, Patient&, double&);
    ScheduleOptimiser repairInterdependentService(ScheduleOptimiser&, Patient&, double&);  
public:
    GreedyRepair();
    ~GreedyRepair();
    int repairNodes() override;
};

} 

#endif