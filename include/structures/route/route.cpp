#include "route.hpp"
//#include "../../optimisation/optimisation_structures/routesopt.hpp"

using namespace std;
using namespace homecare;

const int Route::BASE_ROUTE_LEN = 2;
const int Route::DEPOT = 0;

// Constructor
Route::Route(Caregiver t_caregiver) : m_caregiver(t_caregiver)
{
    m_nodes.push_back(Node(t_caregiver));
    m_maxTardiness = 0;
    m_maxIdleTime = 0;
    m_totalTardiness = 0;
    m_totalWaitingTime = 0;
    m_travelTime = 0;
    m_lastNode2DepotDistance = 0;
}
// Destructor
Route::~Route() {}

////////////////////////////////////////////////////////////////////////////////////////////////            getter

int Route::getFreeTime() const { return m_nodes[m_nodes.size() - 1].getDeparturTime(); }

int Route::getLastPatientDistanceIndex() const { return m_nodes[m_nodes.size() - 1].getDistancesIndex(); }

int Route::getLastNode2DepotDistance() const { return m_lastNode2DepotDistance; }

/**
 * Returns the list of available services for the route.
 * @return The list of available services for the route.
 */
vector<string> Route::getAvilableServices() const { return m_caregiver.getServicesList(); }

/**
 * Returns the ID of the caregiver associated with the route.
 * @return The ID of the caregiver associated with the route.
 */
string Route::getCaregiver() const { return m_caregiver.getID(); }

/**
 * Returns the number of nodes in the route.
 * @return The number of nodes in the route.
 */
int Route::getNumNodes() const { return m_nodes.size(); }

/**
 * Returns the node at the specified position in the route.
 * @param pos The position of the node.
 * @return The node at the specified position in the route.
 * @throws std::runtime_error if the route contains only one node.
 */
Node Route::getPatientNode(int pos) const {
    int nNodes = m_nodes.size();
    if (nNodes < 1 || pos >= nNodes || pos <= DEPOT) {
        throw std::out_of_range("\n[route] Error: Out of range of route\n");
    }
    return m_nodes[pos];
}

/**
 * Returns the maximum tardiness for the route.
 * @return The maximum tardiness for the route.
 */
int Route::getMaxTardiness() const { return m_maxTardiness; }

/**
 * Returns the maximum idle time for the route.
 * @return The maximum idle time for the route.
 */
int Route::getMaxIdleTime() const { return m_maxIdleTime; }

/**
 * Returns the total tardiness for the route.
 * @return The total tardiness for the route.
 */
int Route::getTotalTardiness() const { return m_totalTardiness; }

/**
 * Returns the total waiting time for the route.
 * @return The total waiting time for the route.
 */
int Route::getTotalWaitingTime() const { return m_totalWaitingTime; }

/**
 * Returns the travel time for the route.
 * @return The travel time for the route.
 */
int Route::getTravelTime() const { return m_travelTime; }

int Route::getExtraTime() const {
    int endTime = m_nodes[DEPOT].getArrivalTime() - m_caregiver.getShiftEndTime();
    return endTime > 0 ? endTime : 0;
}

string Route::getHash() const {
    stringstream ss;
    ss << m_caregiver.getID() << "--";
    for (const Node &node : m_nodes) {
        ss << node.getHash() << '-';
    }
    ss << "--";
    return ss.str();
}

double Route::getCost() const {
    if (m_nodes.size() < BASE_ROUTE_LEN) {
        return 0;
    }
    return HCData::MAX_WAIT_TIME_WEIGHT * m_maxIdleTime + HCData::MAX_TARDINESS_WEIGHT * m_maxTardiness 
        + HCData::TARDINESS_WEIGHT * m_totalTardiness + HCData::TOT_WAITING_TIME_WEIGHT * m_totalWaitingTime 
        + HCData::EXTRA_TIME_WEIGHT * getExtraTime() + HCData::TRAVEL_TIME_WEIGHT * m_travelTime;
}

bool Route::validityControl(int node_pos) const {
    return node_pos <= DEPOT || node_pos >= m_nodes.size();
}

////////////////////////////////////////////////////////////////////////////////////////////////         update route
int Route::appendNode(Node newNode, int t_arrivalTime) {
    // subtract old return to depot time
    m_travelTime -= m_lastNode2DepotDistance;
    // new arcs time
    int lastToNewDistance = HCData::getDistance(getLastPatientDistanceIndex(), newNode.getDistancesIndex());
    m_lastNode2DepotDistance = HCData::getDistance(newNode.getDistancesIndex(), m_nodes[DEPOT].getDistancesIndex());
    m_travelTime += lastToNewDistance + m_lastNode2DepotDistance;

    // calculate arrival time
    int arrivalTime = getFreeTime() + lastToNewDistance;
    // update waiting time
    int startTime = max(t_arrivalTime, newNode.getTimeWindowOpen());
    if (arrivalTime < startTime) {
        int earliness = startTime - arrivalTime;
        m_totalWaitingTime += earliness;
        m_maxIdleTime = max(earliness, m_maxIdleTime);
    }
    else {
        arrivalTime = t_arrivalTime;
        if (arrivalTime > newNode.getTimeWindowClose()) {
            int tardiness = arrivalTime - newNode.getTimeWindowClose();
            m_totalTardiness += tardiness;
            m_maxTardiness = max(tardiness, m_maxTardiness);
        }
    }
    // FIXME: direi che manca inserimento in lista
    return arrivalTime;
}

Route Route::deleteNode(int nodeIndex, SyncWindows syncWin) {
    int nNodes = m_nodes.size();
    if (nNodes <= 1 || nodeIndex >= nNodes || nodeIndex < 1){
        throw std::out_of_range("Constraint on node destruction.");
    }
    m_nodes.erase(m_nodes.begin() + nodeIndex);
    //recalculateRoute();
    m_nodes = RouteOps::rescheduleRoute(m_nodes, syncWin);
    exploreData();
    return *this;
}

// TODO: remove this function
/*vector<Node> Route::addNodeInRoute(Patient patient, RoutesOpt& blackops, int routeIndex) {
    // if route index is out of bounds throw exception
    if (routeIndex >= blackops.getNumberOfRoutes()) {
        throw std::runtime_error("[Route::addNodeInRoute] Route index out of bounds.");
    }

    // scheduling
    vector<Node> newSchedule;
    vector<Node> otherNodes;
    vector<Node> nodesList = blackops.getRoute(routeIndex).getNodes();

    // get time window for sync services
    map<string, pair<int, int>> syncWindows;
    for (auto& node : nodesList) {
        if (node.isInterdependent()) {
            syncWindows[node.getId()] = blackops.getSyncServiceWindow(node.getId(), node.getService(), routeIndex);
        }
    }

    // add new node to the route
    Node newNode(patient, patient.getWindowStartTime());
    nodesList.push_back(newNode);
    // if node is interdependent add sync time window
    if (patient.hasNextService()) {
        syncWindows[patient.getID()] = make_pair(patient.getWindowStartTime(), patient.getWindowEndTime());
    }

    // support list for scheduling
    vector<Node> opened;
    vector<Node> closed;
    opened.push_back(nodesList[0]);

    // build the route
    Node current (opened[0]);
    while (!opened.empty()) {
        int depTime = current.getDeparturTime();
        opened.clear();
        bool failed = false;
        bool justChanged = false;
        string oldCurrentId = "";
        for (int i = 0; i < nodesList.size() && !failed; i++) {
            if (nodesList[i].getId() != current.getId()) {
                Node& node = nodesList[i];
                // prevision time for new node
                int previsionT = depTime + HCData::getDistance(current.getDistancesIndex(), node.getDistancesIndex());
                // if the node is sync service and the time is not in the window
                if (node.isInterdependent() && previsionT > syncWindows[node.getId()].second) {
                    if (current.isDepot()) { throw runtime_error("Service is not available in this route"); }
                    // change node if constraint is not satisfied
                    failed = true;
                    if (justChanged && oldCurrentId == current.getId()) {
                        return vector<Node>();
                    }
                    justChanged = true;
                    oldCurrentId = current.getId();
                    opened.clear();
                    int newNodeT = closed.back().getArrivalTime() 
                                + HCData::getDistance(closed.back().getDistancesIndex(), node.getDistancesIndex());
                    node.setArrivalTime(newNodeT);
                    opened.push_back(node);
                }
                else {
                    if (node.isInterdependent()) {
                        previsionT = max(previsionT, syncWindows[node.getId()].first);
                    }
                    node.setArrivalTime(previsionT);
                    opened.push_back(node);
                    sort(opened.begin(), opened.end(), 
                        [](Node& a, Node& b) { return a.getDeparturTime() < b.getDeparturTime(); });
                }
            }
        }
        if (!failed) {
            if (current.getId() == newNode.getId()) {
                newNode.setArrivalTime(current.getArrivalTime());
            }
            justChanged = false;
            oldCurrentId = "";
            closed.push_back(current);
            auto it = find_if(nodesList.begin(), nodesList.end(), 
                    [current](const Node& n) { return n.getId() == current.getId(); });
            if (it != nodesList.end()) {
                nodesList.erase(it);
            } else {
                return vector<Node>();
            }
        }
        current = opened[0];
    }
    return closed;
}*/

// TODO: remove this function
/*vector<Node> Route::mergeLists(vector<Node>& first, vector<Node>& second, RoutesOpt& blackops, int routeIndex) {
    // if second is empty return first
    if (second.empty()) {
        return first;
    }
    // if first is not empty the first node in second is the depot
    if (!first.empty()) {
        second.insert(second.begin(), first.back());
        first.pop_back();
    }
    
    // get time window for sync services
    map<string, pair<int, int>> syncWindows;
    for (auto& node : second) {
        if (node.isInterdependent()) {
            syncWindows[node.getId()] = blackops.getSyncServiceWindow(node.getId(), node.getService(), routeIndex);
        }
    }

    // support list for scheduling
    vector<Node> opened;
    vector<Node> closed;
    opened.push_back(second[0]);
    if (!first.empty()) {
        closed.push_back(first.back());
    }

    // build the route
    Node current (opened[0]);
    while (!opened.empty()) {
        int depTime = current.getDeparturTime();
        opened.clear();
        bool failed = false;
        bool justChanged = false;
        string oldCurrentId = "";
        for (int i = 0; i < second.size() && !failed; i++) {
            if (second[i].getId() != current.getId()) {
                Node& node = second[i];
                // prevision time for new node
                int previsionT = depTime + HCData::getDistance(current.getDistancesIndex(), node.getDistancesIndex());
                // if the node is sync service and the time is not in the window
                if (node.isInterdependent() && previsionT > syncWindows[node.getId()].second) {
                    if (current.isDepot()) { throw runtime_error("Service is not available in this route"); }
                    // change node if constraint is not satisfied
                    failed = true;
                    if (justChanged && oldCurrentId == current.getId()) {
                        throw runtime_error("Service is not available in this route");
                    }
                    justChanged = true;
                    oldCurrentId = current.getId();
                    opened.clear();
                    int newNodeT = closed.back().getArrivalTime() 
                                + HCData::getDistance(closed.back().getDistancesIndex(), node.getDistancesIndex());
                    node.setArrivalTime(newNodeT);
                    opened.push_back(node);
                }
                else {
                    node.setArrivalTime(previsionT);
                    opened.push_back(node);
                    sort(opened.begin(), opened.end(), 
                        [](Node& a, Node& b) { return a.getDeparturTime() < b.getDeparturTime(); });
                }
            }
        }
        if (!failed) {
            justChanged = false;
            oldCurrentId = "";
            closed.push_back(current);
            auto it = find_if(second.begin(), second.end(), 
                    [current](const Node& n) { return n.getId() == current.getId(); });
            if (it != second.end()) {
                second.erase(it);
            } else {
                throw runtime_error ("Error in Route::mergeLists: node not found in second list");
            }
        }
        current = opened[0];
    }

    // add nodes to the route
    for (auto& node : closed) {
        first.push_back(node);
    }

    return first;
}*/

bool Route::isAvailable() const { return m_caregiver.isWorking(this->getFreeTime()); }

bool Route::hasService(string request) const
{
    vector<string> availableServices = m_caregiver.getServicesList();
    return find(availableServices.begin(), availableServices.end(), request) != availableServices.end();
}

bool Route::isIndexNodeValid(int index) const {
    return index >= DEPOT && index < m_nodes.size();
}

// TODO: move this function in schedule 
/*void Route::replaceRoute(vector<Node>& newRoute) {
    m_nodes.clear();
    m_nodes = newRoute;
    exploreData();
}*/

vector<Node> Route::getNodes() const { return m_nodes; }

int Route::readNodesFromJson(Json::Value t_patientsInJson, vector<Patient> t_patients, vector<int> t_depotDistances)
{
    const string PATIENT_FIELD("patient");
    const string SERVICE_FIELD("service");
    const string ARRIVAL_FIELD("arrival_time");
    const string DEPARTURE_FIELD("departure_time");

    for (int i = 0; i < t_patientsInJson.size(); ++i)
    {
        string patientToSearch = t_patientsInJson[i][PATIENT_FIELD].asString();
        vector<Patient>::iterator patient = find_if(t_patients.begin(), t_patients.end(),
                                                    [patientToSearch](const Patient p)
                                                    { return p.getID() == patientToSearch; });
        if (patient == t_patients.end())
        {
            throw runtime_error("Errore nel file soluzione. Paziente non trovato.");
        }
        if (patient->getCurrentService().getService() != t_patientsInJson[i][SERVICE_FIELD].asString())
        {
            *patient = patient->getPatientAndNextService();
            if (patient->getCurrentService().getService() != t_patientsInJson[i][SERVICE_FIELD].asString())
            {
                throw runtime_error("Errore nel file soluzione. Servizio non associato al paziente.");
            }
        }
        m_nodes.push_back(Node(*patient, t_patientsInJson[i][ARRIVAL_FIELD].asInt()));
        if (m_nodes[m_nodes.size() - 1].getDeparturTime() != t_patientsInJson[i][DEPARTURE_FIELD].asInt())
        {
            throw runtime_error("Errore nel file soluzione. Problema nei tempi.");
        }
    }
    // link to depot
    m_nodes[DEPOT].setArrivalTime(m_nodes[m_nodes.size() - 1].getDeparturTime() +
                                  t_depotDistances[m_nodes[m_nodes.size() - 1].getDistancesIndex()]);

    return m_nodes.size() - 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////          Utils

int Route::getNoChangeWindowCloseTime(int n_node) const {
    if (validityControl(n_node)) {
        throw std::out_of_range("[route] Constraint on route selection.");
    }
    // if last node return max(arrival, close)
    if (n_node == m_nodes.size() - 1) {
        return max(m_nodes[n_node].getArrivalTime(), m_nodes[n_node].getTimeWindowClose());
    }
    // else compare with next node
    int distance = HCData::getDistance(m_nodes[n_node].getDistancesIndex(), m_nodes[n_node + 1].getDistancesIndex());
    int duration = m_nodes[n_node].getDeparturTime() - m_nodes[n_node].getArrivalTime();
    return max(m_nodes[n_node + 1].getArrivalTime(), m_nodes[n_node + 1].getTimeWindowOpen()) - distance - duration;
}

int Route::getNoChangeWindowOpenTime(int n_node) const {
    if (validityControl(n_node)) {
        throw std::out_of_range("[route] Constraint on route selection.");
    }
    int time = max(m_nodes[n_node].getArrivalTime(), m_nodes[n_node].getTimeWindowOpen());
    int time2 = getNoChangeWindowCloseTime(n_node);
    return time > time2 ? time2 : time;
}

void Route::updateNodeTime(int n_node, int arrival) {
    if (validityControl(n_node)) {
        throw std::out_of_range("[route] Constraint on route selection.");
    }
    m_nodes[n_node].setArrivalTime(arrival);
}

void Route::exploreData() {
    //reset params
    m_maxTardiness = 0;
    m_maxIdleTime = 0;
    m_totalTardiness = 0;
    m_totalWaitingTime = 0;
    m_travelTime = 0;

    for (vector<Node>::iterator i = m_nodes.begin() + 1; i != m_nodes.end(); ++i) {
        
        m_travelTime += HCData::getDistance((i-1)->getDistancesIndex(), i->getDistancesIndex());
        if (i->getArrivalTime() < i->getTimeWindowOpen()) {
            int waiting = i->getTimeWindowOpen() - i->getArrivalTime();
            m_totalWaitingTime += waiting;
            if (waiting > m_maxIdleTime) {
                m_maxIdleTime = waiting;
            }
        }
        else if (i->getArrivalTime() > i->getTimeWindowClose()) {
            int tardiness = i->getArrivalTime() - i->getTimeWindowClose();
            m_totalTardiness += tardiness;
            if (tardiness > m_maxTardiness) {
                m_maxTardiness = tardiness;
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////// Output
Json::Value Route::getJSONRoute() const
{
    Json::Value route;
    const string ARRIVAL_FIELD("arrival_time");
    const string DEPARTURE_FIELD("departure_time");
    const string PATIENT_FIELD("patient");
    const string SERVICE_FIELD("service");

    // start from the first patient
    for (int i = 1, j = 0; i < m_nodes.size(); ++i, ++j)
    {
        route[j][ARRIVAL_FIELD] = m_nodes[i].getArrivalTime();
        route[j][DEPARTURE_FIELD] = m_nodes[i].getDeparturTime();
        route[j][PATIENT_FIELD] = m_nodes[i].getId();
        route[j][SERVICE_FIELD] = m_nodes[i].getService();
    }

    return route;
}

string Route::getRouteToString() const
{
    stringstream ss;
    ss << " ROUTE\n";
    for (const Node &node : m_nodes)
    {
        ss << node.toString() << '\n';
    }
    return ss.str();
}