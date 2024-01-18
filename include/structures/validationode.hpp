#ifndef VALIDATIONODE_HPP
#define VALIDATIONODE_HPP

#include <sstream>
#include <vector>

#include "../patient/patient.hpp"

namespace homecare {

class ValidatioNode {
private:
    struct ServiceInfo {
        std::string m_service;
        bool m_completed;
        int m_duration;
        int m_openWindow;
        int m_closeWindow;
        int m_arrivalTime;
        int m_departureTime;
        ServiceInfo(std::string t_name, int t_duration, int t_openWin, int t_closeWin) 
            : m_service (t_name), m_completed (false), m_duration (t_duration), 
            m_openWindow (t_openWin), m_closeWindow (t_closeWin), 
            m_arrivalTime (ValidatioNode::NO_TIME), m_departureTime (ValidatioNode::NO_TIME) {}
    };
    std::string m_patientID;
    std::vector<ServiceInfo> m_services;
    SyncType m_sync;
    int m_minWait, m_maxWait;
    bool checkService(std::string, int, int, int, int);
    static const int FIRST, SECOND;

public:
    static const int NO_TIME;
    static const int FAIL;
    static const int OK;
    ValidatioNode(Patient);
    ~ValidatioNode();
    bool isCompleted() const;
    std::string getPatient() const;
    int setTime(std::string, int, int);
};

}

#endif