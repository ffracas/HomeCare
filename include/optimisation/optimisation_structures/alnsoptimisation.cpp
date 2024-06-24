#include "alnsoptimisation.hpp"

using namespace std;
using namespace homecare;

std::optional<ALNSOptimisation*> ALNSOptimisation::m_instance;

/**
 * Constructs an ALNSOptimisation instance with the given initial schedule and cost.
 *
 * @param t_firstSchedule The initial schedule for the optimization.
 * @param t_cost The initial cost of the optimization.
 */
ALNSOptimisation::ALNSOptimisation(Schedule t_firstSchedule, double t_cost)
    : m_currentCost(t_cost), m_currentSol(makeHash(t_firstSchedule.getSchedule())),
      m_nodeToRelocate(), m_ops(t_firstSchedule)
{
    m_solutionsDump.emplace(m_currentSol, t_firstSchedule);
    m_solutionsRank.push_back(make_pair(t_cost, m_currentSol));
}


ALNSOptimisation::~ALNSOptimisation() {}

/**
 * Gets the singleton instance of the ALNSOptimisation class, creating it if it doesn't exist.
 *
 * @param t_firstSolution The initial solution for the optimization.
 * @param t_cost The initial cost of the optimization.
 * @return The singleton instance of the ALNSOptimisation class.
 * @throws runtime_error if no solution is found for the optimization.
 */
ALNSOptimisation* ALNSOptimisation::getInstance(Schedule& t_firstSolution, double t_cost) {
    if (!m_instance.has_value()) {
        m_instance = new ALNSOptimisation(t_firstSolution, t_cost);
    }
    return m_instance.value();
}

/**
 * Returns the singleton instance of the ALNSOptimisation class.
 *
 * @return The singleton instance of ALNSOptimisation.
 * @throws runtime_error if the instance has not been initialized.
 */
ALNSOptimisation* ALNSOptimisation::getInstance() noexcept (false) {
    if (!m_instance.has_value()) {
        throw runtime_error("\n[ALNSOpt] Error: No solution found for optimisation\n");
    }
    return m_instance.value();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////         DESTROY

/**
 * Removes a node from the specified route in the given schedule optimiser.
 *
 * @param routes The schedule optimiser containing the routes.
 * @param n_route The index of the route to remove the node from.
 * @param n_node The index of the node to remove from the route.
 * @return The updated schedule optimiser with the node removed.
 */

ScheduleOptimiser ALNSOptimisation::destroy(ScheduleOptimiser routes, int n_route, int n_node) {
    if (!routes.isNodeIndexValid(n_route, n_node)) {
        return ScheduleOptimiser();
    }
    // save a copy of the node to delete
    Node deleted(routes.getNodeFromRoute(n_route, n_node));
    // delete the node
    routes.destroyNode(n_route, n_node);
    if (deleted.isInterdependent()) {
        InfoNode otherNode = routes.getInterdependetInfo(deleted.getId(), deleted.getService(), n_route).second;
        routes.destroyNode(otherNode.getRoute(), otherNode.getPositionInRoute());
    }
    routes.destroyPatientRef(deleted.getId());  
    return routes;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////        REPAIR 1

ScheduleOptimiser ALNSOptimisation::repairSingle(ScheduleOptimiser routes, Patient patient, int n_route) {
    if (!routes.isIndexValid(n_route)) { 
        throw out_of_range("[ALNSOptimisation] RepairSingle Error, route selected is out of range"); 
    }
    if (routes.repairNode(n_route, patient) == false) {
        return ScheduleOptimiser();
    }
    return routes;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////        REPAIR 2

ScheduleOptimiser ALNSOptimisation::repairDouble(ScheduleOptimiser routes, Patient patient, 
                                            int first_route, int second_route) {
    if (!routes.isIndexValid(first_route) ||!routes.isIndexValid(second_route)) {
        throw out_of_range("[ALNSOptimisation] RepairDouble Error, route selected is out of range");
    }
    if (routes.repairNode(first_route, patient)) {
        int time_s1 = routes.getRoute(first_route).getNodeArrivalTime(patient.getID());
        if (routes.repairNode(second_route, patient.getPatientAndNextService(time_s1), true)) {
            return routes;
        }
    }
    if (routes.repairNode(second_route, patient.getPatientforS2Sync())) {
        int time_s2 = routes.getRoute(second_route).getNodeArrivalTime(patient.getID());
        if (routes.repairNode(first_route, patient.getPatientforS1Sync(time_s2), true)) {
            return routes;
        }
    }
    return ScheduleOptimiser();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////      ITER

int ALNSOptimisation::startIteration() {
    resetOperation();
    return m_iteration ++;
}

int ALNSOptimisation::resetIteration() {
    m_iteration = 0;
    return m_iteration;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////      SAVE

int ALNSOptimisation::saveRepair(ScheduleOptimiser& repaired) {
    double cost = repaired.getCost();
    string hash = ALNSOptimisation::makeHash(repaired.getSchedule());
    // evaluation of the solution
    int points = OTHERWISE;
    if (m_solutionsDump.find(hash) == m_solutionsDump.end()) {
        m_solutionsDump.insert({hash, repaired});
        if (m_solutionsRank[0].first > cost) {
            points = BEST_SOLUTION;
        }
        else if (m_currentCost > cost) {
            points = BETTER_THAN_CURRENT;
        }
        else if (isAccepted(m_solutionsRank[0].first, cost)) {
            points = NO_BETTER_BUT_CURRENT;
        }
        // insert solution into the rank
        m_solutionsRank.push_back(make_pair(cost, hash));
        sort(m_solutionsRank.begin(), m_solutionsRank.end(), 
            [] (pair<double, string> e1, pair<double, string> e2) { return e1.first < e2.first; });
    }
    if (points == NO_BETTER_BUT_CURRENT) {
        m_currentSol = hash;
        m_currentCost = cost;
    } else {
        m_currentSol = m_solutionsRank[0].second;
        m_currentCost = m_solutionsRank[0].first;
    }
    resetOperation();
    return points;
}

void ALNSOptimisation::saveDestruction(ScheduleOptimiser& destructed, int n_route, int n_node) {
    m_nodeToRelocate.push_back(m_ops.getNodeFromRoute(n_route, n_node).getId());
    m_ops = destructed;
}

//////////////////////////////////////////////////////////////////////////////////////// LIST NODE TO REPAIR
/**
 * Checks if there are nodes to repair.
 * 
 * @return True if there are nodes to repair, false otherwise.
 */
bool ALNSOptimisation::hasNodeToRepair() { return !m_nodeToRelocate.empty(); }

vector<string> ALNSOptimisation::getNodesToRepair() const {
    return m_nodeToRelocate;
}

string ALNSOptimisation::scheduledNode(string scheduled) {
    if (!m_nodeToRelocate.empty()) {
        auto ip = find(m_nodeToRelocate.begin(), m_nodeToRelocate.end(), scheduled);
        if (ip != m_nodeToRelocate.end()) { 
            m_nodeToRelocate.erase(ip);
            return *ip; 
        }
    }
    throw runtime_error("[NodeToRelocate]Error: No " + scheduled + " node found");
}

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
 * Generates a random double between 0 and 1.0.
 * Uses the Mersenne Twister pseudo-random number generator.
 * 
 * @return A random double between 0 and 1.0.
 */
double ALNSOptimisation::generateRandom() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 0.870); // fixme: the round is not equal 1 - or multiply result |list|-1

    // Genera un numero casuale tra 0 e 1
    return dis(gen);
}

void ALNSOptimisation::resetOperation() {
    m_nodeToRelocate.clear();
    ScheduleOptimiser routes(m_solutionsDump[m_currentSol]);
    m_ops = routes;
}

string ALNSOptimisation::makeHash(const vector<Route>& t_routes) {
    stringstream hash;
    for (Route const & route : t_routes) { hash << route.getHash(); }
    return hash.str();
}

bool ALNSOptimisation::isAccepted(double bestCost, double newCost) { 
    int T0 = 1000;
    double c = 0.5;
    double Tk = (double) T0 / (1 + c * log(1 + m_iteration));
    double expn = - (newCost - bestCost) / Tk;
    double target = exp(expn);
    double random = generateRandom();
    return random < target; 
}
