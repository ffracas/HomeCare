#include "regretrepair.hpp"

using namespace std;
using namespace homecare;

namespace std_alias = std;

RegretRepair::RegretRepair() : NodeRepair() {}

RegretRepair::~RegretRepair() {}

int RegretRepair::repairNodes() {
    ScheduleOptimiser current(m_data->getCurrentSchedule());
    
    while (m_data->hasNodeToRepair()) {
        // Best route initialization
        std::string bestNode;
        ScheduleOptimiser bestInsertion;
        double bestRegret = -HCData::MAX_COST;

        for (const string &nodeToTest : m_data->getNodesToRepair()) {
            Patient patient(HCData::getPatient(nodeToTest));
            //cout << endl<< patient.getID() << endl; // fixme: delete debug print
            RegretResult result = patient.isInterdependent() 
                ? repairInterdependentService(current, patient) 
                : repairIndependentService(current, patient);
            
            if (result.difference > bestRegret 
                || (result.difference == bestRegret && result.solution.getCost() < bestInsertion.getCost())) {
                bestRegret = result.difference;
                bestNode = nodeToTest;
                bestInsertion = result.solution;
            }
            else {
                if (result.difference == HCData::MAX_COST) {
                    bestInsertion = result.solution;
                    bestNode = nodeToTest;
                    bestRegret = result.difference;
                }
            }
        }
        if (bestRegret == HCData::MAX_COST && bestInsertion.isEmpty()) {
            return ALNSOptimisation::OTHERWISE;
        }
        // update data
        current = bestInsertion;
        m_data->scheduledNode(bestNode);
    }

    // fixme: debug print
    for (auto route : current.getSchedule()) {
        for (auto node : route.getNodes()) {
            //cout << node.getId() << "->";
        }
        //cout << endl;
    }
    //cout << "\nCurrent cost: " << current.getCost() << "\n";

    if (HCValidation(current.getSchedule()).checkSolution()) {
        return m_data->saveRepair(current);
    }
    return ALNSOptimisation::OTHERWISE;
}

////////////////////////////////////////////////////////////////////////////////                        REPAIR

RegretResult RegretRepair::repairIndependentService(ScheduleOptimiser& routes, Patient& patient) {
    ScheduleOptimiser bestRoute;
    double bestCost1 = HCData::MAX_COST;
    double bestCost2 = HCData::MAX_COST;
    for (int i = 0; i < routes.getNumberOfRoutes(); ++i) {
        if (routes.isServiceAvailableInRoute(patient.getCurrentService().getService(), i) &&
            patient.isCaregiverValid(routes.getRoute(i).getCaregiver())) {
            //cout << "scheduling c" << i << "  ";//fixme: delete
            ScheduleOptimiser newRoutes = m_data->repairSingle(routes, patient, i);
            if (!newRoutes.isEmpty()) {
                //cout << "successful\n";//fixme: delete
                double cost = newRoutes.getCost();
                //cout << "cost: " << cost << endl;//fixme: delete
                //cout << "bestPair cost1: " << bestPair.cost1 << endl;//fixme: delete
                if (cost < bestCost1) {
                    bestCost2 = bestCost1;
                    bestCost1 = cost;
                    bestRoute = newRoutes;
                }
                else if (cost < bestCost2) {
                    bestCost2 = cost;
                    bestRoute = newRoutes;
                }
            }
        }
    }
    RegretResult result {
        bestRoute,
        bestCost2 - bestCost1
    };
    return result;
}

RegretResult RegretRepair::repairInterdependentService(ScheduleOptimiser& routes, Patient& patient) {
    ScheduleOptimiser bestRoute;
    double bestCost1 = HCData::MAX_COST;
    double bestCost2 = HCData::MAX_COST;
    for (int i = 0; i < routes.getNumberOfRoutes(); ++i) {
        for (int j = 0; j < routes.getNumberOfRoutes(); ++j) {
            if (i != j &&
                routes.isServiceAvailableInRoute(patient.getCurrentService().getService(), i) &&
                routes.isServiceAvailableInRoute(patient.getNextService().getService(), j) &&
                patient.isCaregiverValid(routes.getCaregiverOfRoute(i)) &&
                patient.isCaregiverValid(routes.getCaregiverOfRoute(j))) {
                //cout << "scheduling c" << i << " c"<<j<<"  ";//fixme: delete
                ScheduleOptimiser newRoutes = m_data->repairDouble(routes, patient, i, j);
                if (!newRoutes.isEmpty()) {
                    //cout << "successful\n";//fixme: delete
                    double cost = newRoutes.getCost();
                    if (cost < bestCost1) {
                        bestCost2 = bestCost1;
                        bestCost1 = cost;
                        bestRoute = newRoutes;
                    }
                    else if (cost < bestCost2) {
                        bestCost2 = cost;
                        bestRoute = newRoutes;
                    }
                }
            }
        }
    }
    RegretResult result {
        bestRoute,
        bestCost2 - bestCost1
    };
    return result;
} 
