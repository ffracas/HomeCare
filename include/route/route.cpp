#include "route.hpp"

using namespace homecare;
using namespace std;

/**
 * @brief Constructor for the Node class.
 * 
 * Initializes a Route object.
*/
Route::Route(Node t_depot) : m_depot(t_depot), m_quantity(0.0), m_currentTime(0.0), m_lastClientID(t_depot.getID()) {}

Route::~Route() {}

double Route::getFreeTime() { return m_arcs_in_route[m_arcs_in_route.size() - 1].getReadyTime(); }

int Route::getLastClient() { return m_lastClientID; }

int Route::addNode(Node t_newNode, double** t_distances, int t_delay) {
    Node lastNode = Node(m_depot);
    if(m_arcs_in_route.size() >= BASE_ROUTE_LEN) {
        m_arcs_in_route.erase(m_arcs_in_route.end() - 1);           //delete last arc
        lastNode = Node(m_arcs_in_route[m_arcs_in_route.size() - 1].getArrival());
    }
    //insert new arc
    double distanceLastNew = t_distances[lastNode.getID()][t_newNode.getID()];
    m_arcs_in_route.push_back(Arc(lastNode, t_newNode, distanceLastNew, 
                            m_currentTime, m_currentTime + distanceLastNew));    
    //insert new last arc
    double newTime = m_arcs_in_route[m_arcs_in_route.size() - 1].getReadyTime() + distanceLastNew;
    double distanceNewDepot = t_distances[t_newNode.getID()][m_depot.getID()];
    m_arcs_in_route.push_back(Arc(t_newNode, m_depot, distanceNewDepot, 
                            newTime, newTime + distanceNewDepot)); 
    //update variables   
    m_quantity += t_newNode.getDemand();
    m_currentTime = m_arcs_in_route[m_arcs_in_route.size() - 1].getReadyTime();
    
    return m_arcs_in_route.size() - 1; //do not count the arc that return to depot
}

bool Route::isSuitableFor(Node t_node) {
    return t_node.getDemand() + m_quantity <= m_maxQuantity;
}

string Route::getRouteToString() {
    string s_route = "ROUTE\n";
    for (Arc arc : m_arcs_in_route){
        s_route += "ARC - partenza: " + to_string(arc.getDeparture().getID()) 
            + " alle: " + to_string(arc.getDeparturTime()) 
            + " . arrivo: " + to_string(arc.getArrival().getID()) 
            + " alle: " + to_string(arc.getArrvalTime()) 
            + " servizio min: " + to_string(arc.getArrival().getService())  + "\n";
    }
    return s_route;
}