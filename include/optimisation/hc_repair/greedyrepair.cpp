#include "greedyrepair.hpp"

using namespace std;
using namespace homecare;

GreedyRepair::GreedyRepair() {}

GreedyRepair::~GreedyRepair() {}

int GreedyRepair::repairNodes()
{
    while (ALNSOptimisation::hasNodeToRepair())
    {
        // Best
        double bestCost = ALNSOptimisation::MAX_DOUBLE;
        RoutesOpt *bestRoute;
        // find patient
        Patient patient(HCData::getPatient(ALNSOptimisation::popNodeToRepair()));
        RoutesOpt routesToTest(ALNSOptimisation::getCurrentSol());
        // indipendet service
        if (!patient.hasNextService())
        {
            for (int i = 0; i < routesToTest.getNumberOfRoutes(); ++i)
            {
                if (routesToTest.isServiceAvailableInRoute(patient.getCurrentService().getService(), i))
                {
                    RoutesOpt newRoutes (ALNSOptimisation::repairSingle(routesToTest, patient, i));
                    if (!newRoutes.isEmpty())
                    {
                        double cost = ALNSOptimisation::calculateCost(newRoutes.getRoutes());
                        if (cost < bestCost)
                        {
                            bestCost = cost;
                            bestRoute = &newRoutes;
                        }
                    }
                }
            }
        }
        // iterdependent service
        else
        {
            for (int i = 0; i < routesToTest.getNumberOfRoutes(); ++i)
            {
                for (int j = 0; j < routesToTest.getNumberOfRoutes(); ++j)
                {
                    if (i != j && routesToTest.isServiceAvailableInRoute(patient.getCurrentService().getService(), i) 
                    && routesToTest.isServiceAvailableInRoute(patient.getNextService().getService(), j))
                    {
                        RoutesOpt newRoutes (ALNSOptimisation::repairDouble(routesToTest, patient, i, j));
                        if (!newRoutes.isEmpty())
                        {
                            double cost = ALNSOptimisation::calculateCost(newRoutes.getRoutes());
                            if (cost < bestCost)
                            {
                                bestCost = cost;
                                bestRoute = &newRoutes;
                            }
                        }
                    }
                }
            }
        }
        if (bestCost != ALNSOptimisation::MAX_DOUBLE) {
            ALNSOptimisation::saveRepair((*bestRoute));
        }
    }
    return 1;
}
