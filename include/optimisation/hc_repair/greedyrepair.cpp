#include "greedyrepair.hpp"

using namespace std;
using namespace homecare;

GreedyRepair::GreedyRepair() : NodeRepair() {}

GreedyRepair::~GreedyRepair() {}


/**
 * Repairs the nodes in the current schedule by iterating through the nodes that need to be repaired and finding the best solution for each one.
 *
 * @return ALNSOptimisation::SUCCESS if the repair was successful, ALNSOptimisation::OTHERWISE otherwise.
 */
int GreedyRepair::repairNodes()
{
    ScheduleOptimiser routesToTest(m_data->getCurrentSchedule());

    while (m_data->hasNodeToRepair())
    {

        // Best route initialization
        ScheduleOptimiser bestSolution;
        double bestCost = HCData::MAX_COST;
        string bestNode = "no";

        for (const string &nodeToTest : m_data->getNodesToRepair())
        {
            ScheduleOptimiser solution;
            double cost = HCData::MAX_COST;
            Patient patient(HCData::getPatient(nodeToTest));
            //cout << endl<< patient.getID() << endl; // fixme: delete debug print
            if (patient.isInterdependent()) {
                solution = repairInterdependentService(routesToTest, patient, cost);
            }
            else {
                solution = repairIndependentService(routesToTest, patient, cost);
            }
            if (cost < bestCost) {
                bestCost = cost;
                bestNode = nodeToTest;
                bestSolution = solution;
            }
        }
        if (bestCost == HCData::MAX_COST)
        {
            return ALNSOptimisation::OTHERWISE;
        }
        // update data
        routesToTest = bestSolution;
        m_data->scheduledNode(bestNode);
    }

    // fixme: debug print
    for (auto route : routesToTest.getSchedule()) {
        for (auto node : route.getNodes()) {
            //cout << node.getId() << "->";
        }
        //cout << endl;
    }
    //cout << "\nCurrent cost: " << routesToTest.getCost() << "\n";

    if (HCValidation(routesToTest.getSchedule()).checkSolution()) {
        return m_data->saveRepair(routesToTest);
    }
    return ALNSOptimisation::OTHERWISE;
}

ScheduleOptimiser GreedyRepair::repairIndependentService(ScheduleOptimiser& routes, Patient& patient, double& bestCost) {
    ScheduleOptimiser bestRoute;

    for (int i = 0; i < routes.getNumberOfRoutes(); ++i) {
        if (routes.isServiceAvailableInRoute(patient.getCurrentService().getService(), i) &&
            patient.isCaregiverValid(routes.getRoute(i).getCaregiver())) {
            //cout << "scheduling c" << i << "  ";//fixme: delete
            ScheduleOptimiser newRoutes = m_data->repairSingle(routes, patient, i);
            if (!newRoutes.isEmpty()) {
                //cout << "successful\n";//fixme: delete
                double cost = newRoutes.getCost();
                if (cost < bestCost) {
                    bestCost = cost;
                    bestRoute = newRoutes;
                }
            }
        }
    }
    return bestRoute;
}

ScheduleOptimiser GreedyRepair::repairInterdependentService(ScheduleOptimiser& routes, Patient& patient, 
                                                            double& bestCost) {
    ScheduleOptimiser bestRoute;

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
                    if (cost < bestCost) {
                        bestCost = cost;
                        bestRoute = newRoutes;
                    }
                }
            }
        }
    }

    return bestRoute;
}

