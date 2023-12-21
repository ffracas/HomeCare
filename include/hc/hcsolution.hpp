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

public:
    HCSolution(string);
    ~HCSolution();
    int generateISolution() noexcept (false);
};

}

#endif
