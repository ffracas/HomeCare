#include "hcreader.hpp"

using namespace homecare; 


hcreader::hcreader(string t_dataPath) {
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

    for (Patient pat : m_patients) {
        cout<<pat.toString();
    }
    for (Depot dep : m_depots) {
        cout<<dep.toString();
    }
    for (Caregiver car : m_caregivers) {
        cout<<car.toString();
    }


}

hcreader::~hcreader() {}


