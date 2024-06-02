#include "validationode.hpp"

using namespace std;
using namespace homecare;

const int ValidatioNode::NO_TIME = -1;
const int ValidatioNode::FAIL = 0;
const int ValidatioNode::OK = 1;
const int ValidatioNode::FIRST = 0;
const int ValidatioNode::SECOND = 1;

ValidatioNode::ValidatioNode(Patient t_patient) : m_patientID (t_patient.getID()), m_sync (t_patient.getSync()),
        m_minWait (t_patient.getMinWait()), m_maxWait (t_patient.getMaxWait()) {
    m_services.push_back(ServiceInfo(t_patient.getCurrentService().getService(), 
        t_patient.getCurrentService().getDuration(), t_patient.getWindowStartTime(), t_patient.getWindowEndTime())); 
    if (t_patient.getSync() != NoSync) {
        Patient patient = t_patient.hasNextService() ? t_patient.getPatientAndNextService() : t_patient;
        m_services.push_back(ServiceInfo(patient.getCurrentService().getService(), 
            patient.getCurrentService().getDuration(), patient.getWindowStartTime(), patient.getWindowEndTime()));
    }
}

ValidatioNode::~ValidatioNode() {}

bool ValidatioNode::isCompleted() const { 
    for (const ServiceInfo& si : m_services) {
        if (!si.m_completed) { return false; }
    }   
    return true; 
}

string ValidatioNode::getPatient() const { return m_patientID; }

bool ValidatioNode::checkService(string t_service, int t_arrivalTime, int t_departureTime, int serviceI, int otherI) {
    // check if service is valid
    bool isServiceValid = !m_services[serviceI].m_completed &&
                          t_arrivalTime >= m_services[serviceI].m_openWindow &&
                          t_departureTime == t_arrivalTime + m_services[serviceI].m_duration;
    if (!isServiceValid) { return false; }
    // flag completed service and update times
    m_services[serviceI].m_completed = true;
    m_services[serviceI].m_arrivalTime = t_arrivalTime;
    m_services[serviceI].m_departureTime = t_departureTime;
    // only one service available
    if (m_services.size() == 1) { return true; }
    // if patient analysis is not completed, return true
    if (!m_services[otherI].m_completed) { return true; }
    // if patient analysis is completed, check if the services are in sync
    if (m_sync == Simultaneous) {
        return (m_services[SECOND].m_arrivalTime == m_services[FIRST].m_arrivalTime 
                && m_services[SECOND].m_departureTime == m_services[FIRST].m_departureTime);      
    } else if (m_sync == Sequential) {
        return (m_services[SECOND].m_arrivalTime >= m_services[FIRST].m_arrivalTime + m_minWait &&
            m_services[SECOND].m_arrivalTime <= m_services[FIRST].m_arrivalTime + m_maxWait);
    }      
    return false;
}

//Validate on max 2 services
int ValidatioNode::setTime(string t_service, int t_arrivalTime, int t_departureTime) {
    if (m_services[FIRST].m_service == t_service) {
        if (checkService(t_service, t_arrivalTime, t_departureTime, FIRST, SECOND)) { return OK; }
    }
    if (m_services.size() > 1 && m_services[SECOND].m_service == t_service) {
        if (checkService(t_service, t_arrivalTime, t_departureTime, SECOND, FIRST)) { return OK; }
    }
    
    return FAIL; 
}


