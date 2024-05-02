#include "routeops.hpp"

using namespace std;
using namespace homecare;

const double RouteOps::MAX_NODE_COST = 99999.99;

std::vector<Node> RouteOps::rescheduleRoute(const std::vector<Node>& route, const SyncWindows& scheduleOps) {
    vector<Node> newRoute;
    if (route.empty()) {
        return newRoute;
    }
    // set depot
    newRoute.push_back(route[0]);
    // set the rest of the nodes
    vector<Node> nodes (route.begin() + 1, route.end());
    // set sync windows
    map<std::string, pair<int,int>> syncWindows;
    // schedule the route
    newRoute = scheduleRoute(newRoute, nodes, syncWindows);
    if (newRoute.empty()) {
        return newRoute;
    }
    // TODO trasferire questa operazione in routesopt
    /*if (!newRoute.empty()){
        for (const Node& node : newRoute) {
            if (node.isInterdependent()) {
                scheduleOps.updateSyncServiceTime(node.getId(), node.getService(), routeIndex, node.getArrivalTime());
            }
        }
    }*/
    return newRoute;
}

struct CompareCost {
    bool operator()(const pair<double, Node>& p1, const pair<double, Node>& p2) {
        return p1.first > p2.first;
    }
};

vector<Node> RouteOps::scheduleRoute(vector<Node> route, vector<Node> nodes, map<std::string, pair<int,int>>& syncWin) {
    priority_queue<pair<double, Node>, vector<pair<double, Node>>, CompareCost> ranking;
    // if the route is empty, we add the first node (depot) of the list to the route
    if (route.empty()) {
        route.push_back(nodes[0]);
        nodes.erase(nodes.begin());
    }
    // calculate ranking
    for (Node& node : nodes) {
        double cost = calculateCost(route.back(), node, syncWin); 
        if (cost == MAX_NODE_COST) {
            return vector<Node>();
        }
        ranking.push(make_pair(cost, node));
    }
    // explore the solution
    while (!ranking.empty()) {
        Node node = ranking.top().second;
        ranking.pop();
        route.push_back(node);
        vector<Node> newNodes = escludeNode(nodes, node);
        // if the node was the last in the list, return the route
        if (newNodes.empty()) {
            return route;
        }
        vector<Node> newRoute = scheduleRoute(route, newNodes, syncWin);
        if (!newRoute.empty()) {
            return newRoute;
        }
        route.pop_back();
    }
    // if it gets here, the path is not possible and you have to take a step back
    return vector<Node>();
}

double RouteOps::calculateCost(const Node& lastInRoute, Node currentNode, map<std::string, pair<int,int>>& syncWin) {
    int estimatedTime = lastInRoute.getDeparturTime() + 
                    HCData::getDistance(lastInRoute.getDistancesIndex(), currentNode.getDistancesIndex());
    int delta = estimatedTime < currentNode.getTimeWindowOpen() ?
                    estimatedTime - currentNode.getTimeWindowOpen() :
                    estimatedTime > currentNode.getTimeWindowClose() ?
                        estimatedTime - currentNode.getTimeWindowClose() : 0;
    estimatedTime = max(estimatedTime, currentNode.getTimeWindowOpen());
    if (syncWin.find(currentNode.getId()) != syncWin.end()) {
        if (estimatedTime > syncWin[currentNode.getId()].second) {
            return MAX_NODE_COST;
        }
        if (estimatedTime < syncWin[currentNode.getId()].first) {
            estimatedTime = syncWin[currentNode.getId()].first;
        }
    }
    double cost = (double)estimatedTime * HCData::TRAVEL_TIME_WEIGHT;
    // if delta > 0 then there is time to wait for patient. Decrease cost
    if (delta > 0) {
        cost += (double)delta * HCData::TARDINESS_WEIGHT;
    }
    // else delta < 0 then there is time to wait for the doctor. Increase cost
    else {
        cost += (double)delta * HCData::EXTRA_TIME_WEIGHT;
    }
    return cost;
}

vector<Node> RouteOps::escludeNode(const vector<Node>& list, const Node& nodeToEsclude) {
    vector<Node> result;
    for (const Node& node : list) {
        if (node.getId() != nodeToEsclude.getId()) {
            result.push_back(node);
        }
    }
    return result;
}