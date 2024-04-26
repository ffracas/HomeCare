#include "worstremoval.hpp"

using namespace std;
using namespace homecare;

WorstRemoval::WorstRemoval(double t_worseParam) 
        : NodeRemoval(3)/*, p_worst (t_worseParam)*/ {}

WorstRemoval::~WorstRemoval() {}

void WorstRemoval::removeNodes(int elementsToDestroy) {
    /*//resetOperation();
    int n_des = 0;
    ScheduleOptimiser routes(ALNSOptimisation::getCurrentSchedule());
    while (n_des < elementsToDestroy) {
        vector<CostCoord> worstList;
        for (int i = 0; i < ALNSOptimisation::getNumberOfRoutes(); ++i) {
            for (int j = 1; j < routes.getNumberOfNodesInRoute(i); ++j) {
                vector<Route> routeToTest;
                routeToTest.push_back(routes.getRoutes()[i]);
                ScheduleOptimiser solCopy(routes);
                solCopy = ALNSOptimisation::destroy(solCopy, i, j);
                double dif = (ALNSOptimisation::calculateCost(solCopy.getRoutes()) - ALNSOptimisation::getCurrentCost()) 
                            / ALNSOptimisation::calculateCost(routeToTest);
                worstList.push_back(CostCoord(dif, i, j));
                sort(worstList.begin(), worstList.end(), [] (CostCoord cc1, CostCoord cc2) { 
                                                                return cc1.getCost() > cc2.getCost();
                                                            });
            }
        }
        int pos = floor(pow(ALNSOptimisation::generateRandom(), p_worst) * worstList.size());
        ScheduleOptimiser newRoutes(ALNSOptimisation::destroy(routes,
                            worstList[pos].getRouteNumber(), worstList[pos].getNodePosition()));
        if (!newRoutes.isEmpty()) {
            routes = newRoutes;
        }
        n_des ++;
    }*/
}