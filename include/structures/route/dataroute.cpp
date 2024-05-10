#include "dataroute.hpp"

using namespace std;
using namespace homecare;

DataRoute::DataRoute() { 
    m_nodes.clear();
    m_cost = HCData::MAX_COST;
}

DataRoute::DataRoute(Node node, int startTime, int distance) {
    m_nodes.push_back(node);
    calcolateCost(startTime, distance);
}

DataRoute::~DataRoute() {}

void DataRoute::calcolateCost(int startTime, int distance) {
    int estimedTime = startTime;
    int maxTardiness = 0, maxIdleTime = 0;
    int totalTardiness = 0, totalWaitingTime = 0;
    int travelTime = 0;

    vector<Node>::iterator it = m_nodes.begin();
    do {
        if (it->isInterdependent() && it->getTimeWindowClose() < estimedTime) {
            m_cost = HCData::MAX_COST;
            return;
        }
        travelTime += distance;
        if (it->getTimeWindowClose() < estimedTime) { 
            int tardiness = estimedTime - it->getTimeWindowClose();
            totalTardiness += tardiness; 
            maxTardiness = max(maxTardiness, tardiness);
        }
        else if (it->getTimeWindowOpen() > estimedTime) { 
            int idleTime = it->getTimeWindowOpen() - estimedTime;
            totalWaitingTime += idleTime; 
            maxIdleTime = max(maxIdleTime, idleTime);
        }
        it->setArrivalTime(estimedTime);
        estimedTime = it->getDeparturTime();
        it++;
        distance = HCData::getDistance((it- 1)->getDistancesIndex(), it->getDistancesIndex());
    } while (it != m_nodes.end());

    m_cost = HCData::MAX_WAIT_TIME_WEIGHT * maxIdleTime + HCData::MAX_TARDINESS_WEIGHT * maxTardiness 
        + HCData::TARDINESS_WEIGHT * totalTardiness + HCData::TOT_WAITING_TIME_WEIGHT * totalWaitingTime 
        + HCData::TRAVEL_TIME_WEIGHT * travelTime;
}

void DataRoute::addNode(Node node, int startTime, int distance) {
    m_nodes.insert(m_nodes.begin(), node);
    if (m_cost == HCData::MAX_COST) { return; }
    calcolateCost(startTime, distance);
}

vector<Node> DataRoute::getNodes() { return m_nodes; }

double DataRoute::getCost() { return m_cost; }
