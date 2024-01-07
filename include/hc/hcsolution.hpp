#ifndef HCSOLUTION_HPP
#define HCSOLUTION_HPP

#include <sstream>
#include <map>

#include "../readjson/hcdata.hpp"
#include "../structures/route.hpp"

namespace homecare {

class HCSolution {

private:
    HCData m_data;
    vector<Route> m_routes;
    map<string, int> m_prevServCaregiver;
    static const string I_SOL_FILE;

    int searchForRoute(Patient, int = NO_INDEX);
    int calculateArrivalTime(int route, int node); 
    int writeSolutionOnFile(string);

public:
    static const int NO_INDEX;
    static const int MAX_INT;

    HCSolution(string);
    ~HCSolution();
    int generateISolution() noexcept (false);
};

}

#endif
