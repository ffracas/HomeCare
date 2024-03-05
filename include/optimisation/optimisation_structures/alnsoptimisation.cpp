#include "alnsoptimisation.hpp"

using namespace std;
using namespace homecare;

const double ALNSOptimisation::MAX_DOUBLE = 1.79769313486231570e+308;
string ALNSOptimisation::m_currentSol = "";
double ALNSOptimisation::m_currentCost = ALNSOptimisation::MAX_DOUBLE;
RoutesOpt ALNSOptimisation::m_ops;
vector<string> ALNSOptimisation::m_nodeToRelocate;
std::vector<std::pair<double, std::string>> ALNSOptimisation::m_solutionsRank; 
std::map<std::string, std::vector<Route>> ALNSOptimisation::m_solutionsDump;

ALNSOptimisation::~ALNSOptimisation() {}

void ALNSOptimisation::setISol(vector<Route> routes, double cost) {
    m_currentCost = cost;
    string solHash = makeHash(routes);
    m_currentSol = solHash;
    //fill dump
    m_solutionsDump.emplace(solHash, routes);
    //init rank
    m_solutionsRank.push_back(make_pair(cost, solHash));
    //init list and map
    RoutesOpt Isol(routes);
    m_ops = Isol;
}

int ALNSOptimisation::getNumberOfRoutes() { return m_ops.getNumberOfRoutes(); }

///////////////////////////////////////////////////////////////////////////////////////////////////////         DESTROY

RoutesOpt ALNSOptimisation::destroy(RoutesOpt routes, int n_route, int pos_node) {
    try {
        if (routes.getNumberOfRoutes() <= n_route || n_route < 0) { 
            return RoutesOpt(); 
        }
        if (pos_node >= routes.getNumberOfNodesInRoute(n_route) || pos_node < 1) { 
            return RoutesOpt(); 
        }
        // save a copy of the node
        Node deletedOne (routes.getNodeInRoute(n_route, pos_node));
        // delete the node
        Route newRoute (routes.getRoute(n_route).deleteNode(pos_node, routes, n_route));
        routes = routes.replaceRoute(newRoute, n_route);
        // search for interdependet node
        if (deletedOne.isInterdependent()) {
            InfoNode otherNode = routes.getInterdependetInfo(deletedOne.getId(), deletedOne.getService(), n_route)
                                        .second;
            Route interdepRoute (routes.getRoutes()[otherNode.getRoute()]
                                        .deleteNode(otherNode.getPositionInRoute(), routes, otherNode.getRoute()));
            routes = routes.replaceRoute(interdepRoute, otherNode.getRoute());
        }
        // return result
        return routes;
    }
    catch (exception e) {
        cerr << "Error in destroy node: " << e.what() << '\n';
        exit(1); // Termina l'esecuzione del programma
    }
    return RoutesOpt();
}


///////////////////////////////////////////////////////////////////////////////////////////////////////         REPAIR

RoutesOpt ALNSOptimisation::repairSingle(RoutesOpt routesToRepair, Patient patient, int n_route) {
    Node n1(patient, 0);
    vector<Route> routes(routesToRepair.getRoutes());
    tuple<Node, vector<Node>, vector<Node>> data(routes[n_route].addNodeInRoute(patient, routesToRepair, n_route));
    Node node(std::get<0>(data));
    vector<Node> first (std::get<1>(data));
    node.setArrivalTime(first.back().getDeparturTime() 
                        + HCData::getDistance(first.back().getDistancesIndex(), node.getDistancesIndex()));
    first.push_back(node);
    vector<Node> second (std::get<2>(data));   
    vector<Node> completed (Route::mergeLists(first, second, routesToRepair, n_route)); 
    routes[n_route].updateRoute(completed);
    RoutesOpt repaired(routes);
    return repaired;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////         REPAIR

RoutesOpt ALNSOptimisation::repairDouble(RoutesOpt routesToRepair, Patient patient, 
                                            int first_route, int second_route) {
    Node n1(patient, 0);
    vector<Route> routes(routesToRepair.getRoutes());
    tuple<Node, vector<Node>, vector<Node>> data1(routes[first_route]
                                                .addNodeInRoute(patient, routesToRepair, first_route));
    Node node1(std::get<0>(data1));
    vector<Node> *first1  = &std::get<1>(data1);
    vector<Node> *second1 = &std::get<2>(data1);
    tuple<Node, vector<Node>, vector<Node>> data2(routes[second_route]
                                        .addNodeInRoute(patient.getPatientAndNextService(node1.getArrivalTime()),       
                                                                                    routesToRepair, second_route));
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
    vector<Node> route1(Route::mergeLists(*first1, *second1, routesToRepair, first_route));
    vector<Node> route2(Route::mergeLists(*first2, *second2, routesToRepair, second_route));
    routes[first_route].updateRoute(route1);
    routes[second_route].updateRoute(route2);
    
    RoutesOpt repaired(routes);
    return repaired;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////      SAVE

void ALNSOptimisation::saveRepair(RoutesOpt& repaired) {
    double cost = calculateCost(repaired.getRoutes());
    string hash = ALNSOptimisation::makeHash(repaired.getRoutes());
    //salva soluzione
    m_solutionsDump.emplace(hash, repaired.getRoutes());
    //classifica soluzioni
    m_solutionsRank.push_back(make_pair(cost, hash));
    sort(m_solutionsRank.begin(), m_solutionsRank.end(), 
            [] (pair<double, string> e1, pair<double, string> e2) { return e1.first < e2.first; });
    m_currentSol = m_solutionsRank[0].second;
    m_currentCost = m_solutionsRank[0].first;
    resetOperation();
}

void ALNSOptimisation::saveDestruction(RoutesOpt& destructed, int n_route, int pos_node) {
    m_nodeToRelocate.push_back(m_ops.getNodeInRoute(n_route, pos_node).getId());
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

    return HCData::MAX_IDLE_TIME_WEIGHT * maxIdleTime + HCData::MAX_TARDINESS_WEIGHT * maxTardiness 
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

RoutesOpt ALNSOptimisation::getCurrentSchedule() { return m_ops; }

RoutesOpt ALNSOptimisation::getBestSol() { 
    string bestHash = m_solutionsRank[0].second;
    RoutesOpt routes(m_solutionsDump[bestHash]);
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
    RoutesOpt routes(m_solutionsDump[m_currentSol]);
    m_ops = routes;
}

string ALNSOptimisation::makeHash(vector<Route> t_routes) {
    stringstream hash;
    for (Route const & route : t_routes) { hash << route.getHash(); }
    return hash.str();
}

//////////////////////////////////////////////////////////////////////////////  Cancella 
void ALNSOptimisation::printNodeToRelocate() {
    for (string s : m_nodeToRelocate) {
        cout<<s<<"--";
    }
    cout<<endl;
}