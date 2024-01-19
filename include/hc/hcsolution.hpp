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
    static const std::string I_SOL_FILE;

    int searchForRoute(Patient, int = NO_INDEX);
    int calculateArrivalTime(int route, int node); 
    int writeSolutionOnFile(std::string);

public:
    static const int NO_INDEX;
    static const int MAX_INT;

    HCSolution(std::string);
    ~HCSolution();
    int generateISolution() noexcept (false);
};

}

#endif
