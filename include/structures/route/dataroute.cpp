#include "dataroute.hpp"

using namespace std;
using namespace homecare;

DataRoute::DataRoute() : m_cost (HCData::MAX_COST) {}

DataRoute::DataRoute(Node node) {
    m_nodes.push_back(node);
    m_cost = 0;
}

DataRoute::~DataRoute() {}

void DataRoute::addNode(Node node, int nodeOpenWin, int nodeCloseWin, bool isLast) {
    Node last = m_nodes.back();
    int arrivingTime = last.getDeparturTime() + HCData::getDistance(last.getDistancesIndex(), node.getDistancesIndex());
    // check validity
    if (node.isInterdependent()) {
        if (arrivingTime > nodeCloseWin) {
            m_cost = HCData::MAX_COST;
            m_nodes.clear();
            return;
        }
        else if (arrivingTime < nodeOpenWin) {
            int waitingTime = nodeOpenWin - arrivingTime;
            m_totalWaitingTime += waitingTime;
            m_maxIdleTime = max(m_maxIdleTime, waitingTime);
            arrivingTime = nodeOpenWin;
        }
    }

    // calculate cost variables
    m_travelTime += HCData::getDistance(last.getDistancesIndex(), node.getDistancesIndex());
    if (arrivingTime > node.getTimeWindowClose()) {
        int tardiness = arrivingTime - node.getTimeWindowClose();
        m_totalTardiness += tardiness;
        m_maxTardiness = max(m_maxTardiness, tardiness);
    }
    if (arrivingTime < node.getTimeWindowOpen()) {
        int waitingTime = node.getTimeWindowOpen() - arrivingTime;
        m_totalWaitingTime += waitingTime;
        m_maxIdleTime = max(m_maxIdleTime, waitingTime);
    }
    if (!m_completed && isLast) {
        // link last node to depot 
        m_completed = true;
        int lastToDepot = HCData::getDistance(node.getDistancesIndex(), m_nodes[0].getDistancesIndex());
        int returnToDepot = node.getDeparturTime() + lastToDepot;
        m_nodes[0].setArrivalTime(returnToDepot);
        m_extraTime = max(0, returnToDepot);
        // calculate cost of the route only after the last node is added
        m_cost = HCData::MAX_WAIT_TIME_WEIGHT * m_maxIdleTime + HCData::MAX_TARDINESS_WEIGHT * m_maxTardiness 
        + HCData::TARDINESS_WEIGHT * m_totalTardiness + HCData::TOT_WAITING_TIME_WEIGHT * m_totalWaitingTime 
        + HCData::EXTRA_TIME_WEIGHT * m_extraTime + HCData::TRAVEL_TIME_WEIGHT * m_travelTime; 
    }
    // push the node to the route
    node.setArrivalTime(arrivingTime);
    m_nodes.push_back(node);
}

vector<Node> DataRoute::getNodes() { 
    if (m_completed) {
        return m_nodes;
    }
    return vector<Node>(); 
}

double DataRoute::getCost() { return m_cost; }