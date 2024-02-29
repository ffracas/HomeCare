#include "greedyrepair.hpp"

using namespace std;
using namespace homecare;

GreedyRepair::GreedyRepair(ALNSOptimisation &t_repairOps) : NodeRepair(t_repairOps) {}

GreedyRepair::~GreedyRepair() {}

int GreedyRepair::repairNodes()
{
    while (m_repairOps.hasNodeToRepair())
    {
        // Best
        double bestCost = ALNSOptimisation::MAX_DOUBLE;
        RoutesOpt *bestRoute;
        // find patient
        Patient patient(HCData::getPatient(m_repairOps.popNodeToRepair()));
        RoutesOpt routesToTest(m_repairOps.getCurrentSol());
        // indipendet service
        if (!patient.hasNextService())
        {
            for (int i = 0; i < routesToTest.getNumberOfRoutes(); ++i)
            {
                if (routesToTest.isServiceAvailableInRoute(patient.getCurrentService().getService(), i))
                {
                    RoutesOpt newRoutes (m_repairOps.repairSingle(routesToTest, patient, i));
                    if (!newRoutes.isEmpty())
                    {
                        double cost = m_repairOps.calculateCost(newRoutes.getRoutes());
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
                        RoutesOpt newRoutes (m_repairOps.repairDouble(routesToTest, patient, i, j));
                        if (!newRoutes.isEmpty())
                        {
                            double cost = m_repairOps.calculateCost(newRoutes.getRoutes());
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
            m_repairOps.saveRepair(*bestRoute);
        }
    }
    return 1;
}
