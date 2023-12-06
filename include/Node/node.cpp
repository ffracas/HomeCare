#include "node.hpp"

using namespace homecare;

/**
 * @brief Constructor for the Node class.
 * 
 * Initializes a Node object with the specified attributes.
 *
 * @param t_id The unique identifier for the node.
 * @param t_id_city The identifier of the city to which the node belongs.
 * @param t_x_coord The x-coordinate of the node's location.
 * @param t_y_coord The y-coordinate of the node's location.
 * @param t_start_window The starting time of the time window for the node.
 * @param t_end_window The ending time of the time window for the node.
 * @param t_service The service time required at the node.
 * @param t_demand The demand associated with the node.
 * @param t_depot The flag that identify the depot node.
 */
Node::Node(int t_id, int t_id_city, double t_x_coord, double t_y_coord, 
            double t_start_window, double t_end_window, int t_service, int t_demand, bool t_depot = false)   
        : m_id(t_id), m_id_city(t_id_city), m_x_coord(t_x_coord), m_y_coord(t_y_coord),
            m_start_window_time(t_start_window), m_end_window_time(t_end_window), 
            m_service(t_service), m_demand(t_demand), m_depot(t_depot) { 
    m_sync = false;
    m_arrival_time = NO_TIME;
    m_departure_time = NO_TIME;
    m_indice_gestore_sync = NO_VALUE; 
    m_indice_nodo_sync = NO_VALUE; 
}

Node::~Node() {}

int Node::getID() { return m_id; }

int Node::getCityID() { return m_id_city; }
    
double Node::getXCoord() { return m_x_coord; }
    
double Node::getYCoord() { return m_y_coord; }

double Node::getWindowStartTime() { return m_start_window_time; }

double Node::getWindowEndTime() { return m_end_window_time; }

int Node::getService() { return m_service; }

int Node::getDemand() { return m_demand; }

double Node::getDeparturTime() { return m_departure_time; }

double Node::getArrvalTime() { return m_arrival_time; }

int Node::getSyncNode() { return m_indice_nodo_sync; }

bool Node::isSync() { return m_sync; }

bool Node::isInTimeWindow(double t_time) { return t_time <= m_end_window_time; }

void Node::setSyncConfig(bool t_sync, int t_handlerIndex) {
    m_sync = t_sync;
    m_indice_gestore_sync = t_handlerIndex;
}

void Node::setSyncDuplicateNode(int t_id, int t_syncNodeIndex) {
    m_id = t_id;
    m_indice_nodo_sync = t_syncNodeIndex;
    m_demand = 0;
}

void Node::setArrivalTime(double t_arrival) {
    m_arrival_time = t_arrival > m_start_window_time ? t_arrival : m_start_window_time;
    if(!m_depot){
        m_departure_time = m_arrival_time + 1.0 * m_service;
    }   
}

