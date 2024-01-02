#ifndef CAREGIVER_HPP
#define CAREGIVER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <json/json.h>

using namespace std;

namespace homecare {

class Caregiver
{
private:
    string m_id;
    vector<string> m_services;
    string m_depot;
    int m_depotDistanceIndex;
    int m_shiftStartTime;
    int m_shiftEndTime;

public:
    Caregiver(string, vector<string>, string, int, int, int);
    Caregiver(Json::Value) noexcept (false);
    ~Caregiver();
    string toString() const;
    string getID() const;
    int getDepotDistanceIndex() const;
    int getShiftStartTime() const;
    int getShiftEndTime() const;
    string getDepot() const;
    vector<string> getServicesList() const;
    bool isWorking(int) const;
};

}

#endif