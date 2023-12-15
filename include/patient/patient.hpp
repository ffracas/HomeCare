#ifndef PATIENT_HPP
#define PATIENT_HPP

#include <iostream>
#include <string>
#include <vector>

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
    vector<string> m_serviceCode;
    SyncType m_sync;
    int m_minWait;
    int m_maxWait;

    void setSync(SyncType, int, int);
    
public:
    Patient(string, double, double, int, int, int, vector<string>, 
            vector<string> = vector<string>(), SyncType = NoSync, int = 0, int = 0);
    ~Patient();
};

}

#endif