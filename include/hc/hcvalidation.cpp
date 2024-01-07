#include "hcvalidation.hpp"

using namespace std;
using namespace homecare;

HCValidation::HCValidation(string t_dataFilePath, string t_solutionFilePath) 
        : m_data(t_dataFilePath) {
    //set fields name
    const string ROUTE_FIELD    ("routes");
    const string CAREGIVER_FIELD("caregiver_id");
    const string PATIENTS_FIELD ("locations");

    //reading JSON
    ifstream file(t_solutionFilePath);
    if (!file.is_open()) {
        throw runtime_error("Errore nell'apertura del file soluzione.");
        return;
    }
    string content = string((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();  // Chiudi il file

    // Parsing JSON
    Json::Value root;
    Json::Reader reader;

    if (!reader.parse(content, root)) {
        throw runtime_error("Errore nell'apertura del file soluzione.");
        return;
    }

    if ( !root.isMember(ROUTE_FIELD) || !root[ROUTE_FIELD].isArray()) {
        throw runtime_error("Errore nell'apertura del file soluzione.");
        return;        
    }

    for (const auto& routeData : root[ROUTE_FIELD]) {
        try{
            if ( !root.isMember(CAREGIVER_FIELD)    || !root[CAREGIVER_FIELD].isString()||
                    !root.isMember(PATIENTS_FIELD)  || !root[PATIENTS_FIELD].isArray()  ) {
                throw runtime_error("Errore nell'apertura del file soluzione.");
                return; 
            }
            Caregiver caregiver = m_data.getCaregiver(routeData[CAREGIVER_FIELD].asString());
            Route route(caregiver);
            if (!routeData[PATIENTS_FIELD].isArray()) {
                throw runtime_error("Errore nel formato del file soluzione.");
                return;
            }
            route.readNodesFromJson(routeData[PATIENTS_FIELD], m_data.getPatients(), 
                                    m_data.getNodeDistances(caregiver.getDepotDistanceIndex()));
            m_routes.push_back(route);
        } catch (const exception& e) {
            cerr << "Errore " << e.what();
        }
    }

    checkSolution();
}

HCValidation::~HCValidation() {}

bool HCValidation::checkSolution() {
    set<string> visitedNode();
    for (const Route& route : m_routes) {
        int departureTime = 0;
        int arrivalTime = 0;
        
    }
}