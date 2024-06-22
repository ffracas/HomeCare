#include "routeops.hpp"

using namespace std;
using namespace homecare;

vector<Node> RouteOps::rescheduleRoute(vector<Node>& route, const SyncWindows& syncWindows) {
    if (route.empty()) { throw runtime_error("[Route]: Error: list to schedule is empty"); }
    if (route.size() <= 2) { return route; } //if there are depot and at max one node, return the list
    DataRoute newSchedule;
    if (route.size() > MAX_ROUTE_LEN_DINAMIC_P) {
        newSchedule = twoOpt(DataRoute (route, syncWindows), syncWindows);
    }
    else {
        newSchedule = dinamicScheduling(DataRoute(route[0]), route, (1 << route.size()) - 1, 0, syncWindows);
    }
    if (newSchedule.getCost() == HCData::MAX_COST) { return vector<Node>(); }   //dovrebbe funzionare anche senza
    return newSchedule.getNodes();
}

DataRoute RouteOps::dinamicScheduling(DataRoute solution, vector<Node>& nodes, int mask, int current, 
                                    const SyncWindows& sw){
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
            DataRoute newSolution = dinamicScheduling(solution, nodes, mask & (~(1 << current)) , i, sw);
            if (newSolution.getCost() < best.getCost()) {
                best = newSolution;
            }
        }
    }
    return best;
}

DataRoute RouteOps::twoOpt(DataRoute best, const SyncWindows& sw) {
    bool improved = true;
    int n_nodes = best.getNodes().size();
    while (improved) {
        improved = false;
        for (int i = 1; i < n_nodes - 1; ++i) {
            for (int j = i + 1; j < n_nodes; ++j) {
                DataRoute solution = best;
                solution.twoOptSwap(i, j, sw);
                if (solution.getCost() < best.getCost()) {
                    best = solution;
                    improved = true;
                }
            }
        }
    }
    return best;
}