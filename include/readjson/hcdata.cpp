#include "hcdata.hpp"

using namespace homecare; 
using namespace std;

// Initilize static variables
bool HCData::m_orderedPatientsList = false;
std::vector<Patient>          HCData::m_patients;
std::vector<std::vector<int>> HCData::m_distances;
std::vector<Service>          HCData::m_services;
std::vector<Depot>            HCData::m_depots;
std::vector<Caregiver>        HCData::m_caregivers;

// Initialize constants
const double HCData::TRAVEL_TIME_WEIGHT      = 0.165;
const double HCData::MAX_TARDINESS_WEIGHT    = 0.165;
const double HCData::TARDINESS_WEIGHT        = 0.165;
const double HCData::IDLE_TIME_WEIGHT       = 0.165;
const double HCData::MAX_IDLE_TIME_WEIGHT    = 0.165;
const double HCData::TOT_WAITING_TIME_WEIGHT = 0.165;

HCData::~HCData() {}

void HCData::setData(string t_dataPath) {
    //set fields name
    const string DISTANCES_FIELD    ("distances");
    const string PATIENTS_FIELD     ("patients");
    const string SERVICES_FIELD     ("services");
    const string CAREGIVERS_FIELD   ("caregivers"); 
    const string DEPOTS_FIELD       ("departing_points"); 

    //reading JSON
    std::ifstream file(t_dataPath);
    if (!file.is_open()) {
        throw std::runtime_error("Errore nell'apertura del file di configurazione del sistema. Controllare Percorso.");
    }
    string content = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();  // Chiudi il file

    // Parsing JSON
    Json::Value root;
    Json::Reader reader;

    if (!reader.parse(content, root)) {
        throw std::runtime_error("Errore nell'apertura del file di configurazione del sistema.");
    }

    if ( !root.isMember(DISTANCES_FIELD)    || !root[DISTANCES_FIELD].isArray() || 
            !root.isMember(SERVICES_FIELD)  || !root[SERVICES_FIELD].isArray()  ||
            !root.isMember(PATIENTS_FIELD)  || !root[PATIENTS_FIELD].isArray()  ||
            !root.isMember(DEPOTS_FIELD)    || !root[DEPOTS_FIELD].isArray()    ||
            !root.isMember(CAREGIVERS_FIELD)|| !root[CAREGIVERS_FIELD].isArray()) {
        throw std::runtime_error("Errore nel formato del file di configurazione del sistema.");
    }
    /////////////////// Parsing distances
    for (const auto& distances : root[DISTANCES_FIELD]){
        vector<int> row;
        if (!distances.isArray()) {
            throw std::runtime_error("Errore nel formato del file di configurazione del sistema");
        }
        for (const auto& distance : distances) {
            row.push_back(distance.asInt());
        }
        m_distances.push_back(row);
    }
    
    /////////////////// Parsing Services
    for (const auto& service : root[SERVICES_FIELD]) {
        m_services.push_back(Service(service));
    }

    /////////////////// Parsing Patients
    for (const auto& patient : root[PATIENTS_FIELD]) {
        m_patients.push_back(Patient(patient, m_services));
    }

    /////////////////// Parsing Depots
    for (const auto& depot : root[DEPOTS_FIELD]) {
        m_depots.push_back(Depot(depot));
    }

    /////////////////// Parsing Caregivers
    for (const auto& caregiver : root[CAREGIVERS_FIELD]) {
        m_caregivers.push_back(Caregiver(caregiver));
    }

    m_orderedPatientsList = false;

    ////////// Print read elements
    /*for (Patient pat : m_patients) {
        cout<<pat.toString();
    }
    for (Depot dep : m_depots) {
        cout<<dep.toString();
    }
    for (Caregiver car : m_caregivers) {
        cout<<car.toString();
    }*/

}

/**
 * Getter for the patient carrier with patients sorted in expiry order.
 *
 * @return A vector of patients.
 */
vector<Patient> HCData::getPatients() { 
    if (!m_orderedPatientsList) {
        sort(m_patients.begin(), m_patients.end(), 
            [] (Patient p1, Patient p2) { return p1.getWindowEndTime() < p2.getWindowEndTime(); });
        m_orderedPatientsList = true;
    }
    return m_patients; 
}

/**
 * Getter for the distances matrix.
 *
 * @return The values of the distances matrix.
 */
vector<vector<int>> HCData::getDistances() { return m_distances; }

/**
 * @brief Get the distances from the specified node to all other nodes.
 * 
 * This method retrieves a vector of distances from the specified node to all other nodes
 * based on the distance matrix of the HCData object.
 * 
 * @param t_node The index of the node for which distances are requested.
 * @return vector<int> A vector containing the distances from the specified node to all other nodes.
 * @throws std::out_of_range If the specified index is out of bounds.
 */
vector<int> HCData::getNodeDistances(int t_node) noexcept(false) {
    if (t_node < 0 || t_node >= m_distances.size()) {
        throw std::out_of_range("Indice non contenuto nella matrice delle distanze");
    }
    return m_distances[t_node];
}

/**
 * @brief Get the distance between two nodes in the distance matrix.
 * 
 * This method retrieves the distance between the specified departure and arrival nodes
 * from the distance matrix of the HCData object.
 * 
 * @param t_departure The index of the departure node.
 * @param t_arrival The index of the arrival node.
 * @return int The value of the distance value between the specified nodes.
 * @throws std::out_of_range If the specified indices are out of bounds.
 */
int HCData::getDistance(int t_departure, int t_arrival) { 
    if (t_departure < 0 || t_departure >= m_distances.size() || 
            t_arrival < 0 || t_arrival >= m_distances[t_departure].size()) {
        throw std::out_of_range("Indice non contenuto nella matrice delle distanze");
    }
    return m_distances[t_departure][t_arrival]; 
}

/**
 * Getter for the services vector.
 *
 * @return A vector of services.
 */
vector<Service> HCData::getServices() { return m_services;}

/**
 * Getter for the depots vector.
 *
 * @return A vector of depots.
 */
vector<Depot> HCData::getDepots() { return m_depots; }

/**
 * Getter for the caregivers vector.
 *
 * @return A vector of caregivers.
 */
vector<Caregiver> HCData::getCaregivers() { return m_caregivers; }

vector<string> HCData::getCaregiversForService(string service) {
    vector<string> cfs;
    for (const Caregiver& caregiver : m_caregivers) {
        const vector<string>& services = caregiver.getServicesList();
        if(find(services.begin(), services.end(), service) != services.end()) {
            cfs.push_back(caregiver.getID());
        }
    }
    return cfs;
}

Caregiver HCData::getCaregiver(string t_caregiverID) {
    for (const auto& caregiver : m_caregivers ){
        if (caregiver.getID() == t_caregiverID) { return caregiver; }
    }
    throw std::runtime_error("Caregiven not found.");
}

Patient HCData::getPatient(string t_patientId) {
    for (const auto& patient : m_patients) {
        if (patient.getID() == t_patientId) { return patient; }
    }
    throw std::runtime_error("Patient not found.");
}

Patient HCData::getPatient(int pIndex) {
    if (pIndex < 0 || pIndex >= m_patients.size()) {
        throw std::runtime_error("Patient not found.");
    }
    return m_patients[pIndex];
}

int HCData::getPatientPosition(string t_patientId) {
    for (int i = 0; i < m_patients.size(); ++i) {
        if (m_patients[i].getID() == t_patientId) { return i; }
    }
    throw std::runtime_error("Patient not found.");
}