#include "scheduleoptimiser.hpp"

using namespace std;
using namespace homecare;

ScheduleOptimiser::ScheduleOptimiser(const Schedule& schedule) : Schedule(schedule) {
    for (int i = 0; i < getNumberOfRoutes(); ++i) {
        for (int j = 1; j < getNumberOfNodesInRoute(i); ++j) {
            Node n = getNodeFromRoute(i, j);
            int patientIndex = HCData::getPatientPosition(n.getId());
            m_mapOfPatient[n.getId()].insertService(n.getService(), InfoNode(i, j, n.getArrivalTime(), patientIndex));
        }
    }
}

ScheduleOptimiser::ScheduleOptimiser() : Schedule() {}

ScheduleOptimiser::~ScheduleOptimiser() {}

/////////////////////////////////////////////////////////////////////////////   GETTERS

int ScheduleOptimiser::getNPatientServices(string patient) const {
    auto it = m_mapOfPatient.find(patient); // Cerca la chiave nella mappa
    if (it != m_mapOfPatient.end()) {
        // Restituisce il riferimento all'elemento se la chiave esiste
        return it->second.getAllService().size();
    } else { return 0; }
}

vector<InfoNode> ScheduleOptimiser::getPatientServices(string patient) const {
    return m_mapOfPatient.at(patient).getAllService();
}

SyncWindows ScheduleOptimiser::getServiceWindows(int n_route) const noexcept (false) {
    if (!isIndexValid(n_route)) { throw out_of_range("No route"); }
    SyncWindows result;
    for (auto& node : getRoute(n_route).getNodes()) {
        if (node.isInterdependent()) {
            result.addSyncWindow(node.getService(), getSyncServiceWindow(node.getId(), node.getService(), n_route));
        }
    }
    return result;
}

/**
 * Returns the earliest start time and latest end time of the service window
 * for the given patient and service, taking into account synchronization with
 * another service scheduled in the given route.
 *
 * @param patient The ID of the patient
 * @param service The service to get the window for
 * @param currentRoute The route that already has a service scheduled
 * @return A pair with the earliest start and latest end times of the window
 */
pair<int, int> ScheduleOptimiser::getSyncServiceWindow(string patient, string service, int currentRoute) const {
    InfoNode otherNode(m_mapOfPatient.at(patient).getOtherServiceInfo(service, currentRoute).second);
    Patient p = HCData::getPatient(otherNode.getPatientIndex());
    int late;
    int soon;
    late = getRoute(otherNode.getRoute()).getNoChangeWindowCloseTime(otherNode.getPositionInRoute());
    soon = getRoute(otherNode.getRoute()).getNoChangeWindowOpenTime(otherNode.getPositionInRoute());
    if (soon > p.getWindowEndTime()) {
        soon = otherNode.getTime();
        late = soon;
    }
    if (p.isFirstService(service)) { return make_pair(max(soon - p.getMaxWait(), 0), late - p.getMinWait()); }
    else { return make_pair(soon + p.getMinWait(), late + p.getMaxWait()); }
}

/**
 * Returns the information about the other service that is interdependent with the given service for the specified patient and current route.
 *
 * @param patient The ID of the patient.
 * @param service The name of the service.
 * @param currentRoute The index of the current route.
 * @return A pair containing the name of the other service and the corresponding InfoNode.
 * @throws runtime_error if no other node is assigned for the given service.
 */
pair<string, InfoNode> ScheduleOptimiser::getInterdependetInfo(string patient, string service, int currentRoute) {
    return m_mapOfPatient[patient].getOtherServiceInfo(service, currentRoute);
}
    
/////////////////////////////////////////////////////////////////////////////   UPDATE STRUCTURES

/**
 * Replaces the specified route in the schedule with the provided route.
 * This updates the internal data structures to reflect the new route.
 *
 * @param route The new route to replace the existing route.
 * @param n_route The index of the route to replace.
 * @throws std::out_of_range if the route index is out of range.
 */
void ScheduleOptimiser::replaceRoute(Route &route, int n_route) {
    if (!isIndexValid(n_route)) { throw out_of_range("[ScheduleOPT] Error: route index out of range"); }
    Schedule::replaceRoute(route, n_route);
    for (int i = 1; i < route.getNumNodes(); ++i) {
        Node node = route.getPatientNode(i);
        if (!m_mapOfPatient[node.getId()].relocateNode(node.getService(), n_route, node.getArrivalTime(), i)) {
            m_mapOfPatient[node.getId()].insertService(node.getService(), 
                    InfoNode(n_route, i, node.getArrivalTime(), HCData::getPatientPosition(node.getId())));
        }
    }
}

/**
 * Destroys a node in the schedule at the specified route and position.
 *
 * @param n_route The index of the route to destroy the node from.
 * @param pos_node The position of the node to destroy within the route.
 * @param patient The patient associated with the node to be destroyed.
 * @throws std::out_of_range If the route index or node position is invalid.
 */
void ScheduleOptimiser::destroyNode(int n_route, int pos_node, string patient) {
    if (!isIndexValid(n_route) || !isNodeIndexValid(n_route, pos_node)) {
        throw out_of_range("[Schedule] Error: route index out of range");
    }
    SyncWindows sw = getServiceWindows(n_route);
    Schedule::destroyNode(n_route, pos_node, sw);
    m_mapOfPatient.at(patient).destroyAll();    
}

void ScheduleOptimiser::repairNode(int n_route, const Patient& patient) {
    if (!isIndexValid(n_route)) { throw out_of_range("[Schedule] Error: route index out of range"); }
    SyncWindows sw = getServiceWindows(n_route);
    volatile int time = getRoute(n_route).getFreeTime() + 100;
    Node node = Node(patient, time);
    Schedule::repairNode(n_route, node, sw);
    updateMapOfPatient(getRoute(n_route), n_route);
}

void ScheduleOptimiser::updateMapOfPatient(const Route &route, int n_route) {
    for (int i = 1; i < route.getNumNodes(); ++i) {
        Node node = route.getPatientNode(i);
        m_mapOfPatient[node.getId()].relocateNode(node.getService(), n_route, node.getArrivalTime(), i);
    }
}

/**
 * Updates the sync service time for a patient's service on the given route.
 *
 * @param patient The patient whose service time is being updated.
 * @param service The service whose time is being updated.
 * @param time The new time for the service.
 * @param currentRoute The route on which the service is being updated.
 * @throws runtime_error if there is no other node assigned for the service.
 */
void ScheduleOptimiser::updateSyncServiceTime(string patient, string service, int time, int currentRoute)
{
    pair<string, InfoNode> otherNode(m_mapOfPatient[patient].getOtherServiceInfo(service, currentRoute));
    if (!otherNode.second.isAssigned()) { throw runtime_error("[ScheduleOPT] Error: no other node"); }
    Patient p = HCData::getPatient(otherNode.second.getPatientIndex());
    int openWin = 0;
    int closeWin = 0;
    if (p.isFirstService(service)) {
        openWin = time + p.getMinWait();
        closeWin = time + p.getMaxWait();
    }
    else {
        openWin = time - p.getMaxWait();
        closeWin = time - p.getMinWait();
    }
    otherNode = m_mapOfPatient[patient].update(otherNode.first, otherNode.second.getRoute(), openWin, closeWin);
    updateNodeTime(otherNode.second.getRoute(), otherNode.second.getPositionInRoute(), otherNode.second.getTime());
}

/////////////////////////////////////////////////////////////////////////////   CHECKER

bool ScheduleOptimiser::isNodeIndexValid(int n_route, int n_node) const {
    if (!isIndexValid(n_route)) { return false; }
    if (!getRoute(n_route).isIndexNodeValid(n_node)) { return false; }
    return true;
}