#include "noderemoval.hpp"

using namespace std;
using namespace homecare;

const int NodeRemoval::MIN_ROUTE_LENGTH = 2;
const int NodeRemoval::NO_INDEX = -1;

NodeRemoval::NodeRemoval(int t_minNodesN) 
        : MIN_N_NODES (t_minNodesN < MIN_ROUTE_LENGTH ? MIN_ROUTE_LENGTH : t_minNodesN), 
        m_data (ALNSOptimisation::getInstance()) {}

int NodeRemoval::chooseRandomRoute(ScheduleOptimiser& t_routes) {
    vector<int> routesWithMinimumNodes;
    for (int n_route = 0; n_route < t_routes.getNumberOfRoutes(); ++n_route) {
        if (t_routes.getNumberOfNodesInRoute(n_route) > MIN_N_NODES) {
            routesWithMinimumNodes.push_back(n_route);
        }
    }

    if (routesWithMinimumNodes.empty()) { return NO_INDEX; }

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, routesWithMinimumNodes.size() - 1);
    int randomIndex = dis(gen);
    return routesWithMinimumNodes[randomIndex];
    return 0;
}

int NodeRemoval::chooseRandomNode(ScheduleOptimiser& t_routes, int n_route) {
    // If the route contains only depot and one node
    if (t_routes.getNumberOfNodesInRoute(n_route) < MIN_ROUTE_LENGTH) { return NO_INDEX; }
    if (t_routes.getNumberOfNodesInRoute(n_route) == MIN_ROUTE_LENGTH) { return 1; }

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, t_routes.getNumberOfNodesInRoute(n_route) - 2);
    return dis(gen) + 1;
}
