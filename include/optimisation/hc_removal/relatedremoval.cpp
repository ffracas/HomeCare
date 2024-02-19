#include "relatedremoval.hpp"

using namespace std;
using namespace homecare;

RelatedRemoval::RelatedRemoval(ALNSOptimisation& t_ops, double t_related, 
        double t_distanceWeight, double t_windowWeight, double t_serviceWeight)
        : NodeRemoval (t_ops), m_related (t_related), 
        m_distanceWeight (t_distanceWeight), m_windowWeight (t_windowWeight), m_serviceWeight (t_serviceWeight) {}

RelatedRemoval::~RelatedRemoval() {}

double RelatedRemoval::calculateSimilarity(int distance, int difOpenWin, int difCloseWin, bool service, 
            int maxDist, int difTWO, int difTWC) const {
    double s = service ? 0 : m_serviceWeight;
    return (m_distanceWeight * distance) / maxDist 
            + m_windowWeight * (double(difOpenWin) / difTWO + double(difCloseWin) / difTWC)
            + s;
}

void RelatedRemoval::findMinMaxRelatetion(int& maxDist, int& minTWO, 
        int& maxTWO, int& minTWC, int& maxTWC, int n_route, int seedDistIndex) const {
    for (int i = 0; i < m_removalOps.getNumberOfNodesInRoute(n_route); ++i) {
        Node n = m_removalOps.getNodeInRoute(n_route, i);
        //controllo distanze
        int distance = HCData::getDistance(seedDistIndex, n.getDistancesIndex());
        maxDist = distance > maxDist ? distance : maxDist;

        //controllo finestra aperta
        if (n.getTimeWindowOpen() < minTWO) { minTWO = n.getTimeWindowOpen(); }
        else if (n.getTimeWindowOpen() > maxTWO) { maxTWO = n.getTimeWindowOpen(); }

        //controllo finestra chiusa
        if (n.getTimeWindowClose() < minTWC) { minTWC = n.getTimeWindowClose(); }
        else if (n.getTimeWindowClose() > maxTWC) { maxTWC = n.getTimeWindowClose(); }
    }
}

pair<int, int> RelatedRemoval::getRandomNode(vector<pair<int, int>> list) {
    if (list.size() == 0) { throw runtime_error("error in get random node"); }
    if (list.size() == 1) { return list[0]; }
    int pos = rand() % list.size();
    return list[pos];
}

void RelatedRemoval::removeNodes(int elementsToDestroy) {
    //resetOperation();
    int n_route = rand() % m_removalOps.getNumberOfRoutes();
    int n_pos   = rand() % m_removalOps.getNumberOfNodesInRoute(n_route);
    Node seed   = m_removalOps.getNodeInRoute(n_route, n_pos);
    int maxDist = 0;                                                    //min Distance fissato a 0 (nodo stesso)
    int minTWO  = seed.getTimeWindowOpen() , maxTWO = minTWO;
    int minTWC  = seed.getTimeWindowClose(), maxTWC = minTWC;
    findMinMaxRelatetion(maxDist, minTWO, maxTWO, minTWC, maxTWC, n_route, seed.getDistancesIndex());

    vector<CostCoord> similarityRank;
    vector<pair<int, int>> nodesToRemove;
    nodesToRemove.push_back({n_route, n_pos});
    while (nodesToRemove.size() < elementsToDestroy) {
        similarityRank.clear();
        pair<int, int> coord = getRandomNode(nodesToRemove);
        seed = m_removalOps.getNodeInRoute(coord.first, coord.second);
        for (int i = 0; i < m_removalOps.getNumberOfRoutes(); ++i) {
            for (int j = 1; j < m_removalOps.getNumberOfNodesInRoute(i); ++j) {
                if (i != n_route && j != n_pos) {
                    Node p = m_removalOps.getNodeInRoute(i, j);
                    int distance    = HCData::getDistance(seed.getDistancesIndex(), p.getDistancesIndex());
                    int difOpenWin  = abs(p.getTimeWindowOpen() - seed.getTimeWindowOpen());
                    int difCloseWin = abs(p.getTimeWindowClose()   - seed.getTimeWindowClose()); 
                    double similarity = calculateSimilarity(distance, difOpenWin, difCloseWin, 
                            seed.getService() == p.getService(), maxDist, minTWO - maxTWO, minTWC - maxTWC);
                    similarityRank.push_back(CostCoord(similarity, i, j));
                    sort(similarityRank.begin(), similarityRank.end(), [] (CostCoord cc1, CostCoord cc2) { 
                                                                    return cc1.getCost() < cc2.getCost();
                                                                });
                }
            }
        }
        int pos = floor(pow(m_removalOps.generateRandom(), m_related) * similarityRank.size());
        nodesToRemove.push_back({similarityRank[pos].getRouteNumber(), similarityRank[pos].getNodePosition()});
    }
    for (const pair<int, int>& coord : nodesToRemove) {
        m_removalOps.destroy(coord.first, coord.second);
    }
}