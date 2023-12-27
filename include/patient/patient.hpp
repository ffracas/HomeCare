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

enum SyncType { NoSync, Sequential, Simultaneous, Copy };

class Patient {
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
    Patient(string, double, double, int, int, int, vector<string>, vector<Service>, SyncType, int, int);
    ~Patient();
    string toString();
    string getID() const;
    SyncType getSync() const;
    int getDistancesIndex() const;
    int getWindowStartTime() const;   
    int getWindowEndTime() const;
    vector<Service> getServices() const;
    vector<string> getInvalidCaregivers() const;
    Service getCurrentService() const;
    Patient getPatientAndNextService() const noexcept (false);
};

}

#endif