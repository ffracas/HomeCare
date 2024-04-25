#ifndef _SCHEDULE_OPTIMIZER_HPP_
#define _SCHEDULE_OPTIMIZER_HPP_

#include <map>

#include "schedule.hpp"
#include "../syncwindows/syncwindows.hpp"
#include "../service_manager/infonode.hpp"

namespace homecare {

class ScheduleOptimiser : public Schedule {
private:
    std::map<std::string, ServiceManager> m_mapOfPatient;
public:
    ScheduleOptimiser(const Schedule&);
    ~ScheduleOptimiser();

    // getter
    int getNPatientServices(std::string) const;
    std::vector<InfoNode> getPatientServices(std::string) const;
    SyncWindows getServiceWindows(int) const noexcept (false);
    std::pair<int, int> getSyncServiceWindow(std::string, std::string, int) const noexcept(false); 
    std::pair<std::string, InfoNode> getInterdependetInfo(std::string, std::string, int); 
    
    // update structures
    int replaceRoute(Route&, int) noexcept (false);
    void destroyReferencesForPatient(std::string); 
    void updateSyncServiceTime(std::string, std::string, int, int); 
};

}

#endif