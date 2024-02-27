#include "noderemoval.hpp"

using namespace std;
using namespace homecare;

const int NodeRemoval::MIN_ROUTE_LENGTH = 2;
const int NodeRemoval::NO_INDEX = -1;

NodeRemoval::NodeRemoval(ALNSOptimisation& t_ops, int t_minNodesN) 
        : m_removalOps (t_ops), MIN_N_NODES (t_minNodesN < MIN_ROUTE_LENGTH ? MIN_ROUTE_LENGTH : t_minNodesN) {}

int NodeRemoval::chooseRandomRoute() {
    vector<int> routesWithMinimumNodes;
    for (int n_route = 0; n_route < m_removalOps.getNumberOfRoutes(); ++n_route) {
        if (m_removalOps.getNumberOfNodesInRoute(n_route) > MIN_N_NODES) {
            routesWithMinimumNodes.push_back(n_route);
        }
    }

    if (routesWithMinimumNodes.empty()) { return NO_INDEX; }

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, routesWithMinimumNodes.size() - 1);
    int randomIndex = dis(gen);
    return routesWithMinimumNodes[randomIndex];
}

int NodeRemoval::chooseRandomNode(int n_route) {
    // If the route contains only depot and one node
    if (m_removalOps.getNumberOfNodesInRoute(n_route) < MIN_ROUTE_LENGTH) { return NO_INDEX; }
    if (m_removalOps.getNumberOfNodesInRoute(n_route) == MIN_ROUTE_LENGTH) { return 1; }

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, m_removalOps.getNumberOfNodesInRoute(n_route) - 2);
    return dis(gen) + 1;
}
