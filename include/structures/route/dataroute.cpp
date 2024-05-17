#include "dataroute.hpp"

using namespace std;
using namespace homecare;

DataRoute::DataRoute() : m_cost (HCData::MAX_COST) {}

//todo: cambio logica di data route, valutare solo il costo dell'albero sottostante (togliere startTime e distance)
DataRoute::DataRoute(Node node, int startTime, int distance, const SyncWindows& syncWindows) {
    m_nodes.push_back(node);
    calculateCost(startTime, distance, syncWindows);
}

DataRoute::~DataRoute() {}

void DataRoute::calculateCost(int startTime, int distance, const SyncWindows& syncWindows) {
    int estimatedTime = startTime;
    int maxTardiness = 0, maxIdleTime = 0;
    int totalTardiness = 0, totalWaitingTime = 0;
    int travelTime = 0;

    for (auto it = m_nodes.begin(); it != m_nodes.end(); ++it) {
        int startingTime = it->getTimeWindowOpen();

        if (it->isInterdependent()) {
            auto syncWindow = syncWindows.getSyncWindow(it->getId());
            startingTime = std::max(startingTime, syncWindow.first);

            if (syncWindow.second < estimatedTime) {
                m_cost = HCData::MAX_COST;
                return;
            }
        }

        travelTime += distance;

        if (it->getTimeWindowClose() < estimatedTime) {
            int tardiness = estimatedTime - it->getTimeWindowClose();
            totalTardiness += tardiness;
            maxTardiness = std::max(maxTardiness, tardiness);
        } else if (startingTime > estimatedTime) {
            int idleTime = startingTime - estimatedTime;
            totalWaitingTime += idleTime;
            maxIdleTime = std::max(maxIdleTime, idleTime);
        }

        it->setArrivalTime(estimatedTime);
        estimatedTime = it->getDeparturTime();

        // Update the distance for the next iteration, if not at the end
        if (std::next(it) != m_nodes.end()) {
            distance = HCData::getDistance(it->getDistancesIndex(), std::next(it)->getDistancesIndex());
        }
    }

    m_cost = HCData::MAX_WAIT_TIME_WEIGHT * maxIdleTime +
             HCData::MAX_TARDINESS_WEIGHT * maxTardiness +
             HCData::TARDINESS_WEIGHT * totalTardiness +
             HCData::TOT_WAITING_TIME_WEIGHT * totalWaitingTime +
             HCData::TRAVEL_TIME_WEIGHT * travelTime;
}

//todo: cambio logica di data route, valutare solo il costo dell'albero sottostante (togliere startTime e distance)
void DataRoute::addNode(Node node, int startTime, int distance, const SyncWindows& syncWindows) {
    m_nodes.insert(m_nodes.begin(), node);
    if (m_cost == HCData::MAX_COST) { return; }
    calculateCost(startTime, distance, syncWindows);
}

vector<Node> DataRoute::getNodes() { return m_nodes; }

double DataRoute::getCost() { return m_cost; }

int DataRoute::getFirstDistanceIndex() {
  if (m_nodes.empty()) { return -1; }
  return m_nodes[0].getDistancesIndex();
}