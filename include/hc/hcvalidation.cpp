#include "hcvalidation.hpp"

using namespace std;
using namespace homecare;

HCValidation::HCValidation(string t_dataFilePath, string t_solutionFilePath) {
    HCData::setData(t_dataFilePath);
    
    //set fields name
    const string ROUTE_FIELD    ("routes");
    const string CAREGIVER_FIELD("caregiver_id");
    const string PATIENTS_FIELD ("locations");

    //reading JSON
    ifstream file(t_solutionFilePath);
    if (!file.is_open()) {
        throw runtime_error("Errore nell'apertura del file soluzione. Controllare il percorso.");
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
            if ( !routeData.isMember(CAREGIVER_FIELD)    || !routeData[CAREGIVER_FIELD].isString()) {
                throw runtime_error("Errore nell'apertura del file soluzione."); 
            }
            if ( routeData.isMember(PATIENTS_FIELD) && !routeData[PATIENTS_FIELD].isArray()) {
                throw runtime_error("Errore nell'apertura del file soluzione."); 
            }
            Caregiver caregiver = HCData::getCaregiver(routeData[CAREGIVER_FIELD].asString());
            Route route(caregiver);
            if (routeData.isMember(PATIENTS_FIELD)) {
                route.readNodesFromJson(routeData[PATIENTS_FIELD], HCData::getPatients(), 
                                        HCData::getNodeDistances(caregiver.getDepotDistanceIndex()));
                m_routes.push_back(route);
            }
        } catch (const exception& e) {
            cerr << "Errore " << e.what();
        }
    }

    cout << checkSolution();
}

HCValidation::HCValidation(std::vector<Route> t_routes) : m_routes (t_routes) {}

HCValidation::~HCValidation() {}

bool HCValidation::checkSolution() {
    vector<ValidatioNode> nodes;
    for (const Patient& patient : HCData::getPatients()) {
        nodes.push_back(ValidatioNode(patient));
    }
    for (const Route& route : m_routes) {
        vector<string> services = route.getAvilableServices();
        vector<Node> route_nodes = route.getNodes();
        for (int i = 1; i < route_nodes.size(); ++i) {
            Node node(route_nodes[i]);
            //controlla se il servizio è disponibile per questo caregiver
            if (find(services.begin(), services.end(), node.getService()) == services.end()) { 
                cout<<"Some caregivers were assigned a service that cannot be provided\n"; 
                return false; 
            }
            //controlla se il caregiver non è stato rifiutato dal paziente
            vector<string> refused = HCData::getPatient(node.getId()).getInvalidCaregivers();
            if (find(refused.begin(), refused.end(), route.getCaregiver()) != refused.end()) { 
                cout<<"Some caregivers were assigned to patient that don't want it\n"; 
                return false; 
            }
            //controlla che il caregiver non sia già passato per il nodo e che il tempo sia in sincrono con altri servizi
            vector<ValidatioNode>::iterator service = find_if(nodes.begin(), nodes.end(), 
                [node] (const ValidatioNode vnode) { return vnode.getPatient() == node.getId(); } ); 
            if (service == nodes.end()) { 
                cout<<"Some caregivers have visited the same patient multiple times\n"; 
                return false; 
            }
            if (service -> setTime(node.getService(), node.getArrivalTime(), node.getDeparturTime()) 
                    != ValidatioNode::OK) { 
                cout<<endl<<route.getCaregiver()<<"->"<<i<<endl;
                cout<<"Not all patients were examined\n"; 
                return false; 
            }
            if (i > 0) {
                Node prev = route_nodes[i - 1];
                if (prev.getDeparturTime() + HCData::getDistance(prev.getDistancesIndex(), node.getDistancesIndex()) 
                        > node.getArrivalTime()) {
                cout<<endl<<route.getCaregiver()<<"->"<<i<<node.toString()<<endl;
                cout<<prev.getDeparturTime()<<"+ "<<HCData::getDistance(prev.getDistancesIndex(), node.getDistancesIndex()) << " > " <<node.getArrivalTime()<<endl;
                    cout<<"Some data can be wrong\n"; 
                    return false; 
                }
            }
        }
    }
    for (const ValidatioNode& node : nodes) {
        if (!node.isCompleted()) { cout<<node.getPatient();return false; }
    }
    return true;
}