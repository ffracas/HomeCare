#include "alnsoptimisation.hpp"

using namespace std;
using namespace homecare;


ALNSOptimisation::ALNSOptimisation(Schedule t_firstSchedule, double t_cost) 
    : m_currentCost(t_cost), m_currentSol(makeHash(t_firstSchedule.getSchedule())), 
        m_nodeToRelocate(), m_ops (t_firstSchedule) {
    m_solutionsDump.emplace(m_currentSol, t_firstSchedule.getSchedule());
    m_solutionsRank.push_back(make_pair(t_cost, m_currentSol));
}


ALNSOptimisation::~ALNSOptimisation() {}

ALNSOptimisation& ALNSOptimisation::getInstance(Schedule& firstSolution, double cost) {
    static ALNSOptimisation instance(firstSolution, cost);
    return instance;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////         DESTROY

ScheduleOptimiser ALNSOptimisation::destroy(ScheduleOptimiser routes, int n_route, int n_node) {
    if (!routes.isNodeIndexValid(n_route, n_node)) { return ScheduleOptimiser(); }
    // save a copy of the node to delete
    Node deletedOne (routes.getNodeFromRoute(n_route, n_node));
    // delete the node
    if (!routes.destroyNode(n_route, n_node)) { throw runtime_error("[ALNS OPT] Error: Something in destroying node"); }
    if (deletedOne.isInterdependent()) {
        InfoNode otherNode = routes.getInterdependetInfo(deletedOne.getId(), deletedOne.getService(), n_route).second;
        routes.destroyNode(otherNode.getRoute(), otherNode.getPositionInRoute());
    }
    return routes;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////        REPAIR 1

/*ScheduleOptimiser ALNSOptimisation::repairSingle(ScheduleOptimiser routes, Patient patient, int n_route) {
    Node n1(patient, 0);
    // fixme: sostituisci con nuovo metodo statico
    vector<Route> routes(routes.getRoutes());
    tuple<Node, vector<Node>, vector<Node>> data(routes[n_route].addNodeInRoute(patient, routes, n_route)); 
    // todo: static route adding
    Node node(std::get<0>(data));
    vector<Node> first (std::get<1>(data));
    node.setArrivalTime(first.back().getDeparturTime() 
                        + HCData::getDistance(first.back().getDistancesIndex(), node.getDistancesIndex()));
    first.push_back(node);
    vector<Node> second (std::get<2>(data));   
    vector<Node> completed (Route::mergeLists(first, second, routes, n_route)); 
    routes[n_route].updateRoute(completed);
    ScheduleOptimiser repaired(routes);
    return repaired;
    return routes;
}*/

ScheduleOptimiser ALNSOptimisation::repairSingle(ScheduleOptimiser routes, Patient patient, int n_route) {
    /*if (n_route >= 0 && n_route < routes.getNumberOfRoutes()) { 
        throw out_of_range("[ALNSOptimisation] repairSingle, route selected is out of range"); 
    }
     // Node's arrive time is not really important right now
    Node n1(patient, routes.getRoute(n_route).getFreeTime()); 
    // TODO: cambiare sti metodi
    routes.updateRoute() routes[n_route].addNodeInRoute(n1, n_route);*/
    return routes;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////        REPAIR 2
// TODO: riscrivere sta merda

ScheduleOptimiser ALNSOptimisation::repairDouble(ScheduleOptimiser routes, Patient patient, 
                                            int first_route, int second_route) {
    /*Node n1(patient, 0);
    vector<Route> routes(routes.getRoutes());
    tuple<Node, vector<Node>, vector<Node>> data1(routes[first_route]
                                                .addNodeInRoute(patient, routes, first_route));
    Node node1(std::get<0>(data1));
    vector<Node> *first1  = &std::get<1>(data1);
    vector<Node> *second1 = &std::get<2>(data1);
    tuple<Node, vector<Node>, vector<Node>> data2(routes[second_route]
                                        .addNodeInRoute(patient.getPatientAndNextService(node1.getArrivalTime()),       
                                                                                    routes, second_route));
    Node node2(std::get<0>(data2));
    vector<Node> *first2  = &std::get<1>(data2);
    vector<Node> *second2 = &std::get<2>(data2);
    // aggiusta se non va bene
    if (node2.getArrivalTime() < node1.getArrivalTime() + patient.getMinWait()) {
        node2.setArrivalTime(node1.getArrivalTime() + patient.getMinWait());
    }
    else if (node2.getArrivalTime() > node1.getArrivalTime() + patient.getMaxWait()) {
        node1.setArrivalTime(node2.getArrivalTime() - patient.getMaxWait());
    }
    //inserisci i dati
    first1 -> push_back(node1);
    first2 -> push_back(node2);
    vector<Node> route1(Route::mergeLists(*first1, *second1, routes, first_route));
    vector<Node> route2(Route::mergeLists(*first2, *second2, routes, second_route));
    routes[first_route].updateRoute(route1);
    routes[second_route].updateRoute(route2);
    
    ScheduleOptimiser repaired(routes);
    return repaired;*/
    return routes;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////      SAVE

void ALNSOptimisation::saveRepair(ScheduleOptimiser& repaired) {
    /*double cost = calculateCost(repaired.getRoutes());
    string hash = ALNSOptimisation::makeHash(repaired.getRoutes());
    //salva soluzione
    m_solutionsDump.emplace(hash, repaired.getRoutes());
    //classifica soluzioni
    m_solutionsRank.push_back(make_pair(cost, hash));
    sort(m_solutionsRank.begin(), m_solutionsRank.end(), 
            [] (pair<double, string> e1, pair<double, string> e2) { return e1.first < e2.first; });
    m_currentSol = m_solutionsRank[0].second;
    m_currentCost = m_solutionsRank[0].first;
    resetOperation();*/
}

void ALNSOptimisation::saveDestruction(ScheduleOptimiser& destructed, int n_route, int n_node) {
    m_nodeToRelocate.push_back(m_ops.getNodeFromRoute(n_route, n_node).getId());
    m_ops = destructed;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////    COST

/**
 * Calculates the cost based on the provided routes.
 * The cost is calculated by considering various factors such as maximum idle time, maximum tardiness,
 * total tardiness, total waiting time, travel time, and total extra time of the routes.
 * 
 * @param t_routes The routes for which the cost is to be calculated.
 * @return The calculated cost.
 */
double ALNSOptimisation::calculateCost(const vector<Route>& t_routes) {
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

    return HCData::MAX_WAIT_TIME_WEIGHT * maxIdleTime + HCData::MAX_TARDINESS_WEIGHT * maxTardiness 
        + HCData::TARDINESS_WEIGHT * totalTardiness + HCData::TOT_WAITING_TIME_WEIGHT * totalWaitingTime  
        + HCData::EXTRA_TIME_WEIGHT * totalExtraTime + HCData::TRAVEL_TIME_WEIGHT * travelTime;
}

//////////////////////////////////////////////////////////////////////////////////////// LIST NODE TO REPAIR
/**
 * Checks if there are nodes to repair.
 * 
 * @return True if there are nodes to repair, false otherwise.
 */
bool ALNSOptimisation::hasNodeToRepair() { return !m_nodeToRelocate.empty(); }

string ALNSOptimisation::popNodeToRepair() {
    if (!m_nodeToRelocate.empty()) {
        string node(m_nodeToRelocate[0]);
        m_nodeToRelocate.erase(m_nodeToRelocate.begin());
        return node;
    } 
    throw runtime_error("Node to relocate is empty");
}

////////////////////////////////////////////////////////////////////////////////////////// OTHER UTILITY

ScheduleOptimiser ALNSOptimisation::getCurrentSchedule() { return m_ops; }

ScheduleOptimiser ALNSOptimisation::getBestSol() { 
    string bestHash = m_solutionsRank[0].second;
    ScheduleOptimiser routes(m_solutionsDump[bestHash]);
    return routes; 
}

double ALNSOptimisation::getCurrentCost() { return m_currentCost; }

/**
 * Generates a random double between 0 and 0.999.
 * Uses the Mersenne Twister pseudo-random number generator.
 * 
 * @return A random double between 0 and 0.999.
 */
double ALNSOptimisation::generateRandom() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 0.999);

    // Genera un numero casuale tra 0 e 1
    return dis(gen);
}

void ALNSOptimisation::resetOperation() {
    ScheduleOptimiser routes(m_solutionsDump[m_currentSol]);
    m_ops = routes;
}

string ALNSOptimisation::makeHash(vector<Route> t_routes) {
    stringstream hash;
    for (Route const & route : t_routes) { hash << route.getHash(); }
    return hash.str();
}

