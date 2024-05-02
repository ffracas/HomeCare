#ifndef SCHEDULE_HPP
#define SCHEDULE_HPP

#include <vector>
#include <map>
#include <sstream>
#include <optional>

#include "../route/route.hpp"
#include "../service_manager/servicemanager.hpp"
#include "../../readjson/hcdata.hpp"

namespace homecare {

class Schedule {
private:
    std::vector<Route> m_routes;
    
    // utils
    double calculateSupposedCost(int, Node) const;
    int getSupposedArrivalTime(int, int, int) const;
    int searchForRoute(Patient, int) const;

public:
    Schedule();
    Schedule(const std::vector<Caregiver>&);
    ~Schedule();

    // getter
    std::vector<Route> getSchedule() const;
    Route getRoute(int) const noexcept (false);
    Node getNodeFromRoute(int, int) const noexcept (false);
    std::string getCaregiverOfRoute(int) const noexcept (false);

    // get Data
    double getCost() const;
    int getNumberOfRoutes() const;
    int getNumberOfNodesInRoute(int) const noexcept (false);

    // checker
    bool isServiceAvailableInRoute(std::string, int) const noexcept (false); 
    bool isIndexValid(int) const;
    bool isEmpty() const;

    // add nodes
    int greedyAppend(Patient);

    // update routes
    void destroyNode(int, int, const SyncWindows&) noexcept (false);
    void repairNode(int, Node, const SyncWindows&) noexcept (false);
    void replaceRoute(Route&, int) noexcept (false);
    void updateNodeTime(int, int, int) noexcept (false);
};

}

#endif