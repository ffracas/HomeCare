#ifndef PATIENT_HPP
#define PATIENT_HPP

#include <iostream>
#include <string>
#include <vector>
#include <json/json.h>

#include "../service/service.hpp"

namespace homecare
{

enum SyncType { NoSync, Sequential, Simultaneous };

class Patient {
private:
    std::string m_id;
    double m_x;
    double m_y;
    int m_timeWindowOpen;
    int m_timeWindowClose;
    int m_distanceIndex;
    std::vector<std::string> m_invalidCaregivers;
    std::vector<Service> m_services;
    SyncType m_sync;
    int m_minWait;
    int m_maxWait;
    
public:
    Patient(Json::Value, std::vector<Service>) noexcept (false);
    Patient(std::string, double, double, int, int, int, std::vector<std::string>, std::vector<Service>, SyncType, int, int);
    ~Patient();
    std::string toString();
    std::string getID() const;
    SyncType getSync() const;
    int getDistancesIndex() const;
    int getWindowStartTime() const;   
    int getWindowEndTime() const;
    int getMinWait() const;
    int getMaxWait() const;
    std::vector<Service> getServices() const;
    std::vector<std::string> getInvalidCaregivers() const;
    Service getCurrentService() const;
    Service getNextService() const;
    Patient getPatientAndNextService(int = 0) const noexcept (false);
    Patient getPatientforS2Sync() const noexcept (false);
    Patient getPatientforS1Sync(int) const noexcept (false);
    bool hasNextService() const;
    bool isFirstService(std::string) const;
    bool isCaregiverValid(std::string) const;
};

}

#endif