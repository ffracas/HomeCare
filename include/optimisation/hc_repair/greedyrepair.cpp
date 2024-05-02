#include "greedyrepair.hpp"

using namespace std;
using namespace homecare;

GreedyRepair::GreedyRepair() : NodeRepair() {}

GreedyRepair::~GreedyRepair() {}

/**
 * Attempts to repair the nodes in the current schedule by greedily assigning
 * patients to available routes. If no feasible solution can be found, throws
 * a runtime_error.
 *
 * @return 1 if the repair was successful, otherwise throws an exception
 */
int GreedyRepair::repairNodes() {
    ScheduleOptimiser routesToTest(m_data->getCurrentSchedule());

    while (m_data->hasNodeToRepair()) {
        // Best
        ScheduleOptimiser bestRoute;
        double bestCost = HCData::MAX_COST;
        // find patient
        Patient patient(HCData::getPatient(m_data->popNodeToRepair()));
        cout << patient.getID();
        // indipendet service
        if (!patient.hasNextService()) {
            for (int i = 0; i < routesToTest.getNumberOfRoutes(); ++i) {
                if (routesToTest.isServiceAvailableInRoute(patient.getCurrentService().getService(), i)) {
                    ScheduleOptimiser newRoutes = m_data->repairSingle(routesToTest, patient, i);
                    if (!newRoutes.isEmpty()) {
                        double cost = newRoutes.getCost();
                        if (cost < bestCost) {
                            bestCost = cost;
                            bestRoute = newRoutes;
                        }
                    }
                }
            }
        }
        // iterdependent service
        else {
            for (int i = 0; i < routesToTest.getNumberOfRoutes(); ++i) {
                for (int j = 0; j < routesToTest.getNumberOfRoutes(); ++j) {
                    if (i != j && routesToTest.isServiceAvailableInRoute(patient.getCurrentService().getService(), i) && routesToTest.isServiceAvailableInRoute(patient.getNextService().getService(), j) && patient.isCaregiverValid(routesToTest.getCaregiverOfRoute(i)) && patient.getPatientAndNextService().isCaregiverValid(routesToTest.getCaregiverOfRoute(j))) {
                        ScheduleOptimiser newRoutes(m_data->repairDouble(routesToTest, patient, i, j));
                        if (!newRoutes.isEmpty()) {
                            double cost = newRoutes.getCost();
                            if (cost < bestCost) {
                                bestCost = cost;
                                bestRoute = newRoutes;
                            }
                        }
                    }
                }
            }
        }
        cout << "best cost: " << bestCost << endl;
        if (bestCost != HCData::MAX_COST) { routesToTest = bestRoute; }
        else { throw runtime_error("fallimento nel cercare soluzione"); }
    }
    
    // todo: remove this
    for (auto route : routesToTest.getSchedule()) {
        for (auto node : route.getNodes()) {
            cout<<node.getId()<<"->";
        }
        cout<<endl;
    }
    cout<<"\nCurrent cost: "<<routesToTest.getCost()<<"\n";

    m_data->saveRepair(routesToTest);
    return 1;
}
