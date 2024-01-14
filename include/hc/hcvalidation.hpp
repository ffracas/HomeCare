#ifndef HCVALIDATION_HPP
#define HCVALIDATION_HPP

#include <vector>
#include <sstream>

#include "../readjson/hcdata.hpp"
#include "../structures/route.hpp"
#include "../structures/arc.hpp"
#include "../structures/node.hpp"
#include "../structures/validationode.hpp"

namespace homecare {

class HCValidation
{
private:
    HCData m_data;
    std::vector<Route> m_routes;

public:
    HCValidation(std::string, std::string) noexcept (false);
    HCValidation(HCData, std::vector<Route>);
    ~HCValidation();
    bool checkSolution();
};

}

#endif