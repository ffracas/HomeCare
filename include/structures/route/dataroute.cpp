#include "dataroute.hpp"

using namespace std;
using namespace homecare;

DataRoute::DataRoute() : m_cost (HCData::MAX_COST) {}

DataRoute::DataRoute(Node node) {
    m_nodes.push_back(node);
}

DataRoute::DataRoute(vector<Node>& nodes, const SyncWindows& sw) {
    m_nodes.push_back(nodes[0]);
    
    if (nodes.back().isInterdependent() && nodes.back().getArrivalTime() > nodes.back().getTimeWindowClose()) {
        resetToValid(nodes, sw);
        cout<< this->getCost() << endl;
        return;
    }
    for (int i = 1; i < nodes.size(); ++i) {
        if (nodes[i].isInterdependent()) {
            this->addNode(nodes[i], 
                        sw.getOpenSyncTime(nodes[i].getId()), 
                        sw.getCloseSyncTime(nodes[i].getId()),
                        i == nodes.size() - 1);
        }
        else {
            this->addNode(nodes[i], -1, -1, i == nodes.size() - 1);
        }
    }
}

void DataRoute::resetToValid(vector<Node>& nodes, const SyncWindows& sw) {
    vector<Node> interdep;
    vector<Node> indep;
    for (int i = 1; i < nodes.size(); ++i) {
        if (nodes[i].isInterdependent()) {
            interdep.push_back(nodes[i]);
        }
        else {
            indep.push_back(nodes[i]);
        }
    }
    sort(interdep.begin(), interdep.end(), 
        [&sw](const Node& a, const Node& b) { return sw.getCloseSyncTime(a.getId()) < sw.getCloseSyncTime(b.getId()); });
    //todo: delete
    for (auto& node : interdep) {
        for (auto& node : m_nodes) {
            cout<<node.getId()<<" arrivo "<<node.getArrivalTime()<<" partenza "<<node.getDeparturTime()<<"->";
        }
        cout<<endl;
        this->addNode(node,
                    sw.getOpenSyncTime(node.getId()),
                    sw.getCloseSyncTime(node.getId()),
                    false);
    }
    for (int i = 0; i < indep.size(); ++i) {
        this->addNode(indep[i], -1, -1, i == indep.size() - 1);
    }
}

DataRoute::~DataRoute() {}

void DataRoute::addNode(Node node, int nodeOpenWin, int nodeCloseWin, bool isLast) {
    // if route is invalid
    if (m_cost == HCData::MAX_COST) { 
        m_nodes.push_back(node);
        return;
    }
    //else route is valid
    Node last = m_nodes.back();
    int arrivingTime = last.getDeparturTime() + HCData::getDistance(last.getDistancesIndex(), node.getDistancesIndex());
    // check validity
    if (node.isInterdependent()) {
        if (arrivingTime > nodeCloseWin) {
            m_cost = HCData::MAX_COST;
            m_nodes.push_back(node);
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

void DataRoute::twoOptSwap(int xi, int yi, const SyncWindows& sw) {
    if (xi < 1 || yi < 2 || xi >= m_nodes.size() || yi >= m_nodes.size() || xi == yi) {
        throw runtime_error("[DataRoute]: Error: invalid index");
    }
    vector<Node> oldNodes(m_nodes);
    m_nodes.clear();
    resetCost();
    m_nodes.push_back(oldNodes[0]);
    for (int k = 1; k < xi; ++k) {
        addNode(oldNodes[k], sw, false);
    }
    for (int k = yi; k >= xi; --k) {
        addNode(oldNodes[k], sw, (xi == k && yi == oldNodes.size() - 1));
    }
    for (int k = yi + 1; k < oldNodes.size(); ++k) {
        addNode(oldNodes[k], sw, k == oldNodes.size() - 1); 
    }
}

void DataRoute::addNode(Node& node, const SyncWindows& sw, bool isLast) {
    if (node.isInterdependent()) {
        addNode(node, sw.getOpenSyncTime(node.getId()), sw.getCloseSyncTime(node.getId()), isLast);
    } else {
        addNode(node, -1, -1, isLast);
    }
}

void DataRoute::resetCost() {
    m_cost = 0;
    m_maxIdleTime = 0;
    m_maxTardiness = 0;
    m_totalTardiness = 0;
    m_totalWaitingTime = 0;
    m_travelTime = 0;
    m_extraTime = 0;
    m_completed = false;
}

vector<Node> DataRoute::getNodes() { 
    return m_completed ? m_nodes : vector<Node>();
}

double DataRoute::getCost() { return m_cost; }