#ifndef DATA_ROUTEOPS_HPP
#define DATA_ROUTEOPS_HPP

#include <vector>

#include "../node.hpp"
#include "../../readjson/hcdata.hpp"

namespace homecare {

class DataRoute {
private:
    std::vector<Node> m_nodes;
    double m_cost;
    int m_maxTardiness = 0;
    int m_maxIdleTime = 0;
    int m_totalTardiness = 0;
    int m_totalWaitingTime = 0;
    int m_travelTime = 0;
    int m_extraTime = 0;
    bool m_completed = false;

public:
    DataRoute();
    DataRoute(Node);
    ~DataRoute();

    void addNode(Node, int = -1, int = -1, bool = false);

    std::vector<Node> getNodes();
    double getCost();
};


}

#endif