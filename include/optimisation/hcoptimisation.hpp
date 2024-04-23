#ifndef HCOPTIMISATION_HPP
#define HCOPTIMISATION_HPP

#include <vector>
#include <sstream>
#include <random>

#include "../readjson/hcdata.hpp"
#include "../structures/route/route.hpp"
#include "./optimisation_structures/alnsoptimisation.hpp"
#include "hc_removal/randomremoval.hpp"
#include "hc_removal/worstremoval.hpp"
#include "hc_removal/relatedremoval.hpp"
#include "hc_removal/clusterremoval.hpp"
#include "hc_repair/greedyrepair.hpp"


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
    std::vector<Route> optimise();
};

}

#endif