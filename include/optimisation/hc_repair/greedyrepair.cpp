#include "greedyrepair.hpp"

using namespace std;
using namespace homecare;

GreedyRepair::GreedyRepair(ALNSOptimisation& t_repairOps) : NodeRepair(t_repairOps) {}

GreedyRepair::~GreedyRepair() {}

int GreedyRepair::repairNodes() {
    while (m_repairOps.hasNodeToRepair()) {
        // Best 
        double bestCost = ALNSOptimisation::MAX_DOUBLE;
        vector<Route>* bestRoute;
        // find patient
        Patient patient(HCData::getPatient( m_repairOps.popNodeToRepair()));
        // indipendet service
        if (!patient.hasNextService()) {
            for (int i = 0; i < m_repairOps.getNumberOfRoutes(); ++i) {
                vector<Route> routes(m_repairOps.getRoutes());
                if (routes[i].hasService(patient.getCurrentService().getService())) {
                    double valutation = m_repairOps.repair(routes, patient, i);
                    if (valutation < bestCost) {
                        bestCost = valutation;
                        bestRoute = &routes;
                    }
                }
            }
            if (bestCost == ALNSOptimisation::MAX_DOUBLE) { return 0; }
            m_repairOps.saveSol(*bestRoute);
        }
        // iterdependent service
        else {
            CostCoord bestSync(ALNSOptimisation::MAX_DOUBLE, NOT_ASSIGNED, NOT_ASSIGNED);
            int syncTime = 0;
            for (int i = 0; i < m_repairOps.getNumberOfRoutes(); ++i) {
                vector<Route> routes(m_repairOps.getRoutes());
                m_repairOps.repairDouble(patient);




                if (routes[i].hasService(patient.getCurrentService().getService())) {
                    CostCoord first(m_repairOps.repair(routes, patient, i));
                    time = routes[first.getRouteNumber()].getNodes()[best.getNodePosition()].getArrivalTime();
                    for (int j = 0; j < m_repairOps.getNumberOfRoutes(); ++j) {
                        vector<Route> routesToSync(routes);
                        if (routesToSync[j].hasService(patient.getCurrentService().getService())
                            && j != i) {
                        CostCoord second(m_repairOps.repair(routesToSync, patient.getPatientAndNextService(), j));
                            if (second.getCost() < bestSync.getCost()) {
                                best = first;
                                bestSync = second;
                                time = routesToSync[best.getRouteNumber()]
                                            .getNodes()[best.getNodePosition()].getArrivalTime() 
                                        + patient.getMaxWait()
                                        < routesToSync[bestSync.getRouteNumber()]
                                            .getNodes()[bestSync.getNodePosition()].getArrivalTime() 
                                        ? routesToSync[bestSync.getRouteNumber()]
                                            .getNodes()[bestSync.getNodePosition()].getArrivalTime() 
                                          - patient.getMaxWait()
                                        : routesToSync[best.getRouteNumber()]
                                            .getNodes()[best.getNodePosition()].getArrivalTime();
                                syncTime = routesToSync[bestSync.getRouteNumber()]
                                        .getNodes()[bestSync.getNodePosition()].getArrivalTime();
                            }
                        }
                    }
                }
            }
            if (best.getCost() == ALNSOptimisation::MAX_DOUBLE && bestSync.getCost() == ALNSOptimisation::MAX_DOUBLE) { 
                return 0; 
            }
            m_repairOps.repair(Node(patient, time), best.getRouteNumber(), best.getNodePosition());
            m_repairOps.repair(Node(patient.getPatientAndNextService(time), syncTime), 
                                bestSync.getRouteNumber(), bestSync.getNodePosition());
        }
    }
    return 1;
}
