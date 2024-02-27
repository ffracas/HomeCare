#include "alnsoptimisation.hpp"

using namespace std;
using namespace homecare;

const double ALNSOptimisation::MAX_DOUBLE = 1.79769313486231570e+308;

ALNSOptimisation::ALNSOptimisation(vector<Route> t_routes, double t_cost) 
        : m_currentCost (t_cost), m_routes (t_routes), m_nodeToRelocate (), 
        m_mapOfPatient (), m_solutionsDump() {
    string solHash = makeHash(t_routes);
    m_currentSol = solHash;
    m_solutionsDump.emplace(solHash, t_routes);
    m_solutionsRank.push_back(pair<double, string> (t_cost, solHash));
    for (int i = 0; i < t_routes.size(); ++i) {
        for (int j = t_routes[i].getNumNodes() - 1; j > 0; --j) {
            Node n = t_routes[i].getNodeToDestroy(j);
            //set variation time
            int pos = HCData::getPatientPosition(n.getId());
            m_mapOfPatient[n.getId()].insertService(n.getService(), InfoNode(i, j, n.getArrivalTime(), pos));
        }
    }
}

ALNSOptimisation::~ALNSOptimisation() {}

void ALNSOptimisation::resetOperation() {
    // clear m_routes
    m_routes.clear();

    // copy the solution from m_solutionsDump[m_currentSol] in m_routes
    for (const auto& route : m_solutionsDump[m_currentSol]) {
        m_routes.push_back(route);
    }

    // clear the vector that contains node to relocate
    m_nodeToRelocate.clear();
}

string ALNSOptimisation::makeHash(vector<Route> t_routes) {
    stringstream hash;
    for (Route const & route : t_routes) { hash << route.getHash(); }
    return hash.str();
}

/**
 * Destroys a node from the specified route and updates the data structures being utilized by the optimisation process.
 * 
 * @param n_route The index of the route from which to destroy the node.
 * @param n_node The index of the node to destroy within the route.
 * @return The number of services associated with the destroyed node.
 */
int ALNSOptimisation::destroy(int n_route, int n_node) { return destroy(n_route, n_node, m_routes, true); }

/**
 * Destroys a node from the specified route and updates the related data structures.
 * 
 * @param n_route The index of the route from which to destroy the node.
 * @param n_node The index of the node to destroy within the route.
 * @param t_routes The vector of routes to operate on.
 * @param onOperational Flag indicating whether to operate on the operational routes.
 * @return The number of services associated with the destroyed node.
 */
int ALNSOptimisation::destroy(int n_route, int n_node, vector<Route>& t_routes, bool onOperational) {
    // Ensure n_node is valid
    if (n_node < 1) { throw runtime_error("node not valid"); }
    // Get the patient ID to relocate
    string patient = t_routes[n_route].getNodeToDestroy(n_node).getId();
    m_nodeToRelocate.push_back(patient);
    // Iterate over services associated with the patient
    vector<InfoNode> services(m_mapOfPatient[patient].getAllService());
    for (InfoNode service : services) {
        if(!service.isAssigned()) { return 0; }
        // Remove from routes
        n_route = service.getRoute();
        t_routes[n_route].deleteNode(service.getPositionInRoute());
        if(onOperational) {
            // Destroy the service reference
            updateMapOfPatient(t_routes[n_route], n_route);
            m_currentCost = calculateCost(m_routes);
        }
    }
    m_mapOfPatient[patient].destroyAll();
    // Return the number of destroyed node
    return m_mapOfPatient[patient].size();
}

double ALNSOptimisation::repair(vector<Route>& routes, Patient patient, int n_route) {
    return repair(routes, Node(patient, 0), n_route).getCost();
}

CostCoord ALNSOptimisation::repair(Node patient, int route) {
    return repair(m_routes, patient, route, true);
}

CostCoord ALNSOptimisation::repair(vector<Route>& routes, Node patient, int route, bool onOperational) {
    const CostCoord NO_REPAIR(MAX_DOUBLE, -1, -1);
    // Verifica dei parametri da inserire
    auto p = m_mapOfPatient.find(patient.getId());
    if (p == m_mapOfPatient.end()) { return NO_REPAIR; }
    if (!m_mapOfPatient[patient.getId()].isPresent(patient.getService())) { return NO_REPAIR; }

    if (route >= routes.size()) { return NO_REPAIR; }
    int pos = routes[route].addNodeBeetween(patient);
    if (pos < 1) { return NO_REPAIR; }
    if (onOperational) {
        updateMapOfPatient(routes[route], route);
    }
    // calculate Cost and return Cost and coordinate
    return CostCoord(calculateCost(routes), route, pos);
}

bool ALNSOptimisation::repairDouble(Patient patient, int n_route) {
    Node n1(patient, 0);
    tuple<Node, vector<Node>, vector<Node>> data(m_routes[n_route].addNodeInRoute(patient, *this));
}

void ALNSOptimisation::saveSol(const vector<Route>& routes) {
    m_routes.clear();
    m_routes.insert(m_routes.begin(), routes.begin(), routes.end());
    //aggiorna mappa
    updateMapOfPatient();

    double cost = calculateCost(m_routes);
    string hash = ALNSOptimisation::makeHash(m_routes);
    //salva soluzione
    m_solutionsDump.emplace(hash, m_routes);
    //classifica soluzioni
    m_solutionsRank.push_back(make_pair(cost, hash));
    sort(m_solutionsRank.begin(), m_solutionsRank.end(), 
            [] (pair<double, string> e1, pair<double, string> e2) { return e1.first < e2.first; });
    m_currentSol = m_solutionsRank[0].second;
    m_currentCost = m_solutionsRank[0].first;
}

/**
 * Returns the number of routes in the ALNS optimization.
 * 
 * @return The number of routes.
 */
int ALNSOptimisation::getNumberOfRoutes() const { return m_routes.size(); }

/**
 * Returns the number of nodes in the specified route.
 * 
 * @param route The index of the route.
 * @return The number of nodes in the specified route.
 */
int ALNSOptimisation::getNumberOfNodesInRoute(int route) const { 
    if (route >= m_routes.size()) { return -1; }
    return m_routes[route].getNumNodes(); 
}

/**
 * Updates the map of patients based on the given route and route index.
 * This method iterates through the nodes in the route and updates the information in the map of patients.
 * 
 * @param route The route to update the map of patients with.
 * @param n_route The index of the route.
 */
void ALNSOptimisation::updateMapOfPatient(const Route& route, int n_route) {
    for (int j = 1; j < route.getNumNodes(); ++j) {
        const Node& n = route.getNodeToDestroy(j);
        m_mapOfPatient[n.getId()].getInfoForService(n.getService()).setInRoute(n_route, j, n.getArrivalTime());
    }
}

void ALNSOptimisation::updateMapOfPatient() {
    for (int i = 0; i < m_routes.size(); ++i) {
        updateMapOfPatient(m_routes[i], i);
    }
}

/**
 * Retrieves the current cost of the optimization process.
 * 
 * @return The current cost of the optimization process.
 */
double ALNSOptimisation::getCurrentCost() const { return m_currentCost; }

/**
 * Calculates the cost based on the provided routes.
 * The cost is calculated by considering various factors such as maximum idle time, maximum tardiness,
 * total tardiness, total waiting time, travel time, and total extra time of the routes.
 * 
 * @param t_routes The routes for which the cost is to be calculated.
 * @return The calculated cost.
 */
double ALNSOptimisation::calculateCost(const vector<Route>& t_routes) const {
    int maxIdleTime       = 0;
    int maxTardiness      = 0;
    int totalTardiness    = 0;
    int totalWaitingTime  = 0;
    int travelTime        = 0;
    int totalExtraTime    = 0;

    for (Route route : t_routes) {
        maxIdleTime = route.getMaxIdleTime() > maxIdleTime ? route.getMaxIdleTime() : maxIdleTime;
        maxTardiness = route.getMaxTardiness() > maxTardiness ? route.getMaxTardiness() : maxTardiness;
        totalTardiness += route.getTotalTardiness();
        totalWaitingTime += route.getTotalWaitingTime();
        travelTime += route.getTravelTime();
        totalExtraTime += route.getExtraTime();
    }

    return HCData::MAX_IDLE_TIME_WEIGHT * maxIdleTime + HCData::MAX_TARDINESS_WEIGHT * maxTardiness 
        + HCData::TARDINESS_WEIGHT * totalTardiness + HCData::TOT_WAITING_TIME_WEIGHT * totalWaitingTime  
        + HCData::EXTRA_TIME_WEIGHT * totalExtraTime + HCData::TRAVEL_TIME_WEIGHT * travelTime;
}

/**
 * Generates a random double between 0 and 0.999.
 * Uses the Mersenne Twister pseudo-random number generator.
 * 
 * @return A random double between 0 and 0.999.
 */
double ALNSOptimisation::generateRandom() const {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 0.999);

    // Genera un numero casuale tra 0 e 1
    return dis(gen);
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
Node ALNSOptimisation::getNodeInRoute(int n_route, int n_node) const { 
    if (n_route < 0 || n_route >= m_routes.size() || n_node < 0) { 
        throw runtime_error("Errore nella scelta del nodo"); 
    }
    return m_routes[n_route].getNodeToDestroy(n_node); 
}

/**
 * Retrieves the routes stored in the ALNSOptimisation object.
 * @return A vector of Route objects representing the routes.
 */
vector<Route> ALNSOptimisation::getRoutes() const  { return m_routes; }

/**
 * Checks if there are nodes to repair.
 * 
 * @return True if there are nodes to repair, false otherwise.
 */
bool ALNSOptimisation::hasNodeToRepair() const { return !m_nodeToRelocate.empty(); }

string ALNSOptimisation::popNodeToRepair() {
    std::string node;

    if (!m_nodeToRelocate.empty()) {
        node = m_nodeToRelocate[0];
        m_nodeToRelocate.erase(m_nodeToRelocate.begin());
    }

    return node;
}