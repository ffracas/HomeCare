#ifndef HCVALIDATION_HPP
#define HCVALIDATION_HPP

#include <vector>
#include <sstream>

#include "../readjson/hcdata.hpp"
#include "../structures/route/route.hpp"
#include "../structures/node.hpp"
#include "../structures/validationode.hpp"

namespace homecare {

class HCValidation
{
private:
    std::vector<Route> m_routes;

public:
    HCValidation(std::string, std::string) noexcept (false);
    HCValidation(std::vector<Route>);
    ~HCValidation();
    bool checkSolution();
};

}

#endif