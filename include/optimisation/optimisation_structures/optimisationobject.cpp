#include "optimisationobject.hpp"

using namespace std;
using namespace homecare;

OptimisationObject::OptimisationObject(HCData t_data, vector<Route> t_routes, double t_cost) 
        : m_data(t_data), m_actualCost (t_cost), m_operation (t_routes), m_nodeToRelocate (), 
        p_worst (5), 
        p_related (5), p_distanceWeight (2), p_windowWeight (3), p_serviceWeight (1), //TODO settaggio parametri
        m_mapOfPatient (), m_solutionsDump() {
    string solHash = makeHash(t_routes);
    m_actualSol = solHash;
    m_solutionsDump.emplace(solHash, t_routes);
    m_solutionsRank.push_back(pair<double, string> (t_cost, solHash));
    for (int i = 0; i < t_routes.size(); ++i) {
        for (int j = 1; j < t_routes[i].getNumNodes(); ++j) {
            Node n = t_routes[i].getNodeToDestroy(j);
            int pos = m_data.getPatientPosition(n.getId());
            m_mapOfPatient[n.getId()].emplace(n.getService(), InfoNode(i, j, n.getArrivalTime(), pos));
        }
    }
}

OptimisationObject::~OptimisationObject() {}

void OptimisationObject::resetOperation() {
    // Pulisce il vettore m_operation
    m_operation.clear();

    // Copia i valori da m_solutionsDump[m_actualSol] a m_operation
    m_operation.reserve(m_solutionsDump[m_actualSol].size());
    for (const auto& route : m_solutionsDump[m_actualSol]) {
        m_operation.push_back(route);
    }

    // Pulisce il vettore dei nodi da ricollocare
    m_nodeToRelocate.clear();
}

string OptimisationObject::makeHash(vector<Route> t_routes) const {
    stringstream hash;
    for (Route const & route : t_routes) { hash << route.getHash(); }
    return hash.str();
}

int OptimisationObject::destroy(int n_route, int n_node, vector<Route>& t_routes, bool onOperational) {
    if (n_node == 0) { n_node++; }
    string patient = t_routes[n_route].getNodeToDestroy(n_node).getId();
    m_nodeToRelocate.push_back(patient);
    for (auto& service : m_mapOfPatient[patient]) {
        //eliminare dalle route
        n_route = service.second.getRoute();
        if(onOperational) {
            updateMapOfPatient(t_routes[n_route].deleteNode(service.second.getPositionInRoute(),
                                m_data.getDistances()), n_route);
        }
        //eliminare riferimento
        service.second.destroy();
    }
    return m_mapOfPatient[patient].size();
}

void OptimisationObject::updateMapOfPatient(Route route, int n_route) {
    for (int j = 1; j < route.getNumNodes(); ++j) {
        const Node& n = route.getNodeToDestroy(j);
        InfoNode& infoNode = m_mapOfPatient[n.getId()][n.getService()];
        infoNode.setInRoute(n_route, j, n.getArrivalTime());
    }
}

double OptimisationObject::calculateCost(vector<Route> t_routes) {
    int maxIdleTime       = 0;
    int maxTardiness      = 0;
    int totalTardiness    = 0;
    int totalWaitingTime  = 0;
    int travelTime        = 0;
    int totalExtraTime    = 0;

    for (Route route : t_routes) {
        maxIdleTime = route.getMaxIdleTime() > maxIdleTime ? route.getMaxIdleTime() : maxIdleTime;
        maxTardiness = route.getMaxTardiness() > maxTardiness ? route.getMaxTardiness() : maxTardiness;
        totalTardiness += route.getTotalTardiness();
        totalWaitingTime += route.getTotalWaitingTime();
        travelTime += route.getTravelTime();
        totalExtraTime += route.getExtraTime();
    }

    return m_data.MAX_IDLE_TIME_WEIGHT * maxIdleTime + m_data.MAX_TARDINESS_WEIGHT * maxTardiness 
        + m_data.TARDINESS_WEIGHT * totalTardiness + m_data.TOT_WAITING_TIME_WEIGHT * totalWaitingTime  
        + m_data.EXTRA_TIME_WEIGHT * totalExtraTime + m_data.TRAVEL_TIME_WEIGHT * travelTime;
}

double OptimisationObject::generateRandom() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 0.999);

    // Genera un numero casuale tra 0 e 1
    return dis(gen);
}

double OptimisationObject::calculateSimilarity(int distance, int difOpenWin, int difCloseWin, bool service, 
            int maxDist, int difTWO, int difTWC) {
    double s = service ? p_serviceWeight : 0;
    return (p_distanceWeight * distance) / maxDist 
            + p_windowWeight * (double(difOpenWin) / difTWO + double(difCloseWin) / difTWC)
            + s;
}

void OptimisationObject::findMinMaxRelatetion(int& maxDist, int& minTWO, 
        int& maxTWO, int& minTWC, int& maxTWC, int n_route, int seedDistIndex) {
    for (int i = 0; i < m_operation[n_route].getNumNodes(); ++i) {
        Node n = m_operation[n_route].getNodeToDestroy(i);
        //controllo distanze
        int distance = m_data.getDistance(seedDistIndex, n.getDistancesIndex());
        maxDist = distance > maxDist ? distance : maxDist;

        //controllo finestra aperta
        if (n.getTimeWindowOpen() < minTWO) { minTWO = n.getTimeWindowOpen(); }
        else if (n.getTimeWindowOpen() > maxTWO) { maxTWO = n.getTimeWindowOpen(); }

        //controllo finestra chiusa
        if (n.getTimeWindowClose() < minTWC) { minTWC = n.getTimeWindowClose(); }
        else if (n.getTimeWindowClose() > maxTWC) { maxTWC = n.getTimeWindowClose(); }
    }
}

pair<int, int> OptimisationObject::getRandomNode(vector<pair<int, int>> list) {
    if (list.size() <  1) { throw runtime_error("error in get random node"); }
    if (list.size() == 1) { return list[0]; }
    int pos = rand() % list.size();
    return list[pos];
}

void OptimisationObject::randomRemoval(int t_elementsToDestroy) {
    resetOperation();
    int i = 0;
    while (i < t_elementsToDestroy) {
        int n_route = rand() % m_operation.size();
        i += destroy(n_route, rand() % m_operation[n_route].getNumNodes(), m_operation);
    }
}

void OptimisationObject::worseRemoval(int t_elementsToDestroy) {
    resetOperation();
    int n_des = 0;
    while (n_des < t_elementsToDestroy) {
        vector<CostCoord> worstList;
        for (int i = 0; i < m_operation.size(); ++i) {
            for (int j = 1; j < m_operation[i].getNumNodes(); ++j) {
                vector<Route> solCopy(m_operation);
                destroy(i, j, solCopy);
                double dif = calculateCost(solCopy) - m_actualCost;
                worstList.push_back(CostCoord({dif, i, j}));
                sort(worstList.begin(), worstList.end(), [] (CostCoord cc1, CostCoord cc2) { 
                                                                return cc1.cost > cc2.cost;
                                                            });
            }
        }
        int pos = floor(pow(generateRandom(), p_worst) * worstList.size());
        n_des += destroy(worstList[pos].n_route, worstList[pos].n_node, m_operation);
    }
}

void OptimisationObject::relatedRemoval(int t_elementsToDestroy) {
    resetOperation();
    int n_route = rand() % m_operation.size();
    int n_pos   = rand() % m_operation[n_route].getNumNodes();
    Node seed   = m_operation[n_route].getNodeToDestroy(n_pos);
    int maxDist = 0;                                                    //min Distance fissato a 0 (nodo stesso)
    int minTWO  = seed.getTimeWindowOpen() , maxTWO = minTWO;
    int minTWC  = seed.getTimeWindowClose(), maxTWC = minTWC;
    findMinMaxRelatetion(maxDist, minTWO, maxTWO, minTWC, maxTWC, n_route, seed.getDistancesIndex());

    vector<CostCoord> similarityRank;
    vector<pair<int, int>> nodesToRemove;
    nodesToRemove.push_back({n_route, n_pos});
    while (nodesToRemove.size() < t_elementsToDestroy) {
        similarityRank.clear();
        pair<int, int> coord = getRandomNode(nodesToRemove);
        seed = m_operation[coord.first].getNodeToDestroy(coord.second);
        for (int i = 0; i < m_operation.size(); ++i) {
            for (int j = 1; j < m_operation[i].getNumNodes(); ++j) {
                if (i != n_route && j != n_pos) {
                    Node p = m_operation[i].getNodeToDestroy(j);
                    int distance    = m_data.getDistance(seed.getDistancesIndex(), p.getDistancesIndex());
                    int difOpenWin  = abs(p.getTimeWindowOpen() - seed.getTimeWindowOpen());
                    int difCloseWin = abs(p.getTimeWindowClose()   - seed.getTimeWindowClose()); 
                    double similarity = calculateSimilarity(distance, difOpenWin, difCloseWin, 
                            seed.getService() == p.getService(), maxDist, minTWO - maxTWO, minTWC - maxTWC);
                    similarityRank.push_back(CostCoord({similarity, i, j}));
                    sort(similarityRank.begin(), similarityRank.end(), [] (CostCoord cc1, CostCoord cc2) { 
                                                                    return cc1.cost < cc2.cost;
                                                                });
                }
            }
        }
        int pos = floor(pow(generateRandom(), p_related) * similarityRank.size());
        nodesToRemove.push_back({similarityRank[pos].n_route, similarityRank[pos].n_node});
    }
    for (const pair<int, int>& coord : nodesToRemove) {
        destroy(coord.first, coord.second, m_operation);
    }
}

void OptimisationObject::clusterRemoval(int t_elementsToDestroy) {}