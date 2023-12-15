#include "patient.hpp"

using namespace homecare;

Patient::Patient(string t_id, double t_x, double t_y, 
            int t_timeWindowOpen, int t_timeWindowClose, int t_distanceIndex, vector<string> t_serviceCode, 
            vector<string> t_invalidCaregiver, SyncType t_sync, int t_minWait, int t_maxWait) 
        : m_id(t_id), m_x(t_x), m_y(t_y), m_timeWindowOpen(t_timeWindowOpen), m_timeWindowClose(t_timeWindowClose) {
    switch (t_sync)  {
        case Sequential: 
            setSync(t_sync, t_minWait, t_maxWait);
            break;
        case Simultaneous:
            setSync(t_sync, 0, 0);
            break;
        default:
            setSync(NoSync, 0, 0);
    }     
}   

Patient::~Patient() {}

void Patient::setSync(SyncType t_sync, int t_minWait, int t_maxWait) {
    m_sync = t_sync;
    m_minWait = t_minWait;
    m_maxWait = t_maxWait;
}