#include "worstremoval.hpp"

using namespace std;
using namespace homecare;

WorstRemoval::WorstRemoval() : NodeRemoval(7) {}

WorstRemoval::~WorstRemoval() {}

void WorstRemoval::removeNodes(int elementsToDestroy) {
    //resetOperation();
    int n_des = 0;
    ScheduleOptimiser routes(m_data->getCurrentSchedule());
    while (n_des < elementsToDestroy) {
        vector<CostCoord> worstList;
        for (int i = 0; i < routes.getNumberOfRoutes(); ++i) {
            for (int j = 1; j < routes.getNumberOfNodesInRoute(i); ++j) {
                double currentRouteCost = routes.getCost();
                ScheduleOptimiser solCopy(routes);
                solCopy = m_data->destroy(solCopy, i, j);
                if (!solCopy.isEmpty()) {
                    // (newCost - oldCost) / routeCost -> (cost difference / current)
                    double dif = (routes.getCost() - solCopy.getCost()) / currentRouteCost;
                    worstList.push_back(CostCoord(dif, i, j));  //fixme: aggiornare lista con priority list
                    sort(worstList.begin(), worstList.end(), [] (CostCoord cc1, CostCoord cc2) { 
                            return cc1.getCost() > cc2.getCost();
                        });
                }
            }
        }
        int pos = floor(pow(m_data->generateRandom(), p_worst) * worstList.size());
        if (pos == worstList.size()) { pos --;}
        ScheduleOptimiser newRoutes(m_data->destroy(routes,
                            worstList[pos].getRouteNumber(), worstList[pos].getNodePosition()));
        if (!newRoutes.isEmpty()) {
            m_data->saveDestruction(newRoutes, worstList[pos].getRouteNumber(), worstList[pos].getNodePosition());
            routes = newRoutes;
            n_des ++;
        }
    }
}
