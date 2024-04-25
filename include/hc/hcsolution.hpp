#ifndef HCSOLUTION_HPP
#define HCSOLUTION_HPP

#include <sstream>
#include <map>

#include "hcvalidation.hpp"
#include "../readjson/hcdata.hpp"
#include "../structures/route/route.hpp"
#include "../optimisation/hcoptimisation.hpp"
#include "../structures/schedule/schedule.hpp"

namespace homecare {

class HCSolution {

private:
    Schedule m_schedule;
    
    int m_maxTardiness;
    int m_maxIdleTime;
    int m_totalExtraTime;
    int m_totalTardiness;
    int m_totalWaitingTime;
    int m_travelTime;
    double m_solCost;

    static const std::string I_SOL_FILE;

    int searchForRoute(Patient, int = HCData::NO_INDEX);
    int calculateArrivalTime(int route, int node); 
    int writeSolutionOnFile(std::string);
    double calculateCost();
    double calculateCost(std::vector<Route>&);

public:
    HCSolution();
    ~HCSolution();
    int generateISolution() noexcept(false);
    void updateCostData();
    int optimizeSolution() noexcept(false);
};

}

#endif
