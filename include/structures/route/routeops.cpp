#include "routeops.hpp"

using namespace std;
using namespace homecare;

const int RouteOps::BASE_ROUTE_LEN = 2;

vector<Node> RouteOps::rescheduleRoute(vector<Node>& route, const SyncWindows& syncWindows) {
    if (route.empty()) { throw runtime_error("[Route]: Error: list to schedule is empty"); }
    // schedule the route
    // if is too short (only depot and one node scheduled at max)
    //if (route.size() <= BASE_ROUTE_LEN) { return route; }
    // else calculate schedule
    DataRoute newSchedule = scheduleRoute(DataRoute(route[0]), route, (1 << route.size()) - 1, 0, syncWindows);
    if (newSchedule.getCost() == HCData::MAX_COST) { return vector<Node>(); }  
    return newSchedule.getNodes();
}

DataRoute RouteOps::scheduleRoute(DataRoute solution, vector<Node>& nodes, int mask, int current, const SyncWindows& sw){
    // base case: only one node and return to depot to schedule
    if (mask == (1 << current)) {
        Node node = nodes[current];
        solution.addNode(node, 
                        node.isInterdependent() ? sw.getOpenSyncTime(node.getId())  : -1,
                        node.isInterdependent() ? sw.getCloseSyncTime(node.getId()) : -1,
                        true);
        return solution;
    }

    // add node to solution
    if (nodes[current].isInterdependent()) {
        solution.addNode(nodes[current], 
                        sw.getOpenSyncTime(nodes[current].getId()), 
                        sw.getCloseSyncTime(nodes[current].getId()));
    }
    else if (current != 0) { solution.addNode(nodes[current]); }
    // if is failed
    if (solution.getCost() == HCData::MAX_COST) { return solution; }
    // best solution, data to return
    DataRoute best;
    // iterate over all possible options
    for (int i = 1; i < nodes.size(); ++i) {
        if ((mask & (1 << i)) && i != current) {
            DataRoute newSolution = scheduleRoute(solution, nodes, mask & (~(1 << current)) , i, sw);
            if (newSolution.getCost() < best.getCost()) {
                best = newSolution;
            }
        }
    }
    return best;
}

//todo: questo deve essere incluso in scheduleRoute(lista, nodo)
//fixme: aggiungere calcolo costo last-depot
/*vector<Node> RouteOps::scheduleRoute(std::vector<Node>& nodes, const SyncWindows& syncWindows) {
    // scheduling
    Node depot(nodes[0]);
    vector<Node> newRoute = scheduleRoute(nodes, syncWindows, depot, depot.getDeparturTime(), 0).getNodes();
    // controllo risultato
    if (newRoute.empty()) { return newRoute; }
    int distance = HCData::getDistance(newRoute.back().getDistancesIndex(), newRoute[0].getDistancesIndex());
    newRoute[0].setArrivalTime(newRoute.back().getDeparturTime() + distance);    
    return newRoute;
}*/

/*DataRoute RouteOps::scheduleRoute(vector<Node> nodes, const SyncWindows& syncWindows, 
                                    Node current, int precDeparture, int precDistance) {
    if (nodes.empty()) { return DataRoute(); }
    if (nodes.size() == 1) { return DataRoute(current, precDeparture, precDistance, syncWindows); }
    nodes = excludeNode(nodes, current);
    if (!validityControl(nodes, current.getDeparturTime(), current.getDistancesIndex(), syncWindows)) { 
        return DataRoute(); 
    }
    DataRoute best;
    for (Node& node : nodes) {
        int distance = HCData::getDistance(current.getDistancesIndex(), node.getDistancesIndex());
        int arrivingTime = current.getDeparturTime() + distance;
        if (node.isInterdependent()) { 
            int openSyncWin  = syncWindows.getOpenSyncTime(node.getId());
            arrivingTime = arrivingTime >= openSyncWin ? arrivingTime : openSyncWin;
        }
        node.setArrivalTime(arrivingTime);
        DataRoute subRoute = scheduleRoute(nodes, syncWindows, node, current.getDeparturTime(), distance);
        // if the result is valid, check if it improves the best
        if (subRoute.getCost() != HCData::MAX_COST) { 
            subRoute.addNode(current, precDeparture, precDistance, syncWindows);
            if (subRoute.getCost() < best.getCost()) { best = subRoute; }
        }
    }
    return best;
}*/

// todo: valuta cancellazione
vector<Node> RouteOps::excludeNode(const vector<Node>& list, const Node& nodeToEsclude) {
    vector<Node> result;
    for (const Node& node : list) {
        if (node.getId() != nodeToEsclude.getId()) { result.push_back(node); }
    }
    return result;
}

// todo: valuta cancellazione
bool RouteOps::validityControl(const vector<Node>& nodes, int departureTime, int distanceIndex, const SyncWindows& sw) {
    for (const Node& node : nodes) {
        int estimatedArrival = departureTime + HCData::getDistance(distanceIndex, node.getDistancesIndex());
        if (node.isInterdependent()) if (sw.getSyncWindow(node.getId()).second < estimatedArrival) { return false; }
    }
    return true;
}
