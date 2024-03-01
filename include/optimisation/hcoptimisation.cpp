#include "hcoptimisation.hpp"

using namespace std;
using namespace homecare;

const int HCOptimisation::ELEMENT_TO_DESTROY = 7;
const int HCOptimisation::PERIOD = 10;

HCOptimisation::HCOptimisation(std::vector<Route> t_routes, double t_cost) 
        : m_initial (t_routes) {
            m_ops.setISol(t_routes, t_cost);
        }

HCOptimisation::~HCOptimisation() {}

void HCOptimisation::optimise() {
    RandomRemoval rr;
    GreedyRepair  gr; 
    
    rr.removeNodes(ELEMENT_TO_DESTROY);
    try {
        gr.repairNodes();
    }catch(exception e) {
        cout << "No best";
    }
       
    for (const Route& r : m_ops.getCurrentSol().getRoutes()) {
        cout << r.getRouteToString() << "\n ----------------------------- \n";
    }
}
