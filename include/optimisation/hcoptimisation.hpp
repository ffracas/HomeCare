#ifndef HCOPTIMISATION_HPP
#define HCOPTIMISATION_HPP

#include <vector>
#include <sstream>
#include <random>

#include "../readjson/hcdata.hpp"
#include "../structures/route.hpp"
#include "./optimisation_structures/optimisationobject.hpp"

namespace homecare {

class HCOptimisation {
private:
    /* data */
    std::vector<Route> m_initial;
    OptimisationObject m_ops;
    static const int ELEMENT_TO_DESTROY;
    static const int PERIOD;
    //TODO variabili per meccanismi di scelta dell'algoritmo

public:
    HCOptimisation(HCData, std::vector<Route>, double);
    ~HCOptimisation();
    void optimise();
};

}

#endif