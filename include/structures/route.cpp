#include "route.hpp"

using namespace homecare;

Route::Route(Caregiver t_caregiver) 
        : m_caregiver(t_caregiver), m_currentTime (t_caregiver.getShiftStartTime()), 
        m_lastPatientDistanceIndex (t_caregiver.getDepotDistanceIndex()),
        m_depot(t_caregiver), m_depot2depot(false) {} 

Route::~Route() {}

int Route::getFreeTime() const { 
    if (m_arcs.size() <= 0) { return m_depot.getTimeWindowOpen(); }
    return m_depot2depot ? m_arcs[m_arcs.size() - 2].getReadyTime() : m_arcs[m_arcs.size() - 1].getReadyTime(); 
}

int Route::getlastPatientDistanceIndex() const { return m_lastPatientDistanceIndex; }

int Route::addNode(Node t_newNode, vector<int> t_depDists, vector<int> t_arrDists, int t_delay) { 
    Node last = m_depot;
    if (m_arcs.size() >= BASE_ROUTE_LEN) {
        m_arcs.erase(m_arcs.end() - 1);  
        last = Node(m_arcs[m_arcs.size() - 1].getArrival());
        m_depot2depot = false;
    }

    //insert new arc
    m_currentTime = getFreeTime();
    int distance = t_depDists[t_newNode.getDistancesIndex()];
    m_arcs.push_back(Arc(last, t_newNode, distance, m_currentTime, t_delay));    

    //link to depot
    m_currentTime = getFreeTime();
    distance = t_arrDists[m_depot.getDistancesIndex()];
    m_arcs.push_back(Arc(t_newNode, m_depot, distance, m_currentTime, m_currentTime + distance));  
    m_depot2depot = true;  
    
    return m_arcs.size() - 1;
} 

string Route::getRouteToString() const {
    stringstream ss;
    
    ss << " ROUTE\n";
    for (const Arc &arc: m_arcs) {
        ss << arc.toString() << '\n';
    }

    return ss.str();
}

vector<string> Route::getAvilableServices() const { return m_caregiver.getServicesList(); }

string Route::getCaregiver() const { return m_caregiver.getID(); }