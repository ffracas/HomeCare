#include "route.hpp"

using namespace std;
using namespace homecare;

const int Route::BASE_ROUTE_LEN = 2;
const int Route::DEPOT = 0;

Route::Route(Caregiver t_caregiver) : m_caregiver(t_caregiver) {
    m_nodes.push_back(Node(t_caregiver));
    m_maxTardiness = 0;
    m_maxIdleTime = 0;
    m_totalTardiness = 0;
    m_totalWaitingTime = 0;
    m_travelTime = 0;
    m_lastNode2DepotDistance = 0;
} 

Route::~Route() {}

int Route::getFreeTime() const { return m_nodes[m_nodes.size() - 1].getDeparturTime(); }

int Route::getlastPatientDistanceIndex() const { return m_nodes[m_nodes.size() - 1].getDistancesIndex(); }

int Route::addNode(Patient t_newPatient, vector<int> t_arrDists, int t_estimatedArrivalTime, int t_last2newDistance) { 
    //update cost data
    //sub distance of the last arc
    m_travelTime -= m_lastNode2DepotDistance - t_last2newDistance;
    //update waiting time
    if (t_estimatedArrivalTime < t_newPatient.getWindowStartTime()){
        int earliness = t_newPatient.getWindowStartTime() 
                        - (m_nodes[m_nodes.size() - 1].getDeparturTime() + t_last2newDistance);
        m_totalWaitingTime += earliness;
        m_maxIdleTime = earliness > m_maxIdleTime ? earliness : m_maxIdleTime;
        t_estimatedArrivalTime = t_newPatient.getWindowStartTime();
    }
    //update tardiness
    if (t_estimatedArrivalTime > t_newPatient.getWindowEndTime()) {
        int tardiness = t_estimatedArrivalTime - t_newPatient.getWindowEndTime();
        m_totalTardiness += tardiness;
        m_maxTardiness = tardiness > m_maxTardiness ? tardiness : m_maxTardiness;
    }
    
    //add new node
    m_nodes.push_back(Node(t_newPatient, t_estimatedArrivalTime));
    m_lastNode2DepotDistance = t_arrDists[m_caregiver.getDepotDistanceIndex()];
    m_nodes[DEPOT].setArrivalTime(m_nodes[m_nodes.size() - 1].getDeparturTime() + m_lastNode2DepotDistance);

    //update cost data (travel time)
    m_travelTime += m_lastNode2DepotDistance;

    return m_nodes.size() - 1;
} 

string Route::getRouteToString() const {
    stringstream ss;
    
    ss << " ROUTE\n";
    for (const Node &node: m_nodes) {
        ss << node.toString() << '\n';
    }

    return ss.str();
}

Json::Value Route::getJSONRoute() const {
    Json::Value route;
    const string  ARRIVAL_FIELD     ("arrival_time");
    const string  DEPARTURE_FIELD   ("departure_time");
    const string  PATIENT_FIELD     ("patient");
    const string  SERVICE_FIELD     ("service");

    //start from the first patient
    for (int i = 1, j = 0; i < m_nodes.size(); ++i, ++j) {
        route[j][ARRIVAL_FIELD  ] = m_nodes[i].getArrivalTime();
        route[j][DEPARTURE_FIELD] = m_nodes[i].getDeparturTime();
        route[j][PATIENT_FIELD  ] = m_nodes[i].getId();
        route[j][SERVICE_FIELD  ] = m_nodes[i].getService();
    }

    return route;
}

/**
 * Returns the list of available services for the route.
 * @return The list of available services for the route.
 */
vector<string> Route::getAvilableServices() const { return m_caregiver.getServicesList(); }

/**
 * Returns the ID of the caregiver associated with the route.
 * @return The ID of the caregiver associated with the route.
 */
string Route::getCaregiver() const { return m_caregiver.getID(); }

/**
 * Returns the number of nodes in the route.
 * @return The number of nodes in the route.
 */
int Route::getNumNodes() const { return m_nodes.size(); }

/**
 * Returns the node at the specified position in the route.
 * @param pos The position of the node.
 * @return The node at the specified position in the route.
 * @throws std::runtime_error if the route contains only one node.
 */
Node Route::getNode(int pos) const { 
    int nNodes = m_nodes.size();
    if (nNodes == 1 || pos >= nNodes) { throw std::runtime_error("Constraint on route selection."); }
    if (pos != DEPOT) { return m_nodes[pos]; }
    return m_nodes[pos + 1];
}

/**
 * Returns the maximum tardiness for the route.
 * @return The maximum tardiness for the route.
 */
int Route::getMaxTardiness() const { return m_maxTardiness; }

/**
 * Returns the maximum idle time for the route.
 * @return The maximum idle time for the route.
 */
int Route::getMaxIdleTime() const { return m_maxIdleTime; }

/**
 * Returns the total tardiness for the route.
 * @return The total tardiness for the route.
 */
int Route::getTotalTardiness() const { return m_totalTardiness; }

/**
 * Returns the total waiting time for the route.
 * @return The total waiting time for the route.
 */
int Route::getTotalWaitingTime() const { return m_totalWaitingTime; }

/**
 * Returns the travel time for the route.
 * @return The travel time for the route.
 */
int Route::getTravelTime() const { return m_travelTime; }

int Route::getExtraTime() const { 
    int endTime = m_nodes[DEPOT].getArrivalTime() - m_caregiver.getShiftEndTime();
    return endTime > 0 ? endTime : 0;
}

string Route::getHash() const {
    stringstream ss;
    
    ss << m_caregiver.getID() << "--";
    for (const Node &node: m_nodes) { ss << node.getHash() << '-'; }
    ss << "--";
    return ss.str();
}

bool Route::isAvailable() const { return m_caregiver.isWorking(this -> getFreeTime()); }

bool Route::hasService(string request) const { 
    vector<string> availableServices = m_caregiver.getServicesList();
    return find(availableServices.begin(), availableServices.end(), request) != availableServices.end(); 
}

vector<Node> Route::getNodes() const { return m_nodes; }

int Route::readNodesFromJson (Json::Value t_patientsInJson, vector<Patient> t_patients, vector<int> t_depotDistances) {

    const string PATIENT_FIELD      ("patient");
    const string SERVICE_FIELD      ("service");
    const string ARRIVAL_FIELD      ("arrival_time");
    const string DEPARTURE_FIELD    ("departure_time");

    for (int i = 0; i < t_patientsInJson.size(); ++i) {
        string patientToSearch = t_patientsInJson[i][PATIENT_FIELD].asString();
        vector<Patient>::iterator patient = find_if(t_patients.begin(), t_patients.end(), 
            [patientToSearch] (const Patient p) { return p.getID() == patientToSearch; });
        if (patient == t_patients.end()) {
            throw runtime_error("Errore nel file soluzione. Paziente non trovato.");
        }
        if (patient -> getCurrentService().getService() != t_patientsInJson[i][SERVICE_FIELD].asString()) {
            *patient = patient -> getPatientAndNextService();
            if (patient -> getCurrentService().getService() != t_patientsInJson[i][SERVICE_FIELD].asString()) {
                throw runtime_error("Errore nel file soluzione. Servizio non associato al paziente.");
            }
        }
        m_nodes.push_back(Node(*patient, t_patientsInJson[i][ARRIVAL_FIELD].asInt()));
        if (m_nodes[m_nodes.size() - 1].getDeparturTime() != t_patientsInJson[i][DEPARTURE_FIELD].asInt()) {
            throw runtime_error("Errore nel file soluzione. Problema nei tempi.");
        }
    }
    //link to depot
    m_nodes[DEPOT].setArrivalTime(m_nodes[m_nodes.size() - 1].getDeparturTime() + 
                                    t_depotDistances[m_nodes[m_nodes.size() - 1].getDistancesIndex()]);
    
    return m_nodes.size() - 1;
}