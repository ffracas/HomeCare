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

ScheduleOptimiser::~ScheduleOptimiser() {}

/////////////////////////////////////////////////////////////////////////////   GETTERS

int ScheduleOptimiser::getNPatientServices(string patient) const {
    auto it = m_mapOfPatient.find(patient); // Cerca la chiave nella mappa
    if (it != m_mapOfPatient.end()) {
        // Restituisce il riferimento all'elemento se la chiave esiste
        return it->second.getAllService().size();
    } else {
        return 0;
    }
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
    if (p.isFirstService(service)) {
        return make_pair(max(soon - p.getMaxWait(), 0), late - p.getMinWait());
    }
    else {
        return make_pair(soon + p.getMinWait(), late + p.getMaxWait());
    }
}

pair<string, InfoNode> ScheduleOptimiser::getInterdependetInfo(string patient, string service, int currentRoute) {
    return m_mapOfPatient[patient].getOtherServiceInfo(service, currentRoute);
} 
    

/////////////////////////////////////////////////////////////////////////////   UPDATE STRUCTURES
// FIXME: implement
int ScheduleOptimiser::replaceRoute(Route& route, int n_route) {
    if (!isIndexValid(n_route)) { 
        throw out_of_range("[ScheduleOPT] Error: route index out of range"); 
    }
    
}

void ScheduleOptimiser::destroyReferencesForPatient(string patient) {
    m_mapOfPatient.at(patient).destroyAll();
} 

void ScheduleOptimiser::updateSyncServiceTime(string patient, string service, int time, int currentRoute) { 
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