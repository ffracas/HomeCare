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
    }
    string content = string((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();  // Chiudi il file

    // Parsing JSON
    Json::Value root;
    Json::Reader reader;

    if (!reader.parse(content, root)) {
        throw runtime_error("Errore nell'apertura del file soluzione.");
    }

    if ( !root.isMember(ROUTE_FIELD) || !root[ROUTE_FIELD].isArray()) {
        throw runtime_error("Errore nell'apertura del file soluzione.");        
    }

    for (const auto& routeData : root[ROUTE_FIELD]) {
        try{
            if ( !root.isMember(CAREGIVER_FIELD)    || !root[CAREGIVER_FIELD].isString()||
                    !root.isMember(PATIENTS_FIELD)  || !root[PATIENTS_FIELD].isArray()  ) {
                throw runtime_error("Errore nell'apertura del file soluzione."); 
            }
            Caregiver caregiver = m_data.getCaregiver(routeData[CAREGIVER_FIELD].asString());
            Route route(caregiver);
            if (!routeData[PATIENTS_FIELD].isArray()) {
                throw runtime_error("Errore nel formato del file soluzione.");
            }
            route.readNodesFromJson(routeData[PATIENTS_FIELD], m_data.getPatients(), 
                                    m_data.getNodeDistances(caregiver.getDepotDistanceIndex()));
            m_routes.push_back(route);
        } catch (const exception& e) {
            cerr << "Errore " << e.what();
        }
    }

    cout << checkSolution();
}

HCValidation::HCValidation(HCData t_data, std::vector<Route> t_routes) : m_data (t_data), m_routes (t_routes) {}

HCValidation::~HCValidation() {}

bool HCValidation::checkSolution() {
    vector<ValidatioNode> nodes;
    for (const Patient& patient : m_data.getPatients()) {
        nodes.push_back(ValidatioNode(patient));
    }
    for (const Route& route : m_routes) {
        vector<string> services = route.getAvilableServices();
        vector<Node> route_nodes = route.getNodes();
        for (int i = 1; i < route_nodes.size(); ++i) {
            Node node(route_nodes[i]);
            //controlla se il servizio è disponibile per questo caregiver
            if (find(services.begin(), services.end(), node.getService()) == services.end()) { return false; }
            //controlla se il caregiver non è stato rifiutato dal paziente
            vector<string> refused = m_data.getPatient(node.getId()).getInvalidCaregivers();
            if (find(refused.begin(), refused.end(), route.getCaregiver()) != refused.end()) { return false; }
            //controlla che il caregiver non sia già passato per il nodo e che il tempo sia in sincrono con altri servizi
            vector<ValidatioNode>::iterator service = find_if(nodes.begin(), nodes.end(), 
                [node] (const ValidatioNode vnode) { return vnode.getPatient() == node.getId(); } ); 
            if (service == nodes.end()) {  return false; }
            if (service -> setTime(node.getService(), node.getArrivalTime(), node.getDeparturTime()) 
                    != ValidatioNode::OK) { return false; }
            if (i > 0) {
                Node prev = route_nodes[i - 1];
                if (prev.getDeparturTime() + m_data.getDistance(prev.getDistancesIndex(), node.getDistancesIndex()) 
                        > node.getArrivalTime()) { return false; }
            }
        }
    }
    for (const ValidatioNode& node : nodes) {
        if (!node.isCompleted()) { return false; }
    }
    return true;
}