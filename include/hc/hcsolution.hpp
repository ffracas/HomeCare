#ifndef HCSOLUTION_HPP
#define HCSOLUTION_HPP

#include <sstream>
#include <map>

#include "../readjson/hcdata.hpp"
#include "../structures/route.hpp"
#include "hcvalidation.hpp"
#include "../optimisation/hcoptimisation.hpp"

namespace homecare {

class HCSolution {

private:
    std::vector<Route> m_routes;
    
    int m_maxTardiness;
    int m_maxIdleTime;
    int m_totalExtraTime;
    int m_totalTardiness;
    int m_totalWaitingTime;
    int m_travelTime;
    double m_solCost;

    static const std::string I_SOL_FILE;

    int searchForRoute(Patient, int = NO_INDEX);
    int calculateArrivalTime(int route, int node); 
    int writeSolutionOnFile(std::string);
    double calculateCost();
    double calculateCost(std::vector<Route>&);

public:
    static const int NO_INDEX;
    static const int MAX_INT;

    HCSolution();
    ~HCSolution();
    int generateISolution() noexcept(false);
    void updateCostData();
    int optimizeSolution() noexcept(false);
};

}

#endif
