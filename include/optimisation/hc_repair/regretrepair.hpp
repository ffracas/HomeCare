#ifndef __REGRETREPAIR_HPP__
#define __REGRETREPAIR_HPP__

#include "noderepair.hpp"

namespace homecare {

struct RepairResult {
    ScheduleOptimiser solution1;
    ScheduleOptimiser solution2;
    double cost1 = HCData::MAX_COST;
    double cost2 = HCData::MAX_COST;
};

class RegretRepair : public NodeRepair {

private:
    RepairResult repairIndependentService(ScheduleOptimiser&, Patient&);
    RepairResult repairInterdependentService(ScheduleOptimiser&, Patient&); 
    ScheduleOptimiser regret2(const std::vector<std::string>& nodesToRepair, ScheduleOptimiser& schedule);  

public:
    RegretRepair();
    ~RegretRepair();
    int repairNodes() override;
};
}

#endif