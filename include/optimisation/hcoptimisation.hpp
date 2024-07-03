#ifndef HCOPTIMISATION_HPP
#define HCOPTIMISATION_HPP

#include <vector>
#include <sstream>
#include <random>

#include "../readjson/hcdata.hpp"
#include "../structures/route/route.hpp"
#include "../structures/schedule/schedule.hpp"
#include "optimisation_structures/alnsoptimisation.hpp"
#include "roulette/roulette.hpp"
#include "hc_removal/randomremoval.hpp"
#include "hc_removal/worstremoval.hpp"
#include "hc_removal/relatedremoval.hpp"
#include "hc_removal/clusterremoval.hpp"
#include "hc_repair/greedyrepair.hpp"
#include "hc_repair/regretrepair.hpp"


namespace homecare {

class HCOptimisation {
private:
    Schedule m_solution;
    ALNSOptimisation* m_ops;
    static const int PERIOD = 5;
    static const int MAX_ITERATIONS = 10;
    int element_to_destroy = 1;
    //static const int MAX_ITERATIONS_WITHOUT_IMPROVEMENT = 10;
    
    void destroyPilot();

public:
    HCOptimisation(Schedule, double);
    ~HCOptimisation();
    Schedule optimise();
};

}

#endif