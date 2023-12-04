#include "route.hpp"

using namespace homecare;
using namespace std;

/**
 * @brief Constructor for the Node class.
 * 
 * Initializes a Route object.
*/
Route::Route(Node t_depot) : m_depot(t_depot) {}

Route::~Route() {}

double Route::getCurrentTime() { return m_currentTime; }

int Route::addNextNode(Node t_arrival, double** t_distances) {
    //insert new node in route
    if(m_arcs_in_route.size() >= BASE_ROUTE_LEN) {
        m_arcs_in_route.pop_back();             //delete old last arc
    }
        
    Node lastNode = m_arcs_in_route.size() == 0 ? m_depot : m_arcs_in_route[m_arcs_in_route.size() - 1].getArrival(); 
    double distance = t_distances[lastNode.getID()][t_arrival.getID()];
    t_arrival.setArrivalTime(lastNode.getDeparturTime() + distance);
    m_arcs_in_route.push_back(Arc(lastNode, t_arrival, distance));                                      //insert new arc
    m_arcs_in_route.push_back(Arc(t_arrival, m_depot, t_distances[m_depot.getID()][t_arrival.getID()])); //insert new last arc
    m_quantity += t_arrival.getDemand();
    m_currentTime = t_arrival.getDeparturTime();
    
    return m_arcs_in_route.size() - 1; //do not count the arc that return to depot
}

void Route::setSeed(Node t_seed, double t_distance) {
    if(m_arcs_in_route.size() > 0) return;
    t_seed.setArrivalTime(t_distance);
    m_quantity = t_seed.getService();
    Arc arc1(m_depot, t_seed, t_distance);
    m_arcs_in_route.push_back(arc1);
    Arc arc2(t_seed, m_depot, t_distance);
    m_arcs_in_route.push_back(arc2);
}

bool Route::contains(int t_nodeId) {
    if(m_depot.getID() == t_nodeId) return true;
    for (Arc arc : m_arcs_in_route){
        if(arc.getArrival().getID() == t_nodeId) return true;
    }
    return false;
}

int Route::searchForNextNode(vector<Node> t_nodes, double** t_distances, Params params) {
    int nextIndex = -1;
    int arcIndex = -1;
    double bestCost = homecare::MIN_DOUBLE;
    for(int i = 0; i < t_nodes.size(); ++i) {
        if(!(t_nodes[i].isSync() && contains(t_nodes[i].getSyncNode()))) {
            for(int j = 0; j < m_arcs_in_route.size(); ++j) {
                Node node_i = m_arcs_in_route[j].getDeparture();
                Node node_j = m_arcs_in_route[j].getArrival();
                double d_ij = m_arcs_in_route[j].getDistance();
                double d_iu = t_distances[node_i.getID()][t_nodes[i].getID()];
                double d_uj = t_distances[t_nodes[i].getID()][node_j.getID()];
                double u_arrivalTime = node_i.getDeparturTime() + d_iu > t_nodes[i].getWindowStartTime() ? 
                                        node_i.getDeparturTime() + d_iu : t_nodes[i].getWindowStartTime();
                if(!(t_nodes[i].getWindowEndTime() < node_i.getWindowStartTime() 
                        || t_nodes[i].getWindowStartTime() > node_j.getWindowEndTime()
                        || node_i.getDeparturTime() + d_iu > t_nodes[i].getWindowEndTime() 
                        || u_arrivalTime + t_nodes[i].getService() > node_j.getWindowEndTime())) {
                    Route newRoute(*this);
                    if(newRoute.addNodeBetween(j, t_nodes[i], t_distances) != EMPTY_ROUTE){
                        double c_11 = d_iu + d_uj - params.mu() * d_ij;
                        double c_12 = newRoute.getCurrentTime() - m_currentTime; 
                        double cost = params.lambda() * t_distances[m_depot.getID()][t_nodes[i].getID()] - 
                                (params.alfa1() * c_11 + params.alfa2() * c_12);
                        if(bestCost < cost){
                            nextIndex = i;
                            bestCost = cost;
                        }
                    }
                    
                }
            }
        }
    }
    if(nextIndex != -1 && arcIndex != -1){
        addNodeBetween(arcIndex, t_nodes[nextIndex], t_distances);
    }
    return nextIndex;
}

int Route::addNodeBetween(int t_arcIndex, Node t_node, double** t_distances){                   //ij + u -> iuj
    int arcNum = m_arcs_in_route.size();
    if(t_arcIndex < 0 || t_arcIndex > arcNum) return EMPTY_ROUTE;
    vector<Arc> backup = m_arcs_in_route;
    Node node_i = t_arcIndex == 0 ? m_depot : m_arcs_in_route[t_arcIndex].getDeparture();       //departure Node in solomon notation
    Node node_j = t_arcIndex == arcNum ? m_depot : m_arcs_in_route[t_arcIndex].getArrival();    //arrival Node in solomon notation
    m_currentTime -= m_arcs_in_route[t_arcIndex].getDistance();
    m_arcs_in_route.erase(m_arcs_in_route.begin() + t_arcIndex);
    double distance_iu = t_distances[node_i.getID()][t_node.getID()];
    double distance_uj = t_distances[t_node.getID()][node_j.getID()];
    t_node.setArrivalTime(node_i.getDeparturTime() + distance_iu); 
    node_j.setArrivalTime(t_node.getDeparturTime() + distance_uj);
    m_arcs_in_route.insert(m_arcs_in_route.begin() + t_arcIndex, Arc(node_i, t_node, distance_iu));
    m_arcs_in_route.insert(m_arcs_in_route.begin() + t_arcIndex + 1, Arc(t_node, node_j, distance_uj));
    ++arcNum;
    m_quantity += t_node.getDemand();
    m_currentTime += distance_iu + distance_uj +  t_node.getService();
    for(int j = t_arcIndex; j < arcNum; ++j) {
        double newTime = m_arcs_in_route[j].getDeparture().getDeparturTime() + m_arcs_in_route[j].getDistance();
        if(!m_arcs_in_route[j].getArrival().isInTimeWindow(newTime)) {
            m_arcs_in_route = backup;
            return EMPTY_ROUTE;
        }
        else {
            m_arcs_in_route[j].getArrival().setArrivalTime(newTime);
        }
    }
    return m_arcs_in_route.size();
}

string Route::getRouteToString(){
    string s_route = "ROUTE\n";
    for (Arc arc : m_arcs_in_route){
        s_route += "ARC - partenza: " + arc.getDeparture().getID();
        s_route += " alle: " + std::to_string(arc.getDeparture().getDeparturTime());
        s_route += " -> arrivo: " + arc.getArrival().getID();
        s_route += " alle: " + std::to_string(arc.getArrival().getArrvalTime()) + "\n";
    }
    return s_route;
}