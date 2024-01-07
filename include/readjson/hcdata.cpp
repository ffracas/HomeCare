#include "hcdata.hpp"

using namespace homecare; 


HCData::HCData(string t_dataPath) {
    //set fields name
    const string DISTANCES_FIELD    ("distances");
    const string PATIENTS_FIELD     ("patients");
    const string SERVICES_FIELD     ("services");
    const string CAREGIVERS_FIELD   ("caregivers"); 
    const string DEPOTS_FIELD       ("departing_points"); 

    //reading JSON
    std::ifstream file(t_dataPath);
    if (!file.is_open()) {
        throw std::runtime_error("Errore nell'apertura del file di configurazione del sistema.");
        return;
    }
    string content = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();  // Chiudi il file

    // Parsing JSON
    Json::Value root;
    Json::Reader reader;

    if (!reader.parse(content, root)) {
        throw std::runtime_error("Errore nell'apertura del file di configurazione del sistema.");
        return;
    }

    if ( !root.isMember(DISTANCES_FIELD)    || !root[DISTANCES_FIELD].isArray() || 
            !root.isMember(SERVICES_FIELD)  || !root[SERVICES_FIELD].isArray()  ||
            !root.isMember(PATIENTS_FIELD)  || !root[PATIENTS_FIELD].isArray()  ||
            !root.isMember(DEPOTS_FIELD)    || !root[DEPOTS_FIELD].isArray()    ||
            !root.isMember(CAREGIVERS_FIELD)|| !root[CAREGIVERS_FIELD].isArray()) {
        throw std::runtime_error("Errore nel formato del file di configurazione del sistema.");
        return;
    }
    /////////////////// Parsing distances
    for (const auto& distances : root[DISTANCES_FIELD]){
        vector<int> row;
        if (!distances.isArray()) {
            throw std::runtime_error("Errore nel formato del file di configurazione del sistema");
            return;
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

HCData::~HCData() {}

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
 * @return A const of values of the distances matrix.
 */
vector<vector<int>> HCData::getDistances() const { return m_distances; }

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
        return vector<int>();
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
 * @return const int The value of the distance value between the specified nodes.
 * @throws std::out_of_range If the specified indices are out of bounds.
 */
int HCData::getDistance(int t_departure, int t_arrival) { 
    if (t_departure < 0 || t_departure >= m_distances.size() || 
            t_arrival < 0 || t_arrival >= m_distances[t_departure].size()) {
        throw std::out_of_range("Indice non contenuto nella matrice delle distanze");
        return 1;
    }
    return m_distances[t_departure][t_arrival]; 
}

/**
 * Getter for the services vector.
 *
 * @return A const of vector of services.
 */
vector<Service> HCData::getServices() const { return m_services;}

/**
 * Getter for the depots vector.
 *
 * @return A const of vector of depots.
 */
vector<Depot> HCData::getDepots() const { return m_depots; }

/**
 * Getter for the caregivers vector.
 *
 * @return A const of vector of caregivers.
 */
vector<Caregiver> HCData::getCaregivers() const { return m_caregivers; }

Caregiver HCData::getCaregiver(string t_caregiverID) const {
    for (const auto& caregiver : m_caregivers ){
        if (caregiver.getID() == t_caregiverID) { return caregiver; }
    }
    throw std::runtime_error("Caregiven");
    return m_caregivers[0];
}