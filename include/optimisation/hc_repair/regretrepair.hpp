#ifndef __REGRETREPAIR_HPP__
#define __REGRETREPAIR_HPP__

#include "noderepair.hpp"

namespace homecare {

struct RegretResult {
    ScheduleOptimiser solution;
    double difference;
};

class RegretRepair : public NodeRepair {

private:
    RegretResult repairIndependentService(ScheduleOptimiser&, Patient&);
    RegretResult repairInterdependentService(ScheduleOptimiser&, Patient&);   

public:
    RegretRepair();
    ~RegretRepair();
    int repairNodes() override;
};
}

#endif