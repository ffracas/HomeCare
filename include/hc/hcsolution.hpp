#ifndef HCSOLUTION_HPP
#define HCSOLUTION_HPP

#include <sstream>
#include <map>

#include "../readjson/hcdata.hpp"
#include "../structures/route.hpp"
#include "hcvalidation.hpp"

namespace homecare {

class HCSolution {

private:
    HCData m_data;
    std::vector<Route> m_routes;
    std::map<std::string, int> m_prevServCaregiver;

    int m_maxTardiness;
    int m_maxIdleTime;
    int m_totalExtraTime;
    int m_totalTardiness;
    int m_totalWaitingTime;
    int m_travelTime;
    double m_solCost;

    static const double m_travelTimeWeight;
    static const double m_maxTardinessWeight;
    static const double m_tardinessWeight;
    static const double m_extraTimeWeight;
    static const double m_maxIdleTimeWeight;
    static const double m_totWaitingTimeWeight;

    static const std::string I_SOL_FILE;

    int searchForRoute(Patient, int = NO_INDEX);
    int calculateArrivalTime(int route, int node); 
    int writeSolutionOnFile(std::string);
    double calculateCost();

public:
    static const int NO_INDEX;
    static const int MAX_INT;

    HCSolution(std::string);
    ~HCSolution();
    int generateISolution() noexcept (false);
};

}

#endif
