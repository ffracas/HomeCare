#include "hcoptimisation.hpp"

using namespace std;
using namespace homecare;

const int HCOptimisation::ELEMENT_TO_DESTROY = 7;
const int HCOptimisation::PERIOD = 10;


HCOptimisation::HCOptimisation(Schedule t_schedule, double t_cost) 
        : m_solution (t_schedule), m_ops (ALNSOptimisation::getInstance(t_schedule, t_cost)) {}

HCOptimisation::~HCOptimisation() {}

Schedule HCOptimisation::optimise() {
    RandomRemoval rare;
    RelatedRemoval rere;
    GreedyRepair  gr; 
    
    m_ops->resetIteration();
    
    cout<<"\n-----\nIterarion "<<m_ops->startIteration()<<endl;
    cout<<"\nrem\n";
    
    //rare.removeNodes(ELEMENT_TO_DESTROY); // fixme: insert in roulette
    rere.removeNodes(ELEMENT_TO_DESTROY);

    /*ScheduleOptimiser routes(m_ops->getCurrentSchedule());
    ScheduleOptimiser r1(m_ops->destroy(routes, 0, 3));
    m_ops->saveDestruction(r1,0,3);
    cout<<"--------------\n";
    ScheduleOptimiser r2(m_ops->destroy(m_ops->getCurrentSchedule(), 12,1));
    m_ops->saveDestruction(r2,12,1);
    cout<<"--------------\n";
    ScheduleOptimiser r3(m_ops->destroy(m_ops->getCurrentSchedule(), 7,4));
    m_ops->saveDestruction(r3,7,4);
    cout<<"--------------\n";
    ScheduleOptimiser r4(m_ops->destroy(m_ops->getCurrentSchedule(), 12,1));
    m_ops->saveDestruction(r4,12,1);
    cout<<"--------------\n";
    ScheduleOptimiser r5(m_ops->destroy(m_ops->getCurrentSchedule(), 5,3));
    m_ops->saveDestruction(r5,5,3);
    cout<<"--------------\n";*/

    //todo: to delete
    cout<<endl;
    for (const string& nodeToTest : m_ops->getNodesToRepair()) {
        cout<<nodeToTest<<" - ";
    }
    cout<<endl;
    for (auto route : m_ops->getCurrentSchedule().getSchedule()) {
        for (auto node : route.getNodes()) {
            cout<<node.getId()<<"->";
        }
        cout<<endl;
    }

    cout<<"\nrep\n";
    int points = gr.repairNodes();
    cout<<"\nPoints: "<<points<<endl;

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
