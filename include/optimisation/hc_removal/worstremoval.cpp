#include "worstremoval.hpp"

using namespace std;
using namespace homecare;

WorstRemoval::WorstRemoval(ALNSOptimisation& t_ops, double t_worseParam) 
        : NodeRemoval(t_ops), p_worst (t_worseParam) {}

WorstRemoval::~WorstRemoval() {}

void WorstRemoval::removeNodes(int elementsToDestroy) {
    //resetOperation();
    int n_des = 0;
    while (n_des < elementsToDestroy) {
        vector<CostCoord> worstList;
        for (int i = 0; i < m_removalOps.getNumberOfRoutes(); ++i) {
            for (int j = 1; j < m_removalOps.getNumberOfNodesInRoute(i); ++j) {
                vector<Route> solCopy(m_removalOps.getRoutes());
                m_removalOps.destroy(i, j, solCopy);
                double dif = m_removalOps.calculateCost(solCopy) - m_removalOps.getCurrentCost();
                worstList.push_back(CostCoord(dif, i, j));
                sort(worstList.begin(), worstList.end(), [] (CostCoord cc1, CostCoord cc2) { 
                                                                return cc1.getCost() > cc2.getCost();
                                                            });
            }
        }
        int pos = floor(pow(m_removalOps.generateRandom(), p_worst) * worstList.size());
        m_removalOps.destroy(worstList[pos].getRouteNumber(), worstList[pos].getNodePosition());
        n_des ++;
    }
}