#include "patient.hpp"

using namespace std;
using namespace homecare;

Patient::Patient(string t_id, double t_x, double t_y, int t_timeWindowOpen, int t_timeWindowClose, 
        int t_distanceIndex, vector<string> t_invalidCaregivers, vector<Service> t_services, 
        SyncType t_sync, int t_minWait, int t_maxWait) 
        : m_id (t_id), m_x (t_x), m_y (t_y), 
        m_timeWindowOpen (t_timeWindowOpen), m_timeWindowClose (t_timeWindowClose), m_distanceIndex (t_distanceIndex), 
        m_invalidCaregivers (t_invalidCaregivers), m_services (t_services), m_sync (t_sync), 
        m_minWait (t_minWait), m_maxWait (t_maxWait) {}

/**
 * Constructor for initializing a Patient object based on a JSON representation.
 *
 * @param t_patient The JSON representation of the patient.
 * @param t_defaultServices A vector of services to recover any missing information on patient service.
 * 
 * @throws std::runtime_error if the JSON object is not formatted correctly.
 */
Patient::Patient(Json::Value t_patient, vector<Service> t_defaultServices) {
    //patient object fields
    const string ID                 ("id");                         //String
    const string LOCATION           ("location");                   //Array of double
    const string TIME_WINDOW        ("time_window");                //Array of double
    const string CARES              ("required_caregivers");        //Array of obj
    const string SERVICE            ("service");                    //string
    const string DURATION           ("duration");                   //int
    const string DIS_MATRIX_INDEX   ("distance_matrix_index");      //Int distance matrix index
    const string SYNC_TYPE          ("synchronization");            //obj
    const string SYNC_TYPE_FIELD    ("type");                       //field type
    const string SYNC_WINDOW_FIELD  ("distance");                   //field window
    const string INVALID_CAREGIVERS ("incompatible_caregivers");    //Array of string


    if (!t_patient.isMember(ID)                     || !t_patient[ID].isString()        || 
            !t_patient.isMember(LOCATION)           || !t_patient[LOCATION].isArray()   || 
            !t_patient.isMember(TIME_WINDOW)        || !t_patient[TIME_WINDOW].isArray()|| 
            !t_patient.isMember(CARES)              || !t_patient[CARES].isArray()      || 
            !t_patient.isMember(DIS_MATRIX_INDEX)   || !t_patient[ID].isString() ) {
        throw std::runtime_error("Errore nel formato del file di configurazione del sistema.");
        return;
    }
    m_id = t_patient[ID].asString();
    m_x = t_patient[LOCATION][0].asDouble();
    m_y = t_patient[LOCATION][1].asDouble();
    m_timeWindowOpen  = t_patient[TIME_WINDOW][0].asInt();
    m_timeWindowClose = t_patient[TIME_WINDOW][1].asInt();
    m_distanceIndex = t_patient[DIS_MATRIX_INDEX].asInt();
    m_minWait = 0;
    m_maxWait = 0;
    m_sync = NoSync;
    if (t_patient[SYNC_TYPE].isMember(SYNC_TYPE_FIELD)) { 
        if (t_patient[SYNC_TYPE][SYNC_TYPE_FIELD].asString() == "sequential") {
            m_sync = Sequential;
            m_minWait = t_patient[SYNC_TYPE][SYNC_WINDOW_FIELD][0].asInt();
            m_maxWait = t_patient[SYNC_TYPE][SYNC_WINDOW_FIELD][1].asInt();
            if (m_minWait >= m_maxWait){
                throw std::runtime_error("Errore nel formato del file di configurazione del sistema.");
                return;
            }
        }
        else if (t_patient[SYNC_TYPE][SYNC_TYPE_FIELD].asString() == "simultaneous") { m_sync = Simultaneous; }
    }

    if (t_patient.isMember(INVALID_CAREGIVERS)) { 
        for (auto& invalid : t_patient[INVALID_CAREGIVERS]) {
            m_invalidCaregivers.push_back(invalid.asString()); 
        }
    }
    
    for (auto& service : t_patient[CARES]) {
        if (!service.isMember(SERVICE) || !service[SERVICE].isString()) {
            throw std::runtime_error("Errore nel formato del file di configurazione del sistema.");
            return;
        }
        string serv(service[SERVICE].asString());
        if (service.isMember(DURATION) && service[DURATION].isInt()) {
            m_services.push_back(Service(serv, service[DURATION].asInt()));
        }
        else if (!service.isMember(DURATION)) {
            auto it = find_if(t_defaultServices.begin(), t_defaultServices.end(), 
                        [&serv] (const Service& obj) { return obj.getService() == serv; });
            if (it == t_defaultServices.end()) {
                throw std::runtime_error("Errore nel formato del file di configurazione del sistema.");
                return;
            }
            m_services.push_back(Service(serv, it->getDuration()));
        } 
        else {
            throw std::runtime_error("Errore nel formato del file di configurazione del sistema.");
            return;
        }
    }
    
}

Patient::~Patient() {}

/**
 * This method writes the object into a string
*/
string Patient::toString() {
    stringstream ss;
    ss << "id: " << m_id << ' ';
    ss << "x: " << m_x << ' ';
    ss << "y: " << m_y << ' ';
    ss << "timeWindowOpen: " << m_timeWindowOpen << ' ';
    ss << "timeWindowClose: " << m_timeWindowClose << ' ';
    ss << "distanceIndex: " << m_distanceIndex << ' ';
    ss << "invalidCaregivers: ";
    for (const auto& caregiver : m_invalidCaregivers) {
        ss << caregiver << ", ";
    }
    ss << ' ';
    ss << "serviceCode: ";
    for (const auto& service : m_services) {
        ss << service.toString() << ", ";
    }
    ss << ' ';
    ss << "sync: " << m_sync << ' ';
    ss << "minWait: " << m_minWait << ' ';
    ss << "maxWait: " << m_maxWait << '\n';

    return ss.str();
}

/**
 * Getter for the patient's ID.
 * @return The patient's ID.
 */
string Patient::getID() const { return m_id; }

/**
 * Getter for the distances matrix index associated with the patient.
 * @return The distances index associated with the patient.
 */
int Patient::getDistancesIndex() const { return m_distanceIndex; }

/**
 * Getter for the start time of the patient's time window.
 * @return The start time of the patient's time window.
 */
int Patient::getWindowStartTime() const { return m_timeWindowOpen; }

/**
 * Getter for the end time of the patient's time window.
 * @return The end time of the patient's time window.
 */
int Patient::getWindowEndTime() const { return m_timeWindowClose; }

/**
 * Getter for the maximum waiting time for the patient.
 * @return Maximum waiting time for the patient.
 */
int Patient::getMaxWait() const { return m_maxWait; }

/**
 * Getter for the minimum waiting time for the patient.
 * @return Minimum waiting time for the patient.
 */
int Patient::getMinWait() const { return m_minWait; }

/**
 * Getter for the list of services associated with the patient.
 * @return The list of services associated with the patient.
 */
vector<Service> Patient::getServices() const { return m_services; }

/**
 * Getter for the list of invalid caregivers for the patient.
 * @return The list of invalid caregivers for the patient.
 */
vector<string> Patient::getInvalidCaregivers() const { return m_invalidCaregivers; }

SyncType Patient::getSync() const { return m_sync; }

Service Patient::getCurrentService() const { return m_services[0]; }

Service Patient::getNextService() const { 
    if (m_sync == NoSync) {
        throw std::runtime_error("Errore! Non è possibile attivare il prossimo servizio per questo paziente");
    }
    vector<Service> next(hasNextService() ? m_services.begin() + 1 : m_services.begin(), m_services.end());
    return next[0]; 
}

Patient Patient::getPatientAndNextService(int t_time) const {   
    if (m_sync == NoSync) {
        throw std::runtime_error("Errore! Non è possibile attivare il prossimo servizio per questo paziente");
    }
    if (m_sync == Simultaneous && !hasNextService()) { return *this; }
    t_time = t_time > m_timeWindowOpen ? t_time : m_timeWindowOpen;
    vector<Service> next(hasNextService() ? m_services.begin() + 1 : m_services.begin(), m_services.end());
    return Patient(m_id, m_x, m_y, t_time + m_minWait, t_time + m_maxWait, m_distanceIndex,
            m_invalidCaregivers, next, m_sync, m_minWait, m_maxWait);
}

bool Patient::hasNextService() const {
    if (m_sync == NoSync || m_services.size() <= 1) { return false; }
    return true; 
}

bool Patient::isFirstService(string service) const {
    return m_services[0].getService() == service;
}