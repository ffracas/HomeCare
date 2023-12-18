#ifndef DEPOT_HPP
#define DEPOT_HPP

#include <iostream>
#include <string>
#include <vector>
#include <json/json.h>

using namespace std;

namespace homecare {

class Depot {
private:
    string m_id;
    double m_x;
    double m_y;
    //int m_distanceIndex;        
public:
    Depot(string, double, double);
    Depot(Json::Value) noexcept (false);
    ~Depot();
    string toString();
};

}

#endif