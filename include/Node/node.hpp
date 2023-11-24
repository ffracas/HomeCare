#ifndef NODE_HPP
#define NODE_HPP

#include <iostream>
#include <sstream>

namespace homecare{

class Node{
    private:
        int m_id;
        int m_id_city;
        double m_x_coord;                             //coordinata x
        double m_y_coord;                             //coordinata y
        double m_window_start_time;                   //inizio della finestra, dovrebbe essere int ed indicare i minuti ma bho
        double m_window_end_time;                     //fine della finestra, dovrebbe essere int ed indicare i minuti ma bho
        int m_service;                                //durata servizio in min
        int m_demand;

    public:
        Node(int, int, double, double, double, double, int, int);
        ~Node();
        int getCityID();
        double getXCoord();
        double getYCoord();
        double getWindowStartTime();
        double getWindowEndTime();
        int getService();
};

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
 */
Node::Node(int t_id, int t_id_city, double t_x_coord, double t_y_coord, 
            double t_start_window, double t_end_window, int t_service, int t_demand)   
        : m_id(t_id), m_id_city(t_id_city), m_x_coord(t_x_coord), m_y_coord(t_y_coord),
            m_window_start_time(t_start_window), m_window_end_time(t_end_window), 
            m_service(t_service), m_demand(t_demand) {}

Node::~Node(){}

}

#endif