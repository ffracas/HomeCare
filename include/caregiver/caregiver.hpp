#ifndef CAREGIVER_HPP
#define CAREGIVER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <json/json.h>

namespace homecare {

class Caregiver
{
private:
    std::string m_id;
    std::vector<std::string> m_services;
    std::string m_depot;
    int m_depotDistanceIndex;
    int m_shiftStartTime;
    int m_shiftEndTime;

public:
    Caregiver(std::string, std::vector<std::string>, std::string, int, int, int);
    Caregiver(Json::Value) noexcept (false);
    ~Caregiver();
    std::string toString() const;
    std::string getID() const;
    int getDepotDistanceIndex() const;
    int getShiftStartTime() const;
    int getShiftEndTime() const;
    std::string getDepot() const;
    std::vector<std::string> getServicesList() const;
    bool isWorking(int) const;
};

}

#endif