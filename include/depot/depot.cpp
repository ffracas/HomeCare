#include "depot.hpp"

using namespace std;
using namespace homecare;

Depot::Depot(string t_id, double t_xCoord, double t_yCoord) : m_id(t_id), m_x(t_xCoord), m_y(t_yCoord) {}

Depot::~Depot() {}

Depot::Depot(Json::Value t_depot) {
    const string ID         ("id");             //String
    const string LOCATION   ("location");       //Array of double

    if (!t_depot.isMember(ID)             || !t_depot[ID].isString()    ||
            !t_depot.isMember(LOCATION)   || !t_depot[LOCATION].isArray()) {
        throw std::runtime_error("Errore nel formato del file di configurazione del sistema.");
        return;
    }

    m_id = t_depot[ID].asString();
    m_x  = t_depot[LOCATION][0].asDouble();
    m_y  = t_depot[LOCATION][1].asDouble(); 
}

string Depot::toString() {
    stringstream ss;
    ss << "id: " << m_id << ' ';
    ss << "x: " << m_x << ' ';
    ss << "y: " << m_y << '\n';
    
    return ss.str();
}