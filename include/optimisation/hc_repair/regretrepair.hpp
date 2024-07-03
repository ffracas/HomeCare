#ifndef __REGRETREPAIR_HPP__
#define __REGRETREPAIR_HPP__

#include "noderepair.hpp"

namespace homecare {

struct RegretResult {
    ScheduleOptimiser solution1;
    ScheduleOptimiser solution2;
    double cost1 = HCData::MAX_COST;
    double cost2 = HCData::MAX_COST;
};

class RegretRepair : public NodeRepair {

private:
    RegretResult repairIndependentService(ScheduleOptimiser&, Patient&);
    RegretResult repairInterdependentService(ScheduleOptimiser&, Patient&); 
    ScheduleOptimiser regret2(const std::vector<std::string>& nodesToRepair, ScheduleOptimiser& schedule);  

public:
    RegretRepair();
    ~RegretRepair();
    int repairNodes() override;
};
}

#endif