#include "randomremoval.hpp"

using namespace std;
using namespace homecare;

RandomRemoval::RandomRemoval(ALNSOptimisation& t_ops) 
        : NodeRemoval(t_ops) {}

RandomRemoval::~RandomRemoval() {}

void RandomRemoval::removeNodes(int t_elementsToDestroy) {
    //potrebbe servire reset lista percorsi operazionale
    int i = 0;
    int n_route = chooseRandomRoute();
    while (n_route != NO_INDEX) {
        int pos = chooseRandomNode(n_route);
        if (pos != NO_INDEX && n_route != NO_INDEX) { 
            if (m_removalOps.destroy(n_route, pos) > 0) {
                i ++;
            }
        }
        if (i < t_elementsToDestroy) { n_route = chooseRandomRoute(); }
        else { n_route = NO_INDEX; }
    }
}