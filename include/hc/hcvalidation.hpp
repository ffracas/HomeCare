#ifndef HCVALIDATION_HPP
#define HCVALIDATION_HPP

#include <vector>
#include <set>

#include "../readjson/hcdata.hpp"
#include "../structures/route.hpp"
#include "../structures/arc.hpp"
#include "../structures/node.hpp"

namespace homecare {

class HCValidation
{
private:
    HCData m_data;
    vector<Route> m_routes;

public:
    HCValidation(string, string) noexcept (false);
    ~HCValidation();
    bool checkSolution();
};

}

#endif