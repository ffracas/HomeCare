#include "hcoptimisation.hpp"

using namespace std;
using namespace homecare;

const int HCOptimisation::ELEMENT_TO_DESTROY = 7;
const int HCOptimisation::PERIOD = 10;


HCOptimisation::HCOptimisation(Schedule t_schedule, double t_cost) 
        : m_solution (t_schedule), m_ops (ALNSOptimisation::getInstance(t_schedule, t_cost)) {}

HCOptimisation::~HCOptimisation() {}

Schedule HCOptimisation::optimise() {
    RandomRemoval rr;
    GreedyRepair  gr; 
    
    cout<<"\nrem\n";
    //rr.removeNodes(ELEMENT_TO_DESTROY);
    ScheduleOptimiser routes(m_ops->getCurrentSchedule());
    ScheduleOptimiser r1(m_ops->destroy(routes, 6, 4));
    m_ops->saveDestruction(r1,6,4);
    ScheduleOptimiser r2(m_ops->destroy(r1, 9, 2));
    m_ops->saveDestruction(r2,9,2);
    ScheduleOptimiser r3(m_ops->destroy(r2, 12, 1));
    m_ops->saveDestruction(r3,12,1);

    //todo: to delete
    for (auto route : m_ops->getCurrentSchedule().getSchedule()) {
        for (auto node : route.getNodes()) {
            cout<<node.getId()<<"->";
        }
        cout<<endl;
    }
    cout<<"\nCurrent cost: "<<m_ops->getCurrentCost()<<"\n";

    cout<<"\nrep\n";
    gr.repairNodes();
       
    return m_ops->getBestSol();
    /*for (const Route& r : m_ops.getCurrentSol().getRoutes()) {
        cout << r.getRouteToString() << "\n ----------------------------- \n";
    }*/
}
