#include "worstremoval.hpp"

using namespace std;
using namespace homecare;

WorstRemoval::WorstRemoval(ALNSOptimisation& t_ops, double t_worseParam) 
        : NodeRemoval(t_ops), p_worst (t_worseParam) {}

WorstRemoval::~WorstRemoval() {}

void WorstRemoval::removeNodes(int elementsToDestroy) {
    //resetOperation();
    int n_des = 0;
    RoutesOpt routes(m_removalOps.getCurrentSol());
    while (n_des < elementsToDestroy) {
        vector<CostCoord> worstList;
        for (int i = 0; i < m_removalOps.getNumberOfRoutes(); ++i) {
            for (int j = 1; j < routes.getNumberOfNodesInRoute(i); ++j) {
                vector<Route> routeToTest;
                routeToTest.push_back(routes.getRoutes()[i]);
                RoutesOpt solCopy(routes);
                solCopy = m_removalOps.destroy(solCopy, i, j);
                double dif = (m_removalOps.calculateCost(solCopy.getRoutes()) - m_removalOps.getCurrentCost()) 
                            / m_removalOps.calculateCost(routeToTest);
                worstList.push_back(CostCoord(dif, i, j));
                sort(worstList.begin(), worstList.end(), [] (CostCoord cc1, CostCoord cc2) { 
                                                                return cc1.getCost() > cc2.getCost();
                                                            });
            }
        }
        int pos = floor(pow(m_removalOps.generateRandom(), p_worst) * worstList.size());
        RoutesOpt newRoutes(m_removalOps.destroy(routes,
                            worstList[pos].getRouteNumber(), worstList[pos].getNodePosition()));
        if (!newRoutes.isEmpty()) {
            routes = newRoutes;
        }
        n_des ++;
    }
}