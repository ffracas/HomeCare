#include "relatedremoval.hpp"

using namespace std;
using namespace homecare;

const double RelatedRemoval::m_related = 5.0;
const double RelatedRemoval::m_distanceWeight = 5.0;
const double RelatedRemoval::m_windowWeight = 5.0;
const double RelatedRemoval::m_serviceWeight = 5.0;

RelatedRemoval::RelatedRemoval(ALNSOptimisation& t_ops)
        : NodeRemoval (t_ops) {}

RelatedRemoval::~RelatedRemoval() {}

int RelatedRemoval::calculateSharedCaregivers(string service_i, string service_j, int& n_i, int& n_j) {
    vector<string> caregiver_i = HCData::getCaregiversForService(service_i);
    vector<string> caregiver_j = HCData::getCaregiversForService(service_j);
    n_i = caregiver_i.size();
    n_j = caregiver_j.size();
    int count = 0;
    for (int i = 0; i < n_i; ++i) {
        for (int j = 0; j < n_j; ++j) {
            if (caregiver_i[i] == caregiver_j[j]) { ++count; }
        }
    }
    return count; 
}

/**
 * Static method that calculates the similarity between two services based on distance, 
 * time window differences, and the number of shared caregivers normalised.
 * 
 * @param distance The distance between the services.
 * @param difOpenWin The difference in the opening time window.
 * @param difCloseWin The difference in the closing time window.
 * @param service_i The first service.
 * @param service_j The second service.
 * @return The similarity score between the services.
 */
double RelatedRemoval::calculateSimilarity(int distance, int difOpenWin, int difCloseWin, 
        string service_i, string service_j) {
    int caregivers_i = 0, caregivers_j = 0;
    int sharedCaregivers = calculateSharedCaregivers(service_i, service_j, caregivers_i, caregivers_j);
    int caregiversDenominator = caregivers_i > caregivers_j ? caregivers_j : caregivers_i;
    return (m_distanceWeight * distance)
            + m_windowWeight * (difOpenWin + difCloseWin)
            + m_serviceWeight * abs(1.0 - (double(sharedCaregivers) / caregiversDenominator));
}

pair<int, int> RelatedRemoval::getRandomNode(vector<pair<int, int>> list) {
    if (list.size() == 0) { throw runtime_error("error in get random node"); }
    if (list.size() == 1) { return list[0]; }
    int pos = rand() % list.size();
    return list[pos];
}

void RelatedRemoval::removeNodes(int elementsToDestroy) {
    //resetOperation();
    int n_route = chooseRandomRoute();
    int n_pos   = chooseRandomNode(n_route);
    Node seed   = m_removalOps.getNodeInRoute(n_route, n_pos);

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
                    double similarity = 
                            calculateSimilarity(distance, difOpenWin, difCloseWin, seed.getService(), p.getService());
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