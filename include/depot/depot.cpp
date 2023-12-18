#include "depot.hpp"

using namespace std;
using namespace homecare;

/**
 * Constructor for initializing a Depot object with specific attributes.
 *
 * @param t_id The unique identifier of the depot.
 * @param t_xCoord The X-coordinate of the depot's location.
 * @param t_yCoord The Y-coordinate of the depot's location.
 */
Depot::Depot(string t_id, double t_xCoord, double t_yCoord) : m_id(t_id), m_x(t_xCoord), m_y(t_yCoord) {}

Depot::~Depot() {}

/**
 * Constructor for initializing a Depot object based on a JSON representation.
 *
 * @param t_depot The JSON representation of the depot.
 *                
 * @throws std::runtime_error if the JSON object is not formatted correctly.
 */
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

/**
 * This method writes the object into a string
*/
string Depot::toString() {
    stringstream ss;
    ss << "id: " << m_id << ' ';
    ss << "x: " << m_x << ' ';
    ss << "y: " << m_y << '\n';
    
    return ss.str();
}