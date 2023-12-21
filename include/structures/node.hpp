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
    static const int ZERO;
    static const string NO_SERVICE;

public:
    Node(string, int, int, int, int = ZERO, string = NO_SERVICE);
    Node(Caregiver);
    ~Node();
    string getId() const;
    int getTimeWindowOpen() const;
    int getTimeWindowClose() const;
    int getServiceTime() const;
    int getDistancesIndex() const;
    string getService() const;
    string toString() const;
};
    
} // namespace homecare

#endif