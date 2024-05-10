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

    void calcolateCost(int, int);
public:
    DataRoute();
    DataRoute(Node, int, int);
    ~DataRoute();

    void addNode(Node, int, int);

    std::vector<Node> getNodes();
    double getCost();
};


}

#endif