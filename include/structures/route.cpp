#include "route.hpp"

using namespace homecare;

Route::Route(Caregiver t_caregiver) 
        : m_caregiver(t_caregiver), m_currentTime (t_caregiver.getShiftStartTime()), 
        m_lastPatientDistanceIndex (t_caregiver.getDepotDistanceIndex()),
        m_depot(t_caregiver), m_depot2depot(false) {} 

Route::~Route() {}

int Route::getFreeTime() const { 
    if (m_arcs.size() <= 0) { return m_caregiver.getShiftStartTime(); }
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
    m_lastPatientDistanceIndex = t_newNode.getDistancesIndex();    

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

Json::Value Route::getJSONRoute() const {
    Json::Value route;
    const string  ARRIVAL_FIELD     ("arrival_time");
    const string  DEPARTURE_FIELD   ("departure_time");
    const string  PATIENT_FIELD     ("patient");
    const string  SERVICE_FIELD     ("service");

    for (int i = 0; i < m_arcs.size() - 1; ++i) {
        route[i][ARRIVAL_FIELD  ] = m_arcs[i].getArrvalTime();
        route[i][DEPARTURE_FIELD] = m_arcs[i + 1].getDeparturTime();
        route[i][PATIENT_FIELD  ] = m_arcs[i].getArrival().getId();
        route[i][SERVICE_FIELD  ] = m_arcs[i].getArrival().getService();
    }

    return route;
}

vector<string> Route::getAvilableServices() const { return m_caregiver.getServicesList(); }

string Route::getCaregiver() const { return m_caregiver.getID(); }

bool Route::isAvailable() const { return m_caregiver.isWorking(this -> getFreeTime()); }

vector<Arc> Route::getArcs() const { return m_arcs; }

int Route::readNodesFromJson (Json::Value t_patientsInJson, 
        vector<Patient> t_patients, vector<vector<int>> t_distances) {

    const string  PATIENT_FIELD     ("patient");
    const string  ARRIVAL_FIELD     ("arrival_time");
    const string  DEPARTURE_FIELD   ("departure_time");
    
    m_arcs.clear();
    m_depot2depot = false;
    Node last = m_depot;
    
    for (int i = 0; i < t_patientsInJson.size(); ++i) {
        string patientToSearch = t_patientsInJson[i][PATIENT_FIELD].asString();
        vector<Patient>::iterator patient = find(t_patients.begin(), t_patients.end(), 
            [patientToSearch] (const Patient p) { return p.getID() == patientToSearch; });
        if (patient == t_patients.end()) {
            throw runtime_error("Errore nel formato del file soluzione.");
            return -1;
        }
        Node next(*patient);
        int distance = t_distances[last.getDistancesIndex()][next.getDistancesIndex()];
        m_arcs.push_back(Arc(last, next, distance, 
                i > 0 ? t_patientsInJson[i - 1][DEPARTURE_FIELD].asInt() : m_caregiver.getShiftStartTime(), 
                t_patientsInJson[i][ARRIVAL_FIELD].asInt()));
        last = next;
    }
    //link to depot
    int distance = t_distances[last.getDistancesIndex()][m_depot.getDistancesIndex()];
    int departure = t_patientsInJson[t_patientsInJson.size() - 1][DEPARTURE_FIELD].asInt();
    m_arcs.push_back(Arc(last, m_depot, distance, departure, departure + distance));
    m_currentTime = getFreeTime();
    m_depot2depot = true;  
    
    return m_arcs.size() - 1;
}