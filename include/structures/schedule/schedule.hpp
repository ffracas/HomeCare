#ifndef SCHEDULE_HPP
#define SCHEDULE_HPP

#include <vector>
#include <map>
#include <sstream>

#include "../../readjson/hcdata.hpp"
#include "../route/route.hpp"
#include "../service_manager/servicemanager.hpp"

namespace homecare {

class Schedule
{
private:
    std::vector<Route> m_routes;
    std::map<std::string, ServiceManager> m_mapOfPatient;

    // utils
    double calculateSupposedCost(int, Node) const;
    int getSupposedArrivalTime(int, int, int) const;
    int searchForRoute(Patient, int) const;

public:
    Schedule(const std::vector<Caregiver>&);
    ~Schedule();

    // getter
    std::vector<Route> getSchedule() const;
    double getCost() const;
    int getNumberOfRoutes() const;

    // add nodes
    int greedyAppend(Patient);
    int insertNode(int, Patient);

};

}

#endif