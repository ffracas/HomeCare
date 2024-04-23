#include "schedule.hpp"

using namespace std;
using namespace homecare;

Schedule::Schedule(const vector<Caregiver>& t_caregivers) {
    for (auto& caregiver : t_caregivers) {
       m_routes.push_back(Route(caregiver));
    }
}

Schedule::~Schedule() {}

//////////////////////////////////////////////////////////////////////////////////////       UTILS

double Schedule::calculateSupposedCost(int n_route, Node nodeToAdd) const {
    // Base route cost
    double cost = m_routes[n_route].getCost() - m_routes[n_route].getLastNode2DepotDistance();
    // Add travel to the next node time cost 
    int distance = HCData::getDistance(m_routes[n_route].getLastPatientDistanceIndex(), nodeToAdd.getDistancesIndex());
    int arrival = m_routes[n_route].getFreeTime() + distance;
    int deltaTime = 0;
    if (arrival < nodeToAdd.getTimeWindowOpen()) {
        deltaTime = nodeToAdd.getTimeWindowOpen() - arrival;
    } else {
        if (arrival > nodeToAdd.getTimeWindowClose()) {
            deltaTime = arrival - nodeToAdd.getTimeWindowClose();
        } 
    }
    // calcolate supposed cost
    cost += distance * HCData::TRAVEL_TIME_WEIGHT + deltaTime * HCData::EXTRA_TIME_WEIGHT;
    return cost;
}

int Schedule::searchForRoute(Patient patient, int syncIndex) const {
    string request = patient.getCurrentService().getService();
    int bestRoute = HCData::NO_INDEX;
    double bestCost = HCData::MAX_COST;
    // searching for the best route
    for (int i = 0; i < m_routes.size(); ++i) {
        if (i != syncIndex 
            && m_routes[i].hasService(request)
            && patient.isCaregiverValid(m_routes[i].getCaregiver())
            ) {
            double cost = calculateSupposedCost(i, Node(patient, 0));
            if (cost < bestCost) {
                bestCost = cost;
                bestRoute = i;
            }
        }
    }
    return bestRoute;
}

int Schedule::getSupposedArrivalTime(int n_route, int distanceIndex, int openWT) const {
    int lastPatientIndex = m_routes[n_route].getLastPatientDistanceIndex();
    return max(
        m_routes[n_route].getFreeTime() + HCData::getDistance(lastPatientIndex, distanceIndex),
        openWT
    );
}

//////////////////////////////////////////////////////////////////////////////////////       GETTER

double Schedule::getCost() const { 
    int maxIdleTime      = 0;
    int maxTardiness     = 0;
    int totalTardiness   = 0;
    int totalWaitingTime = 0; 
    int travelTime       = 0;   
    int totalExtraTime   = 0;
    // calculate variables
    for (const Route& route : m_routes) {
        maxIdleTime = route.getMaxIdleTime() > maxIdleTime ? route.getMaxIdleTime() : maxIdleTime;
        maxTardiness = route.getMaxTardiness() > maxTardiness ? route.getMaxTardiness() : maxTardiness;
        totalTardiness += route.getTotalTardiness();
        totalWaitingTime += route.getTotalWaitingTime();
        travelTime += route.getTravelTime();
        totalExtraTime += route.getExtraTime();
    }
    // calculate cost and return
    return HCData::MAX_WAIT_TIME_WEIGHT * maxIdleTime + HCData::MAX_TARDINESS_WEIGHT * maxTardiness 
        + HCData::TARDINESS_WEIGHT * totalTardiness + HCData::TOT_WAITING_TIME_WEIGHT * totalWaitingTime  
        + HCData::EXTRA_TIME_WEIGHT * totalExtraTime + HCData::TRAVEL_TIME_WEIGHT * travelTime;
}

//////////////////////////////////////////////////////////////////////////////////////       ADD NODES

int  Schedule::greedyAppendNode(Patient patient) {
    int n_route = searchForRoute(patient, HCData::NO_INDEX);
    int time = getSupposedArrivalTime(n_route, patient.getDistancesIndex(), patient.getWindowStartTime());
    if (patient.getSync() != NoSync) {
        Patient syncPS(patient.getPatientAndNextService());
        int bestSync = searchForRoute(syncPS, n_route);
        int syncTime = getSupposedArrivalTime(bestSync, syncPS.getDistancesIndex(), syncPS.getWindowStartTime());
        if (patient.getSync() == Simultaneous) {
            time = max(time, syncTime);
            syncTime = time;
        }
        else if (patient.getSync() == Sequential) {
            if (syncTime > time + syncPS.getMaxWait()) {
                time = syncTime - syncPS.getMaxWait();
            }
            else if (syncTime < time + syncPS.getMinWait()) {
                syncTime = time + syncPS.getMinWait();
            }
        }
        m_routes[bestSync].appendNode(Node(syncPS, syncTime), syncTime); 
    }
    m_routes[n_route].appendNode(Node(patient, time), time);
}

// TODO: vedere se serve
int  Schedule::insertNode(int n_route, Patient patient) {
    return 0;
}
