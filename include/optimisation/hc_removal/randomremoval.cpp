#include "randomremoval.hpp"

using namespace std;
using namespace homecare;

RandomRemoval::RandomRemoval() : NodeRemoval() {}

RandomRemoval::~RandomRemoval() {}

void RandomRemoval::removeNodes(int t_elementsToDestroy) {
    //potrebbe servire reset lista percorsi operazionale
    int i = 0;
    RoutesOpt routes(ALNSOptimisation::getCurrentSol());
    int n_route = chooseRandomRoute(routes);
    while (n_route != NO_INDEX) {
        int pos = chooseRandomNode(routes, n_route);
        if (pos != NO_INDEX && n_route != NO_INDEX) { 
            RoutesOpt newRoutes(ALNSOptimisation::destroy(routes, n_route, pos));
            if (!newRoutes.isEmpty()) {
                i ++;
                routes = newRoutes;
                ALNSOptimisation::saveDestruction(routes, n_route, pos);
            }
        }
        if (i < t_elementsToDestroy) { n_route = chooseRandomRoute(routes); }
        else { n_route = NO_INDEX; }
    }
}