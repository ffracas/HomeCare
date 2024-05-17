#ifndef _ROUTE_OPS_HPP_
#define _ROUTE_OPS_HPP_

#include <vector>
#include <map>
#include <queue>

#include "dataroute.hpp"
#include "../node.hpp"
#include "../../readjson/hcdata.hpp"
#include "../../structures/syncwindows/syncwindows.hpp"

namespace homecare {

class RouteOps {
    public:
        static const int BASE_ROUTE_LEN;
        static std::vector<Node> rescheduleRoute(std::vector<Node>&, const SyncWindows&);
        static std::vector<Node> scheduleRoute(std::vector<Node>&, const SyncWindows&);
        static DataRoute scheduleRoute(std::vector<Node>, const SyncWindows&, Node, int, int);
        static std::vector<Node> excludeNode(const std::vector<Node>&, const Node&);
        static bool validityControl(const std::vector<Node>&, int, int, const SyncWindows&);
};

    
}

#endif