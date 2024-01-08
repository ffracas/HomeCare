#ifndef DEPOT_HPP
#define DEPOT_HPP

#include <iostream>
#include <string>
#include <vector>
#include <json/json.h>

namespace homecare {

class Depot {
private:
    std::string m_id;
    double m_x;
    double m_y;
    //int m_distanceIndex;        
public:
    Depot(std::string, double, double);
    Depot(Json::Value) noexcept (false);
    ~Depot();
    std::string toString();
};

}

#endif