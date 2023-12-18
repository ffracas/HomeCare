#ifndef PATIENT_HPP
#define PATIENT_HPP

#include <iostream>
#include <string>
#include <vector>
#include <json/json.h>

#include "../service/service.hpp"

using namespace std;
namespace homecare
{

enum SyncType { NoSync, Sequential, Simultaneous };

class Patient
{
private:
    string m_id;
    double m_x;
    double m_y;
    int m_timeWindowOpen;
    int m_timeWindowClose;
    int m_distanceIndex;
    vector<string> m_invalidCaregivers;
    vector<Service> m_services;
    SyncType m_sync;
    int m_minWait;
    int m_maxWait;
    
public:
    Patient(Json::Value, vector<Service>) noexcept (false);
    ~Patient();
    string toString();
};

}

#endif