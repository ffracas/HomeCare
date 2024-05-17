#include "routeops.hpp"

using namespace std;
using namespace homecare;

const int RouteOps::BASE_ROUTE_LEN = 2;

vector<Node> RouteOps::rescheduleRoute(std::vector<Node>& route, const SyncWindows& syncWindows) {
    if (route.empty()) { 
        throw runtime_error("[Route]: Error: list to schedule is empty");
    }
    cout<<"\nnot empty";
    // schedule the route
    if (route.size() <= BASE_ROUTE_LEN) { return route; }
    cout<<"+2\n";
    return scheduleRoute(route, syncWindows);
}

//todo: questo deve essere incluso in scheduleRoute(lista, nodo)
//fixme: aggiungere calcolo costo last-depot
vector<Node> RouteOps::scheduleRoute(std::vector<Node>& nodes, const SyncWindows& syncWindows) {
    cout<<"scheduling depot..."<<endl;
    if (nodes.size() <= BASE_ROUTE_LEN) { return nodes; }
    Node depot(nodes[0]);
    vector<Node> newRoute = scheduleRoute(nodes, syncWindows, depot, depot.getDeparturTime(), 0).getNodes();
    if (newRoute.empty()) { 
        return newRoute;
    }
    int distance = HCData::getDistance(newRoute.back().getDistancesIndex(), newRoute[0].getDistancesIndex());
    newRoute[0].setArrivalTime(newRoute.back().getDeparturTime() + distance);    
    return newRoute;
}

DataRoute RouteOps::scheduleRoute(vector<Node> nodes, const SyncWindows& syncWindows, 
                                    Node current, int precDeparture, int precDistance) {
    if (nodes.empty()) { return DataRoute(); }
    if (nodes.size() == 1) { return DataRoute(current, precDeparture, precDistance, syncWindows); }
    cout<<"current:"<<current.getId()<<" size:"<<nodes.size()<<endl;
    nodes = excludeNode(nodes, current);
    if (!validityControl(nodes, current.getDeparturTime(), current.getDistancesIndex(), syncWindows)) { 
        return DataRoute(); 
    }
    DataRoute best;
    for (Node& node : nodes) {
        int distance = HCData::getDistance(current.getDistancesIndex(), node.getDistancesIndex());
        node.setArrivalTime(current.getDeparturTime() + distance);
        DataRoute subRoute = scheduleRoute(nodes, syncWindows, node, current.getDeparturTime(), distance);
        // if the result is valid, check if it improves the best
        if (subRoute.getCost() != HCData::MAX_COST) { 
            subRoute.addNode(current, precDeparture, precDistance, syncWindows);
            if (subRoute.getCost() < best.getCost()) { best = subRoute; }
        }
    }
    return best;
}

// usare find_if e erase
vector<Node> RouteOps::excludeNode(const vector<Node>& list, const Node& nodeToEsclude) {
    vector<Node> result;
    for (const Node& node : list) {
        if (node.getId() != nodeToEsclude.getId()) { result.push_back(node); }
    }
    return result;
}

bool RouteOps::validityControl(const vector<Node>& nodes, int departureTime, int distanceIndex, const SyncWindows& sw) {
    for (const Node& node : nodes) {
        int estimatedArrival = departureTime + HCData::getDistance(distanceIndex, node.getDistancesIndex());
        if (node.isInterdependent()) if (sw.getSyncWindow(node.getId()).second < estimatedArrival) { return false; }
    }
    return true;
}
