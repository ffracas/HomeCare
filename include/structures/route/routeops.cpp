#include "routeops.hpp"

using namespace std;
using namespace homecare;

const int RouteOps::BASE_ROUTE_LEN = 2;

vector<Node> RouteOps::rescheduleRoute(std::vector<Node>& route) {
    cout<<"rescheduling route..."<<endl;
    vector<Node> newRoute;
    if (route.empty()) { return newRoute; }
    // set depot
    newRoute.push_back(route[0]);
    // set the rest of the nodes
    vector<Node> nodes (route.begin() + 1, route.end());
    // schedule the route
    return scheduleRoute(nodes);
}

//todo: questo deve essere incluso in scheduleRoute(lista, nodo)
vector<Node> RouteOps::scheduleRoute(std::vector<Node>& nodes) {
    cout<<"scheduling depot..."<<endl;
    if (nodes.size() <= BASE_ROUTE_LEN) { return nodes; }
    Node depot(nodes[0]);
    vector<Node> newRoute = scheduleRoute(excludeNode(nodes, depot), depot).getNodes();
    int distance = HCData::getDistance(depot.getDistancesIndex(), newRoute[0].getDistancesIndex());
    newRoute.insert(newRoute.begin(), depot);
    distance = HCData::getDistance(newRoute.back().getDistancesIndex(), newRoute[0].getDistancesIndex());
    newRoute[0].setArrivalTime(newRoute.back().getDeparturTime() + distance);    
    return newRoute;
}

DataRoute RouteOps::scheduleRoute(vector<Node> nodes, Node current) {
    cout<<"scheduling route..."<<nodes.size()<<endl;
    DataRoute best;
    int startTime = current.getDeparturTime();
    // empty list is a problem
    if (nodes.empty()) { return best; }
    cout<<"current distance index "<<current.getDistancesIndex()<<endl;
    // check the feasibility of the route
    if (!validityControl(nodes, startTime, current.getDistancesIndex())) { return best; }
    // base case last node to insert into the route
    cout<<"inizia analisi..."<<endl;
    if (nodes.size() == 1) {
        return DataRoute(nodes[0], 
                        current.getDeparturTime(), 
                        HCData::getDistance(current.getDistancesIndex(), nodes[0].getDistancesIndex()));
    }
    // find the best path continuation
    for (Node& node : nodes) {
        int distance = HCData::getDistance(current.getDistancesIndex(), node.getDistancesIndex());
        Node next (node);
        next.setArrivalTime(startTime + distance);
        cout<<"next node: "<<next.getId()<<endl;
        DataRoute dataRoute (scheduleRoute(excludeNode(nodes, next), next));
        if (dataRoute.getCost() == HCData::MAX_COST) { return DataRoute(); }  //in teoria validity control controlla
        dataRoute.addNode(next, startTime, distance);
        if (dataRoute.getCost() < best.getCost()) { best = dataRoute; }
    }
    return best;
}

vector<Node> RouteOps::excludeNode(const vector<Node>& list, const Node& nodeToEsclude) {
    vector<Node> result;
    for (const Node& node : list) {
        if (node.getId() != nodeToEsclude.getId()) { result.push_back(node); }
    }
    return result;
}

bool RouteOps::validityControl(vector<Node>& nodes, int startTime, int distanceIndex) {
    cout<<"validity control..."<<nodes.size()<<endl;
    try{
    for (const Node& node : nodes) {
        int distance = HCData::getDistance(distanceIndex, node.getDistancesIndex());
        if (node.isInterdependent() && node.getTimeWindowClose() < startTime + distance) { return false; }
    }
    } catch (const std::exception e) {
        cout<<"exception: "<<e.what()<<endl;
        cout<<"current index "<<distanceIndex<<endl;
        exit(1);
    }
    return true;
}