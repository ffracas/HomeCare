#include "regretrepair.hpp"

using namespace std;
using namespace homecare;

namespace std_alias = std;

RegretRepair::RegretRepair() : NodeRepair() {}

RegretRepair::~RegretRepair() {}

int RegretRepair::repairNodes() {
    ScheduleOptimiser schedule(m_data->getCurrentSchedule());
    
    ScheduleOptimiser solution = regret2(m_data->getNodesToRepair(), schedule);
    for (const string &node : m_data->getNodesToRepair()) {
        m_data->scheduledNode(node);
    }
    if (solution.isEmpty()) {
        return ALNSOptimisation::OTHERWISE;
    }
    if (HCValidation(solution.getSchedule()).checkSolution()) {
        cout<<"cost: "<<solution.getCost()<<endl;
        return m_data->saveRepair(solution);
    }
    return ALNSOptimisation::OTHERWISE;
}

ScheduleOptimiser RegretRepair::regret2(const std::vector<std::string>& nodesToRepair, ScheduleOptimiser& schedule) {
    // Best route initialization
    if (schedule.isEmpty()) {
        return ScheduleOptimiser();
    }

    std::string bestNode;
    ScheduleOptimiser bestInsertion1, bestInsertion2;
    double bestRegret = -HCData::MAX_COST;

    // look for the pair with the biggest difference
    for (const string &nodeToTest : nodesToRepair) {
        Patient patient(HCData::getPatient(nodeToTest));
        cout<<patient.getID()<<endl; // fixme: delete debug print
        RepairResult result = patient.isInterdependent() 
            ? repairInterdependentService(schedule, patient) 
            : repairIndependentService(schedule, patient);

        // comapare pairs
        if (result.solution1.isEmpty() && result.solution2.isEmpty()) {
            continue;
        }
        if (!result.solution1.isEmpty() && result.solution2.isEmpty()) {
            bestRegret = HCData::MAX_COST;
            bestNode = nodeToTest;
            bestInsertion1 = result.solution1;
            bestInsertion2 = result.solution2;
        }
        double regret = result.cost2 - result.cost1;
        if (regret > bestRegret) {
            bestRegret = regret;
            bestNode = nodeToTest;
            bestInsertion1 = result.solution1;
            bestInsertion2 = result.solution2;
        }
    }
    // schedule the best node
    vector<string> updateToRepair (nodesToRepair);
    updateToRepair.erase(remove(updateToRepair.begin(), updateToRepair.end(), bestNode), updateToRepair.end());

    if (updateToRepair.empty()) {
        if (HCValidation(bestInsertion1.getSchedule()).checkSolution()) {
            return bestInsertion1;        
        }
        if (HCValidation(bestInsertion2.getSchedule()).checkSolution()) {
            return bestInsertion2;
        }
        return ScheduleOptimiser();
    }
    
    // if there is not best so there's no solution
    if (bestInsertion1.isEmpty()) { return ScheduleOptimiser(); }
    // calculate best solution
    ScheduleOptimiser bestSolution = regret2(updateToRepair, bestInsertion1);
    // if there are not alternatives return best solution
    if (bestInsertion2.isEmpty()) { return bestSolution; }
    // if there is an alternative calculate solution
    ScheduleOptimiser solution2 = regret2(updateToRepair, bestInsertion2);
    // if the best solution is more expensive than the alternative, return alternative
    if (bestSolution.getCost() > solution2.getCost()) { return solution2; }
    return bestSolution;
    
    return ScheduleOptimiser();
}

////////////////////////////////////////////////////////////////////////////////                        REPAIR

RepairResult RegretRepair::repairIndependentService(ScheduleOptimiser& routes, Patient& patient) {
    RepairResult bestPair;

    for (int i = 0; i < routes.getNumberOfRoutes(); ++i) {
        if (routes.isServiceAvailableInRoute(patient.getCurrentService().getService(), i) &&
            patient.isCaregiverValid(routes.getRoute(i).getCaregiver())) {
            cout << "scheduling c" << i << "  ";//fixme: delete
            ScheduleOptimiser newRoutes = m_data->repairSingle(routes, patient, i);
            if (!newRoutes.isEmpty()) {
                cout << "successful\n";//fixme: delete
                double cost = newRoutes.getCost();
                cout << "cost: " << cost << endl;//fixme: delete
                cout << "bestPair cost1: " << bestPair.cost1 << endl;//fixme: delete
                if (cost < bestPair.cost1) {
                    bestPair.cost2 = bestPair.cost1;
                    bestPair.solution2 = bestPair.solution1;
                    bestPair.cost1 = cost;
                    bestPair.solution1 = newRoutes;
                }
                else if (cost < bestPair.cost2) {
                    bestPair.cost2 = cost;
                    bestPair.solution2 = newRoutes;
                }
            }
        }
    }
    return bestPair;
}

RepairResult RegretRepair::repairInterdependentService(ScheduleOptimiser& routes, Patient& patient) {
    RepairResult bestPair;

    for (int i = 0; i < routes.getNumberOfRoutes(); ++i) {
        for (int j = 0; j < routes.getNumberOfRoutes(); ++j) {
            if (i != j &&
                routes.isServiceAvailableInRoute(patient.getCurrentService().getService(), i) &&
                routes.isServiceAvailableInRoute(patient.getNextService().getService(), j) &&
                patient.isCaregiverValid(routes.getCaregiverOfRoute(i)) &&
                patient.isCaregiverValid(routes.getCaregiverOfRoute(j))) {
                cout << "scheduling c" << i << " c"<<j<<"  ";//fixme: delete
                ScheduleOptimiser newRoutes = m_data->repairDouble(routes, patient, i, j);
                if (!newRoutes.isEmpty()) {
                    cout << "successful\n";//fixme: delete
                    double cost = newRoutes.getCost();
                    if (cost < bestPair.cost1) {
                        bestPair.cost2 = bestPair.cost1;
                        bestPair.solution2 = bestPair.solution1;
                        bestPair.cost1 = cost;
                        bestPair.solution1 = newRoutes;
                    }
                    else if (cost < bestPair.cost2) {
                        bestPair.cost2 = cost;
                        bestPair.solution2 = newRoutes;
                    }
                }
            }
        }
    }

    return bestPair;
} 
