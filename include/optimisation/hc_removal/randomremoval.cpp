#include "randomremoval.hpp"

using namespace std;
using namespace homecare;

RandomRemoval::RandomRemoval(ALNSOptimisation& t_ops) : NodeRemoval(t_ops) {}

RandomRemoval::~RandomRemoval() {}

void RandomRemoval::removeNodes(int t_elementsToDestroy) {
    //potrebbe servire reset lista percorsi operazionale
    int i = 0;
    while (i < t_elementsToDestroy) {
        int n_route = chooseRandomRoute();
        i += m_removalOps.destroy(n_route, chooseRandomNode(n_route));
    }
}