#ifndef HCOPTIMISATION_HPP
#define HCOPTIMISATION_HPP

#include <vector>
#include <sstream>
#include <random>

#include "../readjson/hcdata.hpp"
#include "../structures/route/route.hpp"
#include "../structures/schedule/schedule.hpp"
#include "optimisation_structures/alnsoptimisation.hpp"
#include "hc_removal/randomremoval.hpp"
#include "hc_removal/worstremoval.hpp"
#include "hc_removal/relatedremoval.hpp"
#include "hc_removal/clusterremoval.hpp"
#include "hc_repair/greedyrepair.hpp"


namespace homecare {

class HCOptimisation {
private:
    Schedule m_solution;
    ALNSOptimisation* m_ops;
    static const int ELEMENT_TO_DESTROY;
    static const int PERIOD;
    //TODO variabili per meccanismi di scelta dell'algoritmo

public:
    HCOptimisation(Schedule, double);
    ~HCOptimisation();
    Schedule optimise();
};

}

#endif