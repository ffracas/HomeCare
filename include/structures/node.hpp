#ifndef NODE_HPP
#define NODE_HPP

#include <sstream>

#include "../patient/patient.hpp"
#include "../caregiver/caregiver.hpp"

namespace homecare {

class Node {
private:
    std::string m_id;
    int m_timeWindowOpen;
    int m_timeWindowClose;
    int m_distanceIndex;
    int m_duration;
    std::string m_service;
    int m_arrivalTime;
    int m_departureTime;
    bool m_isDepot;
    static const int ZERO;
    static const std::string NO_SERVICE;

public:
    Node(std::string, int, int, int, int = ZERO, std::string = NO_SERVICE);
    Node(Caregiver);
    Node(Patient, int);
    ~Node();
    //getter
    std::string getId() const;
    int getTimeWindowOpen() const;
    int getTimeWindowClose() const;
    int getServiceTime() const;
    int getDistancesIndex() const;
    std::string getService() const;
    std::string toString() const;
    int getDeparturTime() const;
    int getArrivalTime() const;
    //setter
    void setArrivalTime(int);
};
    
} // namespace homecare

#endif