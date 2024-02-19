#ifndef HCOPTIMISATION_HPP
#define HCOPTIMISATION_HPP

#include <vector>
#include <sstream>
#include <random>

#include "../readjson/hcdata.hpp"
#include "../structures/route.hpp"
#include "./optimisation_structures/alnsoptimisation.hpp"

namespace homecare {

class HCOptimisation {
private:
    std::vector<Route> m_initial;
    ALNSOptimisation m_ops;
    static const int ELEMENT_TO_DESTROY;
    static const int PERIOD;
    //TODO variabili per meccanismi di scelta dell'algoritmo

public:
    HCOptimisation(std::vector<Route>, double);
    ~HCOptimisation();
    void optimise();
};

}

#endif