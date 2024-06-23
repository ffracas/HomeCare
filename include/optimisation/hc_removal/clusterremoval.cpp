#include "clusterremoval.hpp"

using namespace std;
using namespace homecare;

ClusterRemoval::ClusterRemoval() : NodeRemoval (7) {}

ClusterRemoval::~ClusterRemoval() {}

void ClusterRemoval::removeNodes(int elementsToDestroyTarget) {
    vector<string> destroyedNodes;
    ScheduleOptimiser routes(m_data->getCurrentSchedule());

    int n_route = chooseRandomRoute(routes);
    while (n_route != NO_INDEX && destroyedNodes.size() < elementsToDestroyTarget) {
        cout << "----" << destroyedNodes.size() << "----\n";

        vector<string> nodesToRemove;
        KruskalGraph kruskalGraph(routes.getNumberOfNodesInRoute(n_route) - 1);
        generateGraph(routes, n_route, kruskalGraph);
        int numElementsToDestroyInRoute = elementsToDestroyTarget - destroyedNodes.size();

        for (const int& pos_node : kruskalGraph.getKruskalRank(numElementsToDestroyInRoute)) {
            nodesToRemove.push_back(routes.getNodeFromRoute(n_route, pos_node).getId());
        }

        for (const string& node : nodesToRemove) {
            int n_pos = routes.getNodePositionInRoute(n_route, node);
            ScheduleOptimiser newRoutes = m_data->destroy(routes, n_route, n_pos);
            if (!newRoutes.isEmpty()) {
                m_data->saveDestruction(newRoutes, n_route, n_pos);
                routes = newRoutes;
                destroyedNodes.push_back(node);
            }
        }

        if (destroyedNodes.size() < elementsToDestroyTarget) {
            n_route = chooseRandomRoute(routes);
        } else {
            n_route = NO_INDEX;
        }
    }
}

void ClusterRemoval::generateGraph(ScheduleOptimiser& routes, int n_route, KruskalGraph & edges) {
    for (int i = 1; i < routes.getNumberOfNodesInRoute(n_route) - 1; ++i) {
        Node n_i = routes.getNodeFromRoute(n_route, i);
        for (int j = i + 1; j < routes.getNumberOfNodesInRoute(n_route); ++j) {
            Node n_j = routes.getNodeFromRoute(n_route, j);
            double distance = HCData::getDistance(n_i.getDistancesIndex(), n_j.getDistancesIndex());
            edges.addWeightedEdge(i, j, distance);
        }
    }
}
