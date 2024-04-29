#include "greedyrepair.hpp"

using namespace std;
using namespace homecare;

GreedyRepair::GreedyRepair() : NodeRepair() {}

GreedyRepair::~GreedyRepair() {}

int GreedyRepair::repairNodes() {
    ScheduleOptimiser routesToTest(m_data->getCurrentSchedule());
    // Best
    double bestCost = HCData::MAX_COST;
    ScheduleOptimiser bestRoute;

    while (m_data-> hasNodeToRepair()) {
        bestCost = HCData::MAX_COST;
        // find patient
        Patient patient(HCData::getPatient(m_data-> popNodeToRepair()));
        cout<<patient.getID();
        // indipendet service
        if (!patient.hasNextService()) {
            for (int i = 0; i < routesToTest.getNumberOfRoutes(); ++i) {
                if (routesToTest.isServiceAvailableInRoute(patient.getCurrentService().getService(), i)) {
                    ScheduleOptimiser newRoutes = m_data-> repairSingle(routesToTest, patient, i);
                    if (!newRoutes.isEmpty()) {
                        double cost = newRoutes.getCost();
                        if (cost < bestCost) {
                            bestCost = cost;
                            bestRoute = newRoutes;
                        }
                    }
                }
            }
            if (bestCost != HCData::MAX_COST) {
                routesToTest = bestRoute;
            }
            else { throw runtime_error("fallimento nel cercare soluzione"); }
        }
        // iterdependent service
        else {
            for (int i = 0; i < routesToTest.getNumberOfRoutes(); ++i) {
                for (int j = 0; j < routesToTest.getNumberOfRoutes(); ++j) {
                    if (i != j && routesToTest.isServiceAvailableInRoute(patient.getCurrentService().getService(), i) 
                    && routesToTest.isServiceAvailableInRoute(patient.getNextService().getService(), j)
                    && patient.isCaregiverValid(routesToTest.getCaregiverOfRoute(i))
                    && patient.getPatientAndNextService().isCaregiverValid(routesToTest.getCaregiverOfRoute(j))){
                        ScheduleOptimiser newRoutes (m_data-> repairDouble(routesToTest, patient, i, j));
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
            if (bestCost != HCData::MAX_COST) {
                routesToTest = bestRoute;
            }
            else { throw runtime_error("fallimento nel cercare soluzione"); }
        }
    }
    m_data-> saveRepair(routesToTest);
    return 1;
}
