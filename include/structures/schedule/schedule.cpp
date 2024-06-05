#include "schedule.hpp"

using namespace std;
using namespace homecare;

Schedule::Schedule() {}

/**
 * Constructs a new Schedule object with the given set of Caregivers.
 *
 * @param t_caregivers A vector of Caregiver objects representing the available caregivers.
 */
Schedule::Schedule(const vector<Caregiver> &t_caregivers) {
    for (auto &caregiver : t_caregivers) {
        m_routes.push_back(Route(caregiver));
    }
}

Schedule::~Schedule() {}

//////////////////////////////////////////////////////////////////////////////////////       UTILS

/**
 * Calculates the supposed cost of adding a new node to a given route.
 *
 * @param n_route The index of the route to add the node to.
 * @param nodeToAdd The node to be added to the route.
 * @return The calculated supposed cost of adding the node to the route.
 */
double Schedule::calculateSupposedCost(int n_route, Node nodeToAdd) const {
    // Base route cost
    double cost = m_routes[n_route].getCost() - m_routes[n_route].getLastNode2DepotDistance();
    // Add travel to the next node time cost
    int distance = HCData::getDistance(m_routes[n_route].getLastPatientDistanceIndex(), nodeToAdd.getDistancesIndex());
    int arrival = m_routes[n_route].getFreeTime() + distance;
    int deltaTime = 0;
    if (arrival < nodeToAdd.getTimeWindowOpen()) {
        deltaTime = nodeToAdd.getTimeWindowOpen() - arrival;
    }
    else {
        if (arrival > nodeToAdd.getTimeWindowClose()) {
            deltaTime = arrival - nodeToAdd.getTimeWindowClose();
        }
    }
    // calcolate supposed cost
    cost += distance * HCData::TRAVEL_TIME_WEIGHT + deltaTime * HCData::EXTRA_TIME_WEIGHT;
    return cost;
}

/**
 * Searches for the best route for a given patient and synchronization index.
 *
 * @param patient The patient to find a route for.
 * @param syncIndex The synchronization index to exclude from the search.
 * @return The index of the best route, or HCData::NO_INDEX if no suitable route is found.
 */
int Schedule::searchForRoute(Patient patient, int syncIndex) const {
    string request = patient.getCurrentService().getService();
    int bestRoute = HCData::NO_INDEX;
    double bestCost = HCData::MAX_COST;
    // searching for the best route
    for (int i = 0; i < m_routes.size(); ++i) {
        if (i != syncIndex && m_routes[i].hasService(request) && patient.isCaregiverValid(m_routes[i].getCaregiver())) {
            double cost = calculateSupposedCost(i, Node(patient, 0));
            if (cost < bestCost) {
                bestCost = cost;
                bestRoute = i;
            }
        }
    }
    return bestRoute;
}

/**
 * Calculates the supposed arrival time for a patient node in a route.
 *
 * @param n_route The index of the route.
 * @param distanceIndex The index of the patient node for which the arrival time is calculated.
 * @param openWT The open waiting time for the patient node.
 * @return The calculated supposed arrival time.
 */
int Schedule::getSupposedArrivalTime(int n_route, int distanceIndex, int openWT) const {
    if (!isIndexValid(n_route)) {
        throw out_of_range("[Schedule] Error: Invalid route index");
    }
    int lastPatientIndex = m_routes[n_route].getLastPatientDistanceIndex();
    return max(
        m_routes[n_route].getFreeTime() + HCData::getDistance(lastPatientIndex, distanceIndex),
        openWT);
}

//////////////////////////////////////////////////////////////////////////////////////       GETTER

/**
 * Returns the schedule as a vector of Route objects.
 *
 * @return A vector containing all the routes in the schedule.
 */
std::vector<Route> Schedule::getSchedule() const {
    return m_routes;
}

/**
 * Returns the route at the specified index.
 *
 * @param n_route The index of the route to retrieve.
 * @return The route at the specified index.
 * @throws std::out_of_range If the index is out of range.
 */
Route Schedule::getRoute(int n_route) const noexcept(false) {
    if (!isEmpty() && isIndexValid(n_route)) {
        return m_routes[n_route];
    }
    throw out_of_range("\n[Schedule] ERROR: Index out of range");
}

/**
 * Retrieves a patient node from the specified route.
 *
 * @param n_route The index of the route to retrieve the node from.
 * @param n_node The index of the node to retrieve from the specified route.
 * @return The patient node at the specified index in the specified route.
 * @throws std::out_of_range If the route index or node index is out of range.
 */
Node Schedule::getNodeFromRoute(int n_route, int n_node) const noexcept(false) {
    if (!isEmpty() && isIndexValid(n_route)) {
        return m_routes[n_route].getPatientNode(n_node);
    }
    throw out_of_range("\n[Schedule] ERROR: Index out of range");
}

/**
 * Returns the caregiver assigned to the specified route.
 *
 * @param n_route The index of the route to retrieve the caregiver for.
 * @return The caregiver assigned to the specified route.
 * @throws std::out_of_range If the specified route index is out of range.
 */
std::string Schedule::getCaregiverOfRoute(int n_route) const noexcept(false) {
    if (isIndexValid(n_route)) {
        return m_routes[n_route].getCaregiver();
    }
    throw out_of_range("\n[Schedule] ERROR: Index out of range");
}

//////////////////////////////////////////////////////////////////////////////////////       GET DATA

/**
 * Calculates the total cost of the schedule based on various factors such as maximum idle time, maximum tardiness, total tardiness, total waiting time, travel time, and extra time.
 *
 * @return The total cost of the schedule.
 */
double Schedule::getCost() const {
    if (m_routes.empty()) {
        return HCData::MAX_COST;
    }
    int maxIdleTime = 0;
    int maxTardiness = 0;
    int totalTardiness = 0;
    int totalWaitingTime = 0;
    int travelTime = 0;
    int totalExtraTime = 0;
    // calculate variables
    for (const Route &route : m_routes) {
        maxIdleTime = route.getMaxIdleTime() > maxIdleTime ? route.getMaxIdleTime() : maxIdleTime;
        maxTardiness = route.getMaxTardiness() > maxTardiness ? route.getMaxTardiness() : maxTardiness;
        totalTardiness += route.getTotalTardiness();
        totalWaitingTime += route.getTotalWaitingTime();
        travelTime += route.getTravelTime();
        totalExtraTime += route.getExtraTime();
    }
    // calculate cost and return
    return HCData::MAX_WAIT_TIME_WEIGHT * maxIdleTime + HCData::MAX_TARDINESS_WEIGHT * maxTardiness +
        HCData::TARDINESS_WEIGHT * totalTardiness + HCData::TOT_WAITING_TIME_WEIGHT * totalWaitingTime +
        HCData::EXTRA_TIME_WEIGHT * totalExtraTime + HCData::TRAVEL_TIME_WEIGHT * travelTime;
}

/**
 * Returns the number of routes in the schedule.
 *
 * @return The number of routes in the schedule.
 */
int Schedule::getNumberOfRoutes() const {
    return m_routes.size();
}

/**
 * Returns the number of nodes in the specified route.
 *
 * @param n_route The index of the route to get the number of nodes for.
 * @return The number of nodes in the specified route.
 * @throws std::out_of_range If the given route index is out of range.
 */
int Schedule::getNumberOfNodesInRoute(int n_route) const noexcept(false) {
    if (!isEmpty() && isIndexValid(n_route)) {
        return m_routes[n_route].getNumNodes();
    }
    throw out_of_range("\n[Schedule] ERROR: Index out of range");
}

int Schedule::getNumberOfCaregiversFor(string service) const {
  return std::count_if(m_routes.begin(), m_routes.end(),
                       [service](const Route& route) { return route.hasService(service); });
}

//////////////////////////////////////////////////////////////////////////////////////       CHECKER

/**
 * Checks if the specified service is available in the given route.
 *
 * @param service The service to check for.
 * @param n_route The index of the route to check.
 * @return `true` if the service is available in the route, `false` otherwise.
 * @throws `std::out_of_range` if the route index is invalid.
 */
bool Schedule::isServiceAvailableInRoute(string service, int n_route) const noexcept(false) {
    if (!isEmpty() && isIndexValid(n_route)) {
        return m_routes[n_route].hasService(service);
    }
    throw out_of_range("\n[Schedule] ERROR: Index out of range");
}

/**
 * Checks if the given route index is valid within the schedule.
 *
 * @param n_route The index of the route to check.
 * @return `true` if the index is valid, `false` otherwise.
 */
bool Schedule::isIndexValid(int n_route) const {
    return n_route >= 0 && n_route < m_routes.size() && !m_routes.empty();
}

/// Returns true if the schedule has no routes, false otherwise.
bool Schedule::isEmpty() const {
    return m_routes.empty();
}

//////////////////////////////////////////////////////////////////////////////////////       ADD NODES

/**
 * Greedily appends a patient to the schedule, finding the best route for the patient.
 *
 * @param patient The patient to be appended to the schedule.
 * @return 0 on success, or a non-zero value on failure.
 */
int Schedule::greedyAppend(Patient patient) {
    int n_route = searchForRoute(patient, HCData::NO_INDEX);
    int time = getSupposedArrivalTime(n_route, patient.getDistancesIndex(), patient.getWindowStartTime());
    if (patient.getSync() != NoSync) {
        Patient syncPS(patient.getPatientAndNextService());
        int bestSync = searchForRoute(syncPS, n_route);
        int syncTime = getSupposedArrivalTime(bestSync, syncPS.getDistancesIndex(), syncPS.getWindowStartTime());
        if (patient.getSync() == Simultaneous) {
            time = max(time, syncTime);
            syncTime = time;
        }
        else if (patient.getSync() == Sequential) {
            if (syncTime > time + syncPS.getMaxWait()) {
                time = syncTime - syncPS.getMaxWait();
            }
            else if (syncTime < time + syncPS.getMinWait()) {
                syncTime = time + syncPS.getMinWait();
            }
        }
        m_routes[bestSync].appendNode(Node(syncPS, syncTime), syncTime);
    }
    m_routes[n_route].appendNode(Node(patient, time), time);
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////       UPDATER

/**
 * Replaces the route at the specified index in the schedule.
 *
 * @param route The new route to replace the existing one.
 * @param n_route The index of the route to be replaced.
 * @return 1 on success, or throws an `out_of_range` exception if the index is invalid.
 */
void Schedule::replaceRoute(Route &route, int n_route) {
    if (!isIndexValid(n_route)) { throw out_of_range("[Schedule] Error: route index out of range"); }
    m_routes[n_route] = route;
}

/**
 * Updates the arrival time of a node in the specified route.
 *
 * @param n_route The index of the route to update.
 * @param n_node The index of the node to update.
 * @param newTime The new arrival time for the node.
 * @throws std::out_of_range if the route index is invalid.
 */
void Schedule::updateNodeTime(int n_route, int n_node, int newTime) {
    if (!isIndexValid(n_route)) {
        throw out_of_range("[Schedule] Error out of range");
    }
    m_routes[n_route].updateNodeTime(n_node, newTime);
}