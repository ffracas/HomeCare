#ifndef ARC_HPP
#define ARC_HPP

#include <json/json.h>

#include "node.hpp"

using namespace std;

namespace homecare {

class Arc
{
private:
    Node m_departure;
    Node m_arrival;
    int m_departureTime;
    int m_arrivalTime;
    int m_distance;
    int m_readyTime;

public:
    Arc(Node, Node, int, int, int);
    ~Arc();
    //getter
    Node getArrival() const;
    Node getDeparture() const;
    int getDistance() const;
    int getDeparturTime() const;
    int getArrvalTime() const;
    int getReadyTime() const;
    string toString() const;
    Json::Value getJSONArc() const;
};

}

#endif 