#include "node.hpp"

using namespace std;
using namespace homecare;

const int Node::ZERO = 0;
const string Node::NO_SERVICE = "";

/**
 * Constructor for the Node class.
 * 
 * @param t_id: Identifier of the node.
 * @param t_openTimeWindow: Opening time of the node's time window.
 * @param t_closeTimeWindow: Closing time of the node's time window.
 * @param t_distanceIndex: Index of distances associated with the node.
 * @param t_duration: Duration of the service at the node.
 * @param t_service: Type of service associated with the node.
 */
Node::Node(string t_id, int t_openTimeWindow, int t_closeTimeWindow, 
            int t_distanceIndex, int t_duration, string t_service, bool t_interdepend) 
        : m_id (t_id), m_timeWindowOpen (t_openTimeWindow), m_timeWindowClose (t_closeTimeWindow),
        m_distanceIndex (t_distanceIndex), m_duration (t_duration), m_service (t_service), 
        m_arrivalTime (t_openTimeWindow), m_departureTime (t_openTimeWindow), 
        m_isDepot(false), m_interdependet (t_interdepend) {}

Node::Node(Patient t_patient, int t_arrivalTime) 
        : m_id (t_patient.getID()), m_timeWindowOpen (t_patient.getWindowStartTime()), 
        m_timeWindowClose (t_patient.getWindowEndTime()), m_distanceIndex (t_patient.getDistancesIndex()), 
        m_duration (t_patient.getCurrentService().getDuration()), 
        m_service (t_patient.getCurrentService().getService()), m_arrivalTime (t_arrivalTime), 
        m_isDepot(false), m_interdependet (t_patient.getSync() == NoSync ? false : true) {
    m_departureTime = m_duration + m_arrivalTime;
}

Node::Node(Caregiver t_caregiver) 
        : m_id(t_caregiver.getDepot()), m_timeWindowOpen (t_caregiver.getShiftStartTime()), 
        m_timeWindowClose(t_caregiver.getShiftEndTime()), m_distanceIndex(t_caregiver.getDepotDistanceIndex()),
        m_duration (ZERO), m_service(NO_SERVICE), m_arrivalTime (t_caregiver.getShiftStartTime()), 
        m_departureTime (t_caregiver.getShiftStartTime()), m_isDepot(true), m_interdependet (false) {}

Node::~Node() {}

/**
 * Get the identifier of the node.
 * 
 * @return Node identifier.
 */
string Node::getId() const { return m_id; }

/**
 * Get the opening time of the node's time window.
 * 
 * @return Opening time of the time window.
 */
int Node::getTimeWindowOpen() const { return m_timeWindowOpen; }

/**
 * Get the closing time of the node's time window.
 * 
 * @return Closing time of the time window.
 */
int Node::getTimeWindowClose() const { return m_timeWindowClose; }

/**
 * Get the duration of the service at the node.
 * 
 * @return Duration of the service.
 */
int Node::getServiceTime() const { return m_duration; }

/**
 * Get the index of distances associated with the node.
 * 
 * @return Index of distances.
 */
int Node::getDistancesIndex() const { return m_distanceIndex; }

int Node::getArrivalTime() const { return m_arrivalTime; }

int Node::getDeparturTime() const { return m_departureTime; }

/**
 * Get the type of service associated with the node.
 * 
 * @return Type of service.
 */
string Node::getService() const { return m_service; }

bool Node::isInterdependent() const { return m_interdependet; }

string Node::getHash() const { 
    stringstream ss;

    ss << "id:" << m_id;
    ss << ",ser:" << m_service;
    ss << ",art:" << m_arrivalTime;
    ss << ",dpt:" << m_departureTime;

    return ss.str();
}

/**
 * Get a textual representation of the node.
 * 
 * @return String with node information.
 */
string Node::toString() const { 
    stringstream ss;

    ss << "id: " << m_id << ' ';
    ss << "service: " << m_service << ' ';
    ss << "duration: " << m_duration << ' ';
    ss << "arrivalTime: " << m_arrivalTime << ' ';
    ss << "departureTime: " << m_departureTime << ' ';

    return ss.str();
}

void Node::setArrivalTime(int t_arrivalTime) { 
    m_arrivalTime = t_arrivalTime; 
    if (!m_isDepot) { m_departureTime = m_arrivalTime + m_duration; }
}
