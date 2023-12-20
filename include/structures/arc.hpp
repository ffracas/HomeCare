#ifndef ARC_HPP
#define ARC_HPP

#include "../patient/patient.hpp"

using namespace std;

namespace homecare {

class Arc
{
private:
    Patient m_departure;
    Patient m_arrival;
    int m_departureTime;
    int m_arrivalTime;
    int m_distance;
    int m_readyTime;

public:
    Arc(Patient, Patient, int, int, int);
    ~Arc();
    //getter
    Patient getArrival() const;
    Patient getDeparture() const;
    int getDistance() const;
    int getDeparturTime() const;
    int getArrvalTime() const;
    int getReadyTime() const;
};

}

#endif 