#ifndef _ROUTE_OPS_HPP_
#define _ROUTE_OPS_HPP_

#include <vector>
#include <map>
#include <queue>

#include "../node.hpp"
#include "../../readjson/hcdata.hpp"
#include "../../structures/syncwindows/syncwindows.hpp"

namespace homecare {

class RouteOps {

public:
    static const double MAX_NODE_COST;
    static std::vector<Node> rescheduleRoute(const std::vector<Node>&, const SyncWindows&);
    static std::vector<Node> scheduleRoute(std::vector<Node>, std::vector<Node>, 
                                            std::map<std::string,std::pair<int,int>>&);
    static double calculateCost(const Node&, Node, std::map<std::string,std::pair<int,int>>&);
    static std::vector<Node> escludeNode(const std::vector<Node>&, const Node&);

};

    
}

#endif