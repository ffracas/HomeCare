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
    ScheduleOptimiser();
    ScheduleOptimiser(const Schedule&);
    ~ScheduleOptimiser();

    // getter
    int getNPatientServices(std::string) const;
    std::vector<InfoNode> getPatientServices(std::string) const;
    SyncWindows getServiceWindows(int) const noexcept (false);
    std::pair<int, int> getSyncServiceWindow(std::string, std::string, int) const noexcept(false); 
    std::pair<std::string, InfoNode> getInterdependetInfo(std::string, std::string, int); 
    
    // update structures
    void replaceRoute(Route&, int) noexcept (false); 
    void updateSyncServiceTime(std::string, std::string, int, int); 
    void destroyNode(int, int, std::string) noexcept (false);
    int repairNode(int, const Patient&, bool = false) noexcept (false);
    
    void updateMapOfPatient(int);
    void destroyPatientRef(std::string);

    // checker
    bool isNodeIndexValid(int, int) const;
};

}

#endif