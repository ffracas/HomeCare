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
 * Getter for the patients vector.
 *
 * @return A const of vector of patients.
 */
const vector<Patient> HCData::HCData::getPatients() const { return m_patients; }

/**
 * Getter for the distances matrix.
 *
 * @return A const of values of the distances matrix.
 */
const vector<vector<int>> HCData::HCData::getDistances() const { return m_distances; }

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
const int HCData::HCData::getDistance(int t_departure, int t_arrival) { 
    if (t_departure < 0 || t_departure > m_distances.size() || 
            t_arrival < 0 || t_arrival > m_distances[t_departure].size()) {
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
const vector<Service> HCData::HCData::getServices() const { return m_services;}

/**
 * Getter for the depots vector.
 *
 * @return A const of vector of depots.
 */
const vector<Depot> HCData::getDepots() const { return m_depots; }

/**
 * Getter for the caregivers vector.
 *
 * @return A const of vector of caregivers.
 */
const vector<Caregiver> HCData::getCaregivers() const { return m_caregivers; }
