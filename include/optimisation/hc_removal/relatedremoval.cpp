#include "relatedremoval.hpp"

using namespace std;
using namespace homecare;

const double RelatedRemoval::m_related_exp_coef = 5.0;
const double RelatedRemoval::m_distanceWeight = 5.0;
const double RelatedRemoval::m_windowWeight = 5.0;
const double RelatedRemoval::m_serviceWeight = 5.0;

RelatedRemoval::RelatedRemoval()
        : NodeRemoval (1) {}

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

//fixme: const e & in parametri
Node RelatedRemoval::getRandomNode(const vector<Node>& list) {
    if (list.empty()) { throw runtime_error("[Coord] error in get random node"); }
    if (list.size() == 1) { return list[0]; }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, list.size() - 1);
    int pos = dist(gen);
    return list[pos];
}

void RelatedRemoval::removeNodes(int elementsToDestroy) {
    //resetOperation();
    ScheduleOptimiser routes(m_data->getCurrentSchedule());
    int n_route = chooseRandomRoute(routes);
    if (n_route == NO_INDEX) { return; }
    int n_pos   = chooseRandomNode(routes, n_route);
    if (n_pos == NO_INDEX) { return; }
    Node seed = routes.getNodeFromRoute(n_route, n_pos);

    vector<Node> deletedNodes;
    while (deletedNodes.size() < elementsToDestroy) {
        routes = m_data->getCurrentSchedule();
        vector<CostCoord> similarityRank;
        for (int i = 0; i < routes.getNumberOfRoutes(); ++i) {
            for (int j = 1; j < routes.getNumberOfNodesInRoute(i); ++j) {
                if (i != n_route && j != n_pos) {
                    Node p = routes.getNodeFromRoute(i, j);
                    int distance    = HCData::getDistance(seed.getDistancesIndex(), p.getDistancesIndex());
                    int difOpenWin  = abs(p.getTimeWindowOpen()  - seed.getTimeWindowOpen() );
                    int difCloseWin = abs(p.getTimeWindowClose() - seed.getTimeWindowClose()); 
                    double similarity = 
                            calculateSimilarity(distance, difOpenWin, difCloseWin, seed.getService(), p.getService());
                            
                    similarityRank.push_back(CostCoord(similarity, i, j));
                    sort(similarityRank.begin(), similarityRank.end(), [] (CostCoord cc1, CostCoord cc2) { 
                                                                    return cc1.getCost() < cc2.getCost();
                                                                });
                }
            }
        }
        int pos = floor(pow(ALNSOptimisation::generateRandom(), m_related_exp_coef) * similarityRank.size());
        Node toDelete =
            routes.getNodeFromRoute(similarityRank[pos].getRouteNumber(), similarityRank[pos].getNodePosition());
        ScheduleOptimiser newRoutes(
            m_data->destroy(routes, similarityRank[pos].getRouteNumber(), similarityRank[pos].getNodePosition()));
        if (!newRoutes.isEmpty()) {
            deletedNodes.push_back(toDelete);
            seed = getRandomNode(deletedNodes);
            cout<<toDelete.getId()<<" cancellato\n";
            m_data->saveDestruction(newRoutes, 
                                similarityRank[pos].getRouteNumber(), similarityRank[pos].getNodePosition());
        }
    }
}