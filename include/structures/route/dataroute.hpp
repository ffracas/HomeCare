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
    double m_cost;

    void calculateCost(int, int, const SyncWindows&);
public:
    DataRoute();
    DataRoute(Node, int, int, const SyncWindows&);
    DataRoute(Node);
    ~DataRoute();

    void addNode(Node, int, int, const SyncWindows&);

    std::vector<Node> getNodes();
    double getCost();
    int getFirstDistanceIndex();
};


}

#endif