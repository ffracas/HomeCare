#include "randomremoval.hpp"

using namespace std;
using namespace homecare;

RandomRemoval::RandomRemoval() : NodeRemoval() {}

RandomRemoval::~RandomRemoval() {}

void RandomRemoval::removeNodes(int t_elementsToDestroy) {
    //potrebbe servire reset lista percorsi operazionali
    int i = 0;
    ScheduleOptimiser routes(m_data->getCurrentSchedule());
    int n_route = chooseRandomRoute(routes);
    while (n_route != NO_INDEX) {
        int pos = chooseRandomNode(routes, n_route);
        if (pos != NO_INDEX && n_route != NO_INDEX) {

            ScheduleOptimiser newRoutes(m_data->destroy(routes, n_route, pos));
            if (!newRoutes.isEmpty()) {
                i ++;
                m_data->saveDestruction(newRoutes, n_route, pos);
                routes = newRoutes;
            }
        }
        if (i < t_elementsToDestroy) { n_route = chooseRandomRoute(routes); }
        else { n_route = NO_INDEX; }
    }
}