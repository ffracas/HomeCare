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
    
    cout<<"\nrem\n";
    //rr.removeNodes(ELEMENT_TO_DESTROY);
    RoutesOpt routes(ALNSOptimisation::getCurrentSchedule());
    RoutesOpt r1(ALNSOptimisation::destroy(routes, 6, 4));
    ALNSOptimisation::saveDestruction(r1,6,4);
    RoutesOpt r2(ALNSOptimisation::destroy(r1, 9, 2));
    ALNSOptimisation::saveDestruction(r2,9,2);
    RoutesOpt r3(ALNSOptimisation::destroy(r2, 12, 1));
    ALNSOptimisation::saveDestruction(r3,12,1);
    cout<<"\nrep\n";
    gr.repairNodes();
       
    return ALNSOptimisation::getBestSol().getRoutes();
    /*for (const Route& r : m_ops.getCurrentSol().getRoutes()) {
        cout << r.getRouteToString() << "\n ----------------------------- \n";
    }*/
}
