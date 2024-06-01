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
    cout<<"--------------\n";
    ScheduleOptimiser r2(m_ops->destroy(m_ops->getCurrentSchedule(), 12, 1));
    m_ops->saveDestruction(r2,12,1);
    cout<<"--------------\n";
    ScheduleOptimiser r3(m_ops->destroy(m_ops->getCurrentSchedule(), 4, 2));
    m_ops->saveDestruction(r3,4,2);
    cout<<"--------------\n";
    ScheduleOptimiser r4(m_ops->destroy(m_ops->getCurrentSchedule(), 6, 4));
    m_ops->saveDestruction(r4,6,4);
    cout<<"--------------\n";



    //todo: to delete
    for (auto route : m_ops->getCurrentSchedule().getSchedule()) {
        for (auto node : route.getNodes()) {
            cout<<node.getId()<<"->";
        }
        cout<<endl;
    }
    //cout<<"\nCurrent cost: "<<m_ops->getCurrentCost()<<"\n";

    cout<<"\nrep\n";
    gr.repairNodes();
    cout<<"\n----------\n";
    //todo: to delete
    for (auto route : m_ops->getCurrentSchedule().getSchedule()) {
        for (auto node : route.getNodes()) {
            cout<<node.getId()<<"->";
        }
        cout<<endl;
    }
       
    return m_ops->getBestSol();
}
