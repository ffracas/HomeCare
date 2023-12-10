#ifndef ARC_HPP
#define ARC_HPP

#include <iostream>
#include <sstream>
#include "../node/node.hpp"

namespace homecare{

class Arc{
    private:
        Node m_departure;
        Node m_arrival;
        double m_distance;
        double m_arrival_time;                          //tempo di arrivo al nodo
        double m_departure_time;                        //tempo di partenza dal nodo
        double m_ready_time;

    public:
        Arc(Node, Node, double, double, double);
        ~Arc();
        //getter
        Node getArrival();
        Node getDeparture();
        double getDistance();
        double getDeparturTime();
        double getArrvalTime();
        double getReadyTime();
};

}

#endif