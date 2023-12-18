#include "caregiver.hpp"

using namespace std;
using namespace homecare;

Caregiver::Caregiver(string t_id, vector<string> t_services, string t_depot, int t_depotIndex, 
                int t_startShift, int t_endShift) : m_id(t_id), m_services(t_services),
                m_depot(t_depot), m_depotDistanceIndex(t_depotIndex), 
                m_shiftStartTime(t_startShift), m_shiftEndTime(t_endShift) {}

Caregiver::Caregiver(Json::Value t_caregivers) {
    const string ID                     ("id");                     //String
    const string SERVICES               ("abilities");              //Array of string
    const string DEPOT_DISTANCES_INDEX  ("distance_matrix_index");  //int
    const string DEPOT                  ("starting_point_id");      //string
    const string SHIFT                  ("time_window");            //Array of int

    if (!t_caregivers.isMember(DEPOT_DISTANCES_INDEX)   || !t_caregivers[DEPOT_DISTANCES_INDEX].isInt() ||
            !t_caregivers.isMember(ID)                  || !t_caregivers[ID].isString()                 || 
            !t_caregivers.isMember(SERVICES)            || !t_caregivers[SERVICES].isArray()            ||
            !t_caregivers.isMember(DEPOT)               || !t_caregivers[DEPOT].isString()              ||
            !t_caregivers.isMember(SHIFT)               || !t_caregivers[SHIFT].isArray() ) {
        throw std::runtime_error("Errore nel formato del file di configurazione del sistema.");
        return;
    }

    m_id = t_caregivers[ID].asString();
    m_depot = t_caregivers[DEPOT].asString();
    m_shiftStartTime = t_caregivers[SHIFT][0].asInt();
    m_shiftEndTime = t_caregivers[SHIFT][1].asInt();
    m_depotDistanceIndex = t_caregivers[DEPOT_DISTANCES_INDEX].asInt();

    for (const auto& ability : t_caregivers[SERVICES]) {
        if (!ability.isString()) {
            throw std::runtime_error("Errore nel formato del file di configurazione del sistema.");
            return;
        }
        m_services.push_back(ability.asString());
    }
}

Caregiver::~Caregiver() {}

string Caregiver::toString() {
    stringstream ss;
    ss << "id: " << m_id << ' ';
    ss << "services: [";
    for (const string& service : m_services) {
        ss << service << ", ";
    }
    ss << "] "; 
    ss << "depot: " << m_depot << ' '; 
    ss << "shiftStartTime: " << m_shiftStartTime << ' '; 
    ss << "shiftEndTime: " << m_shiftEndTime << ' '; 
    ss << "depotDistanceIndex: " << m_depotDistanceIndex << '\n'; 
    
    return ss.str();
}