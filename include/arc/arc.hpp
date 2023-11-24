#ifndef ARC_HPP
#define ARC_HPP

#include <iostream>
#include <sstream>
#include "../node/node.hpp"

namespace homecare{

class Arc{
    private:
        Node* m_departure;
        Node* m_arrival;
        double m_distance;
        double m_time;
        double m_arrival_time;                           //tempo di arrivo al nodo
        double m_departure_time;                         //tempo di partenza dal nodo
        double const NO_TIME = -1;                     //default di non assegnato

    public:
        Arc(Node* departure, Node* arrival, double distance, double time);
        ~Arc();
};

/**
 * @brief Constructor for the Arc class.
 * 
 * Initializes an Arc object with the specified attributes, representing a directed edge 
 * between two nodes in a graph.
 *
 * @param t_departure Pointer to the departure node of the arc.
 * @param t_arrival Pointer to the arrival node of the arc.
 * @param t_distance The distance or weight associated with the arc.
 * @param t_time The time or cost associated with traversing the arc.
 */
Arc::Arc(Node* t_departure, Node* t_arrival, double t_distance, double t_time)
        : m_arrival (t_arrival), m_departure (t_departure), m_distance (t_distance), 
            m_time (t_time), m_arrival_time (NO_TIME), m_departure_time (NO_TIME)  {}

Arc::~Arc(){}

}

#endif