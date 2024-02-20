#include "clusterremoval.hpp"

using namespace std;
using namespace homecare;

const int ClusterRemoval::MIN_N_NODES = 3;

ClusterRemoval::ClusterRemoval(ALNSOptimisation& t_ops) : NodeRemoval (t_ops) {}

ClusterRemoval::~ClusterRemoval() {}

void ClusterRemoval::removeNodes(int elementsToDestroy) {
    vector<pair<int, int>> nodesToRemove;
    bool stop = false;
    while (nodesToRemove.size() < elementsToDestroy && !stop) {
        // Select random node
        int n_route = chooseRandomRoute();
        int i = 0;
        int max_iter = m_removalOps.getNumberOfRoutes();
        while (m_removalOps.getNumberOfNodesInRoute(n_route) > MIN_N_NODES && !stop) {
            if (i >= max_iter) { stop = true; }
            ++ n_route;
            ++ i;
        }
        if (!stop){
            KruskalGraph edges(m_removalOps.getNumberOfNodesInRoute(n_route));
            generateGraph(n_route, edges);
            for(const int& node : edges.getKruskalRank(elementsToDestroy)) {
                nodesToRemove.push_back(make_pair(n_route, node));
            }
        }
    }
    for (const pair<int, int> & node : nodesToRemove) {
        m_removalOps.destroy(node.first, node.second);
    }
}

void ClusterRemoval::generateGraph(int n_route, KruskalGraph & edges) {
    for (int i = 1; i < m_removalOps.getNumberOfNodesInRoute(n_route); ++i) {
        Node n_i = m_removalOps.getNodeInRoute(n_route, i);
        for (int j = i; j < m_removalOps.getNumberOfNodesInRoute(n_route); ++j) {
            double likelihood = 99999.9;
            if (i != j) {
                Node n_j = m_removalOps.getNodeInRoute(n_route, j);
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
