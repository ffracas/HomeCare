#include "clusterremoval.hpp"

using namespace std;
using namespace homecare;

ClusterRemoval::ClusterRemoval(int t_minNodesN) 
        : NodeRemoval (t_minNodesN) {}

ClusterRemoval::~ClusterRemoval() {}

void ClusterRemoval::removeNodes(int elementsToDestroy) {
    vector<pair<int, int>> nodesToRemove;
    // Select random node
    RoutesOpt routes(ALNSOptimisation::getCurrentSchedule());
    int n_route = chooseRandomRoute(routes);
    while (n_route != NO_INDEX) {
        KruskalGraph edges(routes.getNumberOfNodesInRoute(n_route));
        generateGraph(routes, n_route, edges);
        for(const int& node : edges.getKruskalRank(elementsToDestroy - nodesToRemove.size())) {
            nodesToRemove.push_back(make_pair(n_route, node));
        }
        if (nodesToRemove.size() < elementsToDestroy) 
        {   n_route = chooseRandomRoute(routes); }
        else { n_route = NO_INDEX; }
    }
    for (const pair<int, int> & node : nodesToRemove) {
        RoutesOpt newRoutes = ALNSOptimisation::destroy(routes, node.first, node.second);
        if (!newRoutes.isEmpty()) {
            routes = newRoutes;
        }
    }
}

void ClusterRemoval::generateGraph(RoutesOpt& routes, int n_route, KruskalGraph & edges) {
    for (int i = 1; i < routes.getNumberOfNodesInRoute(n_route); ++i) {
        Node n_i = routes.getNodeInRoute(n_route, i);
        for (int j = i; j < routes.getNumberOfNodesInRoute(n_route); ++j) {
            double likelihood = 99999.9;
            if (i != j) {
                Node n_j = routes.getNodeInRoute(n_route, j);
                likelihood = RelatedRemoval::calculateSimilarity(
                    HCData::getDistance(n_i.getDistancesIndex(), n_j.getDistancesIndex()),
                    abs(n_i.getTimeWindowOpen() - n_j.getTimeWindowOpen()),
                    abs(n_i.getTimeWindowClose() - n_j.getTimeWindowClose()),
                    n_i.getService(), n_j.getService());
            }
            edges.addWeightedEdge(likelihood, i, j);
        }
    }
}
