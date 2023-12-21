#ifndef HCSOLUTION_HPP
#define HCSOLUTION_HPP

#include <sstream>

#include "../readjson/hcdata.hpp"
#include "../structures/route.hpp"

using namespace std;

namespace homecare {

class HCSolution {

private:
    HCData m_data;
    vector<Route> m_routes;

    int searchForRoute(Patient, int = NO_INDEX);
    int calculateArrivalTime(int route, Patient node); 

public:
    static const int NO_INDEX;
    static const int MAX_INT;

    HCSolution(string);
    ~HCSolution();
    int generateISolution() noexcept (false);
};

}

#endif
