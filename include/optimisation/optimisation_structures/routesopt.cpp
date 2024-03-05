#include "routesopt.hpp"

using namespace std;
using namespace homecare;

RoutesOpt::RoutesOpt() {}

RoutesOpt::RoutesOpt(vector<Route>& routes) {
    vector<Route> newVec(routes);
    m_routes = newVec;
    for (int i = 0; i < routes.size(); ++i) {
        for (int j = 1; j < routes[i].getNodes().size(); ++j) {
            Node n = routes[i].getNodeToDestroy(j);
            int patientIndex = HCData::getPatientPosition(n.getId());
            m_mapOfPatient[n.getId()].insertService(n.getService(), InfoNode(i, j, n.getArrivalTime(), patientIndex));
        }
    }
}

RoutesOpt::~RoutesOpt() {}

RoutesOpt RoutesOpt::replaceRoute(Route& route, int n_route) {
    if (n_route >= 0 && n_route < m_routes.size()) {
        m_routes[n_route] = route;
        RoutesOpt newRoutesOpt(m_routes);
        return newRoutesOpt;
    } else {
        throw std::out_of_range("[RoutesOpt]");
    }
}

vector<Route> RoutesOpt::getRoutes() const {
    return m_routes;
}

Route RoutesOpt::getRoute(int n_route) const {
    return m_routes[n_route];
}

string RoutesOpt::getRouteCaregiver(int n_route) const {
    if (n_route >= 0 && n_route < m_routes.size()) {
        return m_routes[n_route].getCaregiver();
    } else {
        throw std::out_of_range("[RoutesOpt] c");
    }
}

/**
 * Retrieves the node in the specified route at the specified position.
 * Throws a runtime_error if the route index is out of bounds or the node index is negative.
 * 
 * @param n_route The index of the route.
 * @param n_node The index of the node within the route.
 * @return The node at the specified position in the specified route.
 * @throws runtime_error if the route index is out of bounds or the node index is negative.
 */
Node RoutesOpt::getNodeInRoute(int n_route, int n_node) { 
    if (n_route < 0 || n_route >= m_routes.size()) { 
        throw std::out_of_range("Errore nella scelta del nodo"); 
    }
    return m_routes[n_route].getNodeToDestroy(n_node); 
}

/**
 * Returns the number of routes in the ALNS optimization.
 * 
 * @return The number of routes.
 */
int RoutesOpt::getNumberOfRoutes() const { return m_routes.size(); }

/**
 * Returns the number of nodes in the specified route.
 * 
 * @param route The index of the route.
 * @return The number of nodes in the specified route.
 */
int RoutesOpt::getNumberOfNodesInRoute(int route) const { 
    if (route >= m_routes.size() || route < 0) { throw std::out_of_range("No route"); }
    return m_routes[route].getNumNodes();
}

vector<InfoNode> RoutesOpt::getPatientServices(string patient) const {
    return m_mapOfPatient.at(patient).getAllService();
}

int RoutesOpt::getNPatientServices(string patient) const {
   auto it = m_mapOfPatient.find(patient); // Cerca la chiave nella mappa
    if (it != m_mapOfPatient.end()) {
        // Restituisce il riferimento all'elemento se la chiave esiste
        return it->second.getAllService().size();
    } else {
        // Se la chiave non esiste, genera un'eccezione
        throw std::runtime_error("Chiave non trovata nella mappa");
    }
}

bool RoutesOpt::isServiceAvailableInRoute(string service, int route) const {
    if (route >= m_routes.size() || route < 0) { throw std::out_of_range("No route"); }
    return m_routes[route].hasService(service); 
}

bool RoutesOpt::isEmpty() const {
    return m_routes.empty();
}

void RoutesOpt::destroyReferencesForPatient(string patient) {
    m_mapOfPatient.at(patient).destroyAll();
}
                                                                         
pair<int, int> RoutesOpt::getSyncServiceWindow(string patient, string service, int currentRoute) {
    InfoNode otherNode(m_mapOfPatient[patient].getOtherServiceInfo(service, currentRoute).second);
    Patient p = HCData::getPatient(otherNode.getPatientIndex());
    int late;
    int soon;
    if (!otherNode.isAssigned()){
        late = p.getWindowEndTime();
        soon = p.getWindowStartTime();
    }
    else {
        try{
        late = m_routes[otherNode.getRoute()].getNoChangeWindowCloseTime(otherNode.getPositionInRoute());
        soon = m_routes[otherNode.getRoute()].getNoChangeWindowOpenTime(otherNode.getPositionInRoute());
        } catch(exception e) {
            cerr<<"dio cane impiastricato"<<e.what();
        }
    }
    if (p.isFirstService(service)){
        return make_pair(max(soon - p.getMaxWait(), 0), late - p.getMinWait());
    }
    else {
        return make_pair(soon + p.getMinWait(), late + p.getMaxWait());
    }
}

void RoutesOpt::updateSyncServiceTime(string patient, string service, int time, int currentRoute) { 
    
    pair<string, InfoNode> otherNode(m_mapOfPatient[patient].getOtherServiceInfo(service, currentRoute));
    if (!otherNode.second.isAssigned()) { return; }
    Patient p = HCData::getPatient(otherNode.second.getPatientIndex());
    if (p.isFirstService(service)) {
        otherNode = m_mapOfPatient[patient]
                    .update(otherNode.first, otherNode.second.getRoute(), time + p.getMinWait(), time + p.getMaxWait());
    }
    else {
        otherNode = m_mapOfPatient[patient]
                    .update(otherNode.first, otherNode.second.getRoute(), time - p.getMaxWait(), time - p.getMinWait());
    }
    m_routes[otherNode.second.getRoute()]
            .updateNodeTime(otherNode.second.getPositionInRoute(), otherNode.second.getTime());
}

pair<string, InfoNode> RoutesOpt::getInterdependetInfo(string patient, string service, int currentRoute) {
    return m_mapOfPatient[patient].getOtherServiceInfo(service, currentRoute);
}   