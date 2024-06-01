#ifndef DATA_ROUTEOPS_HPP
#define DATA_ROUTEOPS_HPP

#include <vector>

#include "../node.hpp"
#include "../syncwindows/syncwindows.hpp"
#include "../../readjson/hcdata.hpp"

namespace homecare {

class DataRoute {
private:
    std::vector<Node> m_nodes;
    double m_cost = 0;
    int m_maxTardiness = 0;
    int m_maxIdleTime = 0;
    int m_totalTardiness = 0;
    int m_totalWaitingTime = 0;
    int m_travelTime = 0;
    int m_extraTime = 0;
    bool m_completed = false;

    void resetCost();
    void addNode(Node&, const SyncWindows&, bool = false);

public:
    DataRoute();
    DataRoute(Node);
    DataRoute(std::vector<Node>&, const SyncWindows&);
    ~DataRoute();

    void addNode(Node, int = -1, int = -1, bool = false);
    void twoOptSwap(int, int, const SyncWindows&);

    std::vector<Node> getNodes();
    double getCost();
};


}

#endif