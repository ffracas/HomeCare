#ifndef NODE_HPP
#define NODE_HPP

#include <sstream>

#include "../patient/patient.hpp"
#include "../caregiver/caregiver.hpp"

using namespace std;

namespace homecare {

class Node {
private:
    string m_id;
    int m_timeWindowOpen;
    int m_timeWindowClose;
    int m_distanceIndex;
    int m_duration;
    string m_service;
    int m_arrivalTime;
    int m_departureTime;
    static const int ZERO;
    static const string NO_SERVICE;

public:
    Node(string, int, int, int, int = ZERO, string = NO_SERVICE);
    Node(Caregiver);
    Node(Patient, int);
    ~Node();
    //getter
    string getId() const;
    int getTimeWindowOpen() const;
    int getTimeWindowClose() const;
    int getServiceTime() const;
    int getDistancesIndex() const;
    string getService() const;
    string toString() const;
    int getDeparturTime() const;
    int getArrivalTime() const;
    //setter
    void setArrivalTime(int);
};
    
} // namespace homecare

#endif