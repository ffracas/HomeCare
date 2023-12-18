#include "service.hpp"

using namespace homecare;

/** 
 * Constructor for initializing a Service object.
 * 
 * @param t_service The name of the service.
 * @param t_duration The duration of the service.
 */
Service::Service(string t_service, int t_duration) : m_service (t_service), m_duration (t_duration) {}

/** 
 * Destructor for the Service class.
 */
Service::~Service() {}

/**
 * Constructor for initializing a Service object based on a JSON representation.
 *
 * @param t_service The JSON representation of the service.
 * @param t_patient Flag indicating whether the service is associated with a patient.
 */
Service::Service(Json::Value t_service, bool t_patient) {
    const string ID         (t_patient ? "service"  : "id");                    //String
    const string DURATION   (t_patient ? "duration" : "default_duration");      //Int

    if (!t_service.isMember(ID)             || !t_service[ID].isString()    ||
            !t_service.isMember(DURATION)   || !t_service[DURATION].isInt()) {
        throw std::runtime_error("Errore nel formato del file di configurazione del sistema.");
        return;
    }
    m_service = t_service[ID].asString();
    m_duration = t_service[DURATION].asInt();
}

string Service::toString() const {
    stringstream ss;
    ss << "id: " << m_service << " dur: " << m_duration;
    return ss.str();
}