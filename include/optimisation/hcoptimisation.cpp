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

vector<Route> HCOptimisation::optimise() {
    RandomRemoval rr;
    GreedyRepair  gr; 
    
    rr.removeNodes(ELEMENT_TO_DESTROY);
    gr.repairNodes();
       
    return ALNSOptimisation::getBestSol().getRoutes();
    /*for (const Route& r : m_ops.getCurrentSol().getRoutes()) {
        cout << r.getRouteToString() << "\n ----------------------------- \n";
    }*/
}
