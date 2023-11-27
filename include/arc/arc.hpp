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
        double m_arrival_time = NO_TIME;                           //tempo di arrivo al nodo
        double m_departure_time = NO_TIME;                         //tempo di partenza dal nodo
        static const int NO_TIME = -1;                             //default di non assegnato

    public:
        Arc(Node* departure, Node* arrival, double distance, double time);
        ~Arc();
};

}

#endif