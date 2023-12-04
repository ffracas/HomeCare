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

    public:
        Arc(Node, Node, double);
        ~Arc();
        Node getArrival();
        Node getDeparture();
        double getDistance();
};

}

#endif