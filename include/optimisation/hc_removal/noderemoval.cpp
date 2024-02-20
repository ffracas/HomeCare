#include "noderemoval.hpp"

using namespace std;
using namespace homecare;

NodeRemoval::NodeRemoval(ALNSOptimisation& t_ops) : m_removalOps (t_ops) {}

int NodeRemoval::chooseRandomRoute() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, m_removalOps.getNumberOfRoutes() - 1);
    return dis(gen);
}

int NodeRemoval::chooseRandomNode(int n_route) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, m_removalOps.getNumberOfNodesInRoute(n_route) - 2);
    return dis(gen) + 1;
}