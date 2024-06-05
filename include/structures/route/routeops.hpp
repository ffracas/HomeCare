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
        static const int BASE_ROUTE_LEN = 2;
        static const int MAX_ROUTE_LEN_DINAMIC_P = 7;
        static std::vector<Node> rescheduleRoute(std::vector<Node>&, const SyncWindows&);
        static DataRoute dinamicScheduling(DataRoute, std::vector<Node>&, int, int, const SyncWindows&);
        static DataRoute twoOpt(DataRoute, const SyncWindows&);
};

    
}

#endif