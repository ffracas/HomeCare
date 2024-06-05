#include "greedyrepair.hpp"

using namespace std;
using namespace homecare;

GreedyRepair::GreedyRepair() : NodeRepair() {}

GreedyRepair::~GreedyRepair() {}

/**
 * Attempts to repair the current schedule by greedily reassigning nodes that need to be repaired.
 *
 * The function iterates through the nodes that need to be repaired, and for each node, it tries to find the best
 * route to assign the node to. It does this by checking all the existing routes and seeing if the service required
 * by the node is available in any of the routes, and if the caregiver is valid. If a valid route is found, it
 * attempts to repair the schedule by reassigning the node to that route.
 *
 * If the node has an interdependent service, it also checks for routes that can handle both the current service
 * and the next service required by the node.
 *
 * The function keeps track of the best repair found so far, and updates the schedule with the best repair if one
 * is found. If no valid repair is found, it throws a runtime_error.
 *
 * @return 1 if a valid repair was found and saved, 0 otherwise
 */
int GreedyRepair::repairNodes()
{
    ScheduleOptimiser routesToTest(m_data->getCurrentSchedule());

    while (m_data->hasNodeToRepair()) {
        // Best route initialization
        ScheduleOptimiser bestRoute;
        double bestCost = HCData::MAX_COST;

        // Get the next patient to repair
        Patient patient(HCData::getPatient(m_data->popNodeToRepair()));
        cout << patient.getID() << endl; // Debug print

        // Handle independent service
        if (!patient.hasNextService()) {
            bestRoute = repairIndependentService(routesToTest, patient, bestCost);
        } 
        // Handle interdependent service
        else {
            bestRoute = repairInterdependentService(routesToTest, patient, bestCost);
        }

        cout << "best cost: " << bestCost << endl;
        if (bestCost != HCData::MAX_COST) {
            routesToTest = bestRoute;
        } else {
            throw runtime_error("Failed to find a solution");
        }
    }

    // fixme: debug print
    for (auto route : routesToTest.getSchedule())
    {
        for (auto node : route.getNodes())
        {
            cout << node.getId() << "->";
        }
        cout << endl;
    }
    cout << "\nCurrent cost: " << routesToTest.getCost() << "\n";

    if (HCValidation(routesToTest.getSchedule()).checkSolution()) {
        m_data->saveRepair(routesToTest);
        return 1;
    }
    return 0;
}

ScheduleOptimiser GreedyRepair::repairIndependentService(ScheduleOptimiser& routes, Patient& patient, double& bestCost) {
    ScheduleOptimiser bestRoute;

    for (int i = 0; i < routes.getNumberOfRoutes(); ++i) {
        if (routes.isServiceAvailableInRoute(patient.getCurrentService().getService(), i) &&
            patient.isCaregiverValid(routes.getRoute(i).getCaregiver())) {
            cout << "scheduling c" << i << endl;
            ScheduleOptimiser newRoutes = m_data->repairSingle(routes, patient, i);
            if (!newRoutes.isEmpty()) {
                cout << "successful";
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
                cout << "scheduling c" << i << " and c"<<j << endl;
                ScheduleOptimiser newRoutes = m_data->repairDouble(routes, patient, i, j);
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

    return bestRoute;
}

