#include "route.hpp"

using namespace std;
using namespace homecare;

const int Route::BASE_ROUTE_LEN = 2;
const int Route::DEPOT = 0;

Route::Route(Caregiver t_caregiver) 
        : m_caregiver(t_caregiver) {
            m_nodes.push_back(Node(t_caregiver));
        } 

Route::~Route() {}

int Route::getFreeTime() const { return m_nodes[m_nodes.size() - 1].getDeparturTime(); }

int Route::getlastPatientDistanceIndex() const { return m_nodes[m_nodes.size() - 1].getDistancesIndex(); }

int Route::addNode(Patient t_newPatient, vector<int> t_arrDists, int t_estimatedArrivalTime) { 
    t_estimatedArrivalTime = t_estimatedArrivalTime >= t_newPatient.getWindowStartTime() ? 
                                t_estimatedArrivalTime : t_newPatient.getWindowStartTime();
    m_nodes.push_back(Node(t_newPatient, t_estimatedArrivalTime));
    m_nodes[DEPOT].setArrivalTime(
        m_nodes[m_nodes.size() - 1].getDeparturTime() + t_arrDists[m_nodes[DEPOT].getDistancesIndex()]);

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
    for (int i = 1, j = 0; i < m_nodes.size() - 1; ++i, ++j) {
        route[j][ARRIVAL_FIELD  ] = m_nodes[i].getArrivalTime();
        route[j][DEPARTURE_FIELD] = m_nodes[i].getDeparturTime();
        route[j][PATIENT_FIELD  ] = m_nodes[i].getId();
        route[j][SERVICE_FIELD  ] = m_nodes[i].getService();
    }

    return route;
}

vector<string> Route::getAvilableServices() const { return m_caregiver.getServicesList(); }

string Route::getCaregiver() const { return m_caregiver.getID(); }

bool Route::isAvailable() const { return m_caregiver.isWorking(this -> getFreeTime()); }

vector<Node> Route::getNodes() const { return m_nodes; }

int Route::readNodesFromJson (Json::Value t_patientsInJson, vector<Patient> t_patients, vector<int> t_depotDistances) {

    const string PATIENT_FIELD      ("patient");
    const string SERVICE_FIELD      ("service");
    const string ARRIVAL_FIELD      ("arrival_time");
    const string DEPARTURE_FIELD    ("departure_time");
    
    m_nodes.clear();

    for (int i = 0; i < t_patientsInJson.size(); ++i) {
        string patientToSearch = t_patientsInJson[i][PATIENT_FIELD].asString();
        vector<Patient>::iterator patient = find_if(t_patients.begin(), t_patients.end(), 
            [patientToSearch] (const Patient p) { return p.getID() == patientToSearch; });
        if (patient == t_patients.end()) {
            throw runtime_error("Errore nel file soluzione. Paziente non trovato.");
            return -1;
        }
        if (patient -> getCurrentService().getService() != t_patientsInJson[i][SERVICE_FIELD].asString()) {
            *patient = patient -> getPatientAndNextService();
            if (patient -> getCurrentService().getService() != t_patientsInJson[i][SERVICE_FIELD].asString()) {
                throw runtime_error("Errore nel file soluzione. Servizio non associato al paziente.");
                return -1;
            }
        }
        m_nodes.push_back(Node(*patient, t_patientsInJson[i][ARRIVAL_FIELD].asInt()));
        if (m_nodes[m_nodes.size() - 1].getDeparturTime() != t_patientsInJson[i][DEPARTURE_FIELD].asInt()) {
            throw runtime_error("Errore nel file soluzione. Problema nei tempi.");
            return -1;
        }
    }
    //link to depot
    m_nodes[DEPOT].setArrivalTime(m_nodes[m_nodes.size() - 1].getDeparturTime() + 
                                    t_depotDistances[m_nodes[m_nodes.size() - 1].getDistancesIndex()]);
    
    return m_nodes.size() - 1;
}