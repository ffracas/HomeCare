#include "noderemoval.hpp"

using namespace std;
using namespace homecare;

const int NodeRemoval::MIN_ROUTE_LENGTH = 2;
const int NodeRemoval::NO_INDEX = -1;

NodeRemoval::NodeRemoval(int t_minNodesN) 
        : MIN_N_NODES (t_minNodesN < MIN_ROUTE_LENGTH ? MIN_ROUTE_LENGTH : t_minNodesN), 
        m_data (ALNSOptimisation::getInstance()) {}

int NodeRemoval::chooseRandomRoute(ScheduleOptimiser& t_routes) {
    std::vector<int> routesWithMinimumNodes;

    // Raccogliere tutte le rotte con almeno MIN_N_NODES nodi
    for (int routeNumber = 0; routeNumber < t_routes.getNumberOfRoutes(); ++routeNumber) {
        if (t_routes.getNumberOfNodesInRoute(routeNumber) >= MIN_N_NODES) {
            routesWithMinimumNodes.push_back(routeNumber);
        }
    }

    // Se non ci sono rotte con il numero minimo di nodi, restituire NO_INDEX
    if (routesWithMinimumNodes.empty()) {
        return NO_INDEX;
    }

    // Selezionare casualmente una rotta tra quelle con il numero minimo di nodi
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, routesWithMinimumNodes.size() - 1);
    return routesWithMinimumNodes[dis(gen)];
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
