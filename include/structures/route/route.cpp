#include "route.hpp"
//#include "../../optimisation/optimisation_structures/routesopt.hpp"

using namespace std;
using namespace homecare;

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

double Route::getCost() const {
    if (m_nodes.size() < RouteOps::BASE_ROUTE_LEN) { return 0; }
    return HCData::MAX_WAIT_TIME_WEIGHT * m_maxIdleTime + HCData::MAX_TARDINESS_WEIGHT * m_maxTardiness 
        + HCData::TARDINESS_WEIGHT * m_totalTardiness + HCData::TOT_WAITING_TIME_WEIGHT * m_totalWaitingTime 
        + HCData::EXTRA_TIME_WEIGHT * getExtraTime() + HCData::TRAVEL_TIME_WEIGHT * m_travelTime;
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

////////////////////////////////////////////////////////////////////////////////////////////////       UPDATE
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
    m_nodes.push_back(newNode);
    m_nodes[0].setArrivalTime(m_nodes.back().getDeparturTime() + m_lastNode2DepotDistance);
    return arrivalTime;
}

/**
 * Deletes a node from the route at the specified index.
 *
 * @param nodeIndex The index of the node to be deleted.
 * @param syncWin The sync windows for the route.
 * @throws std::out_of_range If the route has less than the base route length or the node index is invalid.
 */
void Route::deleteNode(int nodeIndex, const SyncWindows& syncWin) {
    if (m_nodes.size() < RouteOps::BASE_ROUTE_LEN || !isIndexNodeValid(nodeIndex)) {
        throw std::out_of_range("\n[Route] Error: Constraint on node destruction.\n");
    }
    m_nodes.erase(m_nodes.begin() + nodeIndex);
    //reschedule(syncWin);
}

int Route::addNodeInRoute(Node newNode, const SyncWindows& syncWin) {
    m_nodes.push_back(newNode);
    return reschedule(syncWin);   
}

int Route::reschedule(const SyncWindows& syncWin) {
    vector<Node> newNodes = RouteOps::rescheduleRoute(m_nodes, syncWin);
    if (newNodes.empty()) { return false; }
    m_nodes = newNodes;
    exploreData();
    return true;
}

bool Route::validityControl(int node_pos) const { return node_pos > DEPOT && node_pos < m_nodes.size(); }

bool Route::isAvailable() const { return m_caregiver.isWorking(this->getFreeTime()); }

bool Route::hasService(string request) const {
    vector<string> availableServices = m_caregiver.getServicesList();
    return find(availableServices.begin(), availableServices.end(), request) != availableServices.end();
}

bool Route::isIndexNodeValid(int index) const { return index > DEPOT && index < m_nodes.size(); }

vector<Node> Route::getNodes() const { return m_nodes; }

////////////////////////////////////////////////////////////////////////////////////////////////       READ FROM FILE

int Route::readNodesFromJson(Json::Value t_patientsInJson, vector<Patient> t_patients, vector<int> t_depotDistances)
{
    const string PATIENT_FIELD("patient");
    const string SERVICE_FIELD("service");
    const string ARRIVAL_FIELD("arrival_time");
    const string DEPARTURE_FIELD("departure_time");

    for (int i = 0; i < t_patientsInJson.size(); ++i) {
        string patientToSearch = t_patientsInJson[i][PATIENT_FIELD].asString();
        vector<Patient>::iterator patient = find_if(t_patients.begin(), t_patients.end(),
                                                    [patientToSearch](const Patient p)
                                                    { return p.getID() == patientToSearch; });
        if (patient == t_patients.end()) {
            throw runtime_error("Errore nel file soluzione. Paziente non trovato.");
        }
        if (patient->getCurrentService().getService() != t_patientsInJson[i][SERVICE_FIELD].asString()) {
            *patient = patient->getPatientAndNextService();
            if (patient->getCurrentService().getService() != t_patientsInJson[i][SERVICE_FIELD].asString()) {
                throw runtime_error("Errore nel file soluzione. Servizio non associato al paziente.");
            }
        }
        m_nodes.push_back(Node(*patient, t_patientsInJson[i][ARRIVAL_FIELD].asInt()));
        if (m_nodes[m_nodes.size() - 1].getDeparturTime() != t_patientsInJson[i][DEPARTURE_FIELD].asInt()) {
            throw runtime_error("Errore nel file soluzione. Problema nei tempi.");
        }
    }
    // link to depot
    m_nodes[DEPOT].setArrivalTime(m_nodes[m_nodes.size() - 1].getDeparturTime() +
                                  t_depotDistances[m_nodes[m_nodes.size() - 1].getDistancesIndex()]);

    return m_nodes.size() - 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////          UTILS

int Route::getNoChangeWindowCloseTime(int n_node) const {
    if (!validityControl(n_node)) {
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
    if (!validityControl(n_node)) {
        throw std::out_of_range("[route] Constraint on route selection.");
    }
    int time = max(m_nodes[n_node].getArrivalTime(), m_nodes[n_node].getTimeWindowOpen());
    int time2 = getNoChangeWindowCloseTime(n_node);
    return time > time2 ? time2 : time;
}

void Route::updateNodeTime(int n_node, int arrival) {
    if (!validityControl(n_node)) {
        throw std::out_of_range("[route] Constraint on route selection.");
    }
    m_nodes[n_node].setArrivalTime(arrival);
}

/**
 * Explores the data associated with the route, calculating various metrics that are 
 * total travel time, total waiting time, total tardiness, and the maximum idle time and tardiness.
 */
void Route::exploreData() {
    // reset params
    m_maxTardiness = 0;
    m_maxIdleTime = 0;
    m_totalTardiness = 0;
    m_totalWaitingTime = 0;
    m_travelTime = 0;

    for (vector<Node>::iterator i = m_nodes.begin() + 1; i != m_nodes.end(); ++i) {
        m_travelTime += HCData::getDistance((i - 1)->getDistancesIndex(), i->getDistancesIndex());
        if (i->getArrivalTime() < i->getTimeWindowOpen()) {
            int waiting = i->getTimeWindowOpen() - i->getArrivalTime();
            m_totalWaitingTime += waiting;
            if (waiting > m_maxIdleTime) { m_maxIdleTime = waiting; }
        }
        else if (i->getArrivalTime() > i->getTimeWindowClose()) {
            int tardiness = i->getArrivalTime() - i->getTimeWindowClose();
            m_totalTardiness += tardiness;
            if (tardiness > m_maxTardiness) { m_maxTardiness = tardiness; }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////// OUTPUT
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