#include "caregiver.hpp"

using namespace std;
using namespace homecare;

/**
 * @brief Constructor for initializing a Caregiver object with specific attributes.
 *
 * @param t_id The unique identifier of the caregiver.
 * @param t_services The vector of service IDs that the caregiver can provide.
 * @param t_depot The ID of the depot where the caregiver is stationed.
 * @param t_depotIndex The distance index to the depot.
 * @param t_startShift The starting time of the caregiver's shift.
 * @param t_endShift The ending time of the caregiver's shift.
 */
Caregiver::Caregiver(string t_id, vector<string> t_services, string t_depot, int t_depotIndex, 
                int t_startShift, int t_endShift) : m_id(t_id), m_services(t_services),
                m_depot(t_depot), m_depotDistanceIndex(t_depotIndex), 
                m_shiftStartTime(t_startShift), m_shiftEndTime(t_endShift) {}

/**
 * Constructor for initializing a Caregiver object from JSON data.
 *
 * @param t_caregivers The JSON object containing caregiver information.
 *                  
 * @throws std::runtime_error if the JSON object is not formatted correctly.
 */
Caregiver::Caregiver(Json::Value t_caregivers) {
    const string ID                     ("id");                     //String
    const string SERVICES               ("abilities");              //Array of string

    if (!t_caregivers.isMember(ID)                  || !t_caregivers[ID].isString()         || 
        !t_caregivers.isMember(SERVICES)            || !t_caregivers[SERVICES].isArray()    ) {
        throw std::runtime_error("Errore nel formato del file di configurazione del sistema.");
        return;
    }

    m_id = t_caregivers[ID].asString();
    m_depot = "d1";
    m_shiftStartTime = 0;
    m_shiftEndTime = 480;
    m_depotDistanceIndex = 0;

    for (const auto& ability : t_caregivers[SERVICES]) {
        if (!ability.isString()) {
            throw std::runtime_error("Errore nel formato del file di configurazione del sistema.");
            return;
        }
        m_services.push_back(ability.asString());
    }
}

Caregiver::~Caregiver() {}

/**
 * This method writes the object into a string
*/
string Caregiver::toString() const {
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

string Caregiver::getID() const { return m_id; }

int Caregiver::getDepotDistanceIndex() const { return m_depotDistanceIndex; }

int Caregiver::getShiftStartTime() const { return m_shiftStartTime; }

int Caregiver::getShiftEndTime() const { return m_shiftEndTime; }

string Caregiver::getDepot() const { return m_depot; }

vector<string> Caregiver::getServicesList() const { return m_services; }

bool Caregiver::isWorking(int actualTime) const { return actualTime < m_shiftEndTime + 240; /*straordinari*/ }