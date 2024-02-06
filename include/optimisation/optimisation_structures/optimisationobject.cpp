#include "optimisationobject.hpp"

using namespace std;
using namespace homecare;

const int OptimisationObject::NOT_ASSIGED = -1;

OptimisationObject::OptimisationObject(HCData t_data, vector<Route> t_routes, double t_cost) 
        : m_data(t_data), m_mapOfPatient(), m_operation(), m_nodeToRelocate(), m_solutionsDump() {
    m_solutionsDump.push_back(t_routes);
    m_actual = m_solutionsDump.size() - 1;
    m_solutionsRank[t_cost][makeHash(t_routes)] = m_actual;
    for (int i = 0; i < t_routes.size(); ++i) {
        for (int j = 0; j < t_routes[i].getNumNodes(); ++j) {
            Node n = t_routes[i].getNode(j);
            pair<int, Patient> p = m_data.getPatientAndIndex(n.getId());
            m_mapOfPatient[p.second.getID()][n.getService()] = InfoNode(i, j, n.getArrivalTime(), p.first);
        }
    }
}

OptimisationObject::~OptimisationObject() {}

string OptimisationObject::makeHash(vector<Route> t_routes) const {
    stringstream hash;
    for (Route const & route : t_routes) { hash << route.getHash(); }
    return hash.str();
}

int OptimisationObject::destroy(int t_route, int t_node) {
    m_operation[t_route].getNode(t_node);
    //riformulare la distruzione
}

void OptimisationObject::randomRemoval(int t_elementsToDestroy) {
    for (int i = 0; i < t_elementsToDestroy; ++i) {
        int n_route = rand() % m_operation.size();
        destroy(n_route, rand() % m_operation[n_route].getNumNodes());
    }
}

void OptimisationObject::worseRemoval(int t_elementsToDestroy) {
    for (int i = 0; i < t_elementsToDestroy; ++i) {
        map<int, string, greater<int> > worstList;
    }
}

void OptimisationObject::relatedRemoval(int t_elementsToDestroy) {}

void OptimisationObject::clusterRemoval(int t_elementsToDestroy) {}