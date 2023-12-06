#include "route.hpp"

using namespace homecare;
using namespace std;

/**
 * @brief Constructor for the Node class.
 * 
 * Initializes a Route object.
*/
Route::Route(Node t_depot) : m_depot(t_depot), m_quantity(0.0), m_currentTime(0.0) {}

Route::~Route() {}

double Route::getCurrentTime() { return m_currentTime; }

int Route::addNextNode(Node t_newNode, double** t_distances) {
    if(m_arcs_in_route.size() >= BASE_ROUTE_LEN) {
        return addNodeBetween(m_arcs_in_route.size() - 1, t_newNode, t_distances);
    }
     
    Node lastNode = m_depot; 
    double distance = t_distances[lastNode.getID()][t_newNode.getID()];
    t_newNode.setArrivalTime(lastNode.getDeparturTime() + distance);
    lastNode.setArrivalTime(t_newNode.getDeparturTime() + distance);
    m_arcs_in_route.push_back(Arc(lastNode, t_newNode, distance));                                         //insert new arc
    m_arcs_in_route.push_back(Arc(t_newNode, lastNode, distance));  //insert new last arc
    m_quantity += t_newNode.getDemand();
    m_currentTime = t_newNode.getDeparturTime();
    
    return m_arcs_in_route.size() - 1; //do not count the arc that return to depot*/
}

bool Route::contains(int t_nodeId) {
    if(m_depot.getID() == t_nodeId) return true;
    for (Arc arc : m_arcs_in_route){
        if(arc.getArrival().getID() == t_nodeId) return true;
    }
    return false;
}

int Route::searchForNextNode(vector<Node> t_nodes, double** t_distances, Params params) {
    int nodeIndex = -1;
    if(t_nodes.size() < 1) return nodeIndex;
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
                    double newCurrentTime = valutateAlternativeRoute(j, t_nodes[i], t_distances);
                    if(newCurrentTime != EMPTY_ROUTE){
                        double c_11 = d_iu + d_uj - params.mu() * d_ij;
                        double c_12 = newCurrentTime - m_currentTime; 
                        double cost = params.lambda() * t_distances[m_depot.getID()][t_nodes[i].getID()] - 
                                (params.alfa1() * c_11 + params.alfa2() * c_12);
                        if(bestCost < cost){
                            nodeIndex = i;
                            arcIndex = j;
                            bestCost = cost;
                        }
                    }
                }
            }
        }
    }
    if(nodeIndex != -1 && arcIndex != -1){
        addNodeBetween(arcIndex, t_nodes[nodeIndex], t_distances);
    }
    return nodeIndex;
}

//da rifare per cambiare la gestione degli archi e dei nodi
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
    for(Arc arc : m_arcs_in_route){
        double newTime = arc.getDeparture().getDeparturTime() + arc.getDistance();
        if(!arc.getArrival().isInTimeWindow(newTime)) {
            m_arcs_in_route = backup;
            return EMPTY_ROUTE;
        }
        arc.setArrivalTime(newTime);
    }
    return m_arcs_in_route.size();
}

double Route::valutateAlternativeRoute(int t_arcIndex, Node t_node, double** t_distaces) {
    int newCurrentTime = 0;
    for(int i = 0; i < t_arcIndex; ++i) {
        newCurrentTime += m_arcs_in_route[i].getDistance();
        newCurrentTime  = (newCurrentTime > m_arcs_in_route[i].getArrival().getWindowStartTime() ?
                newCurrentTime : m_arcs_in_route[i].getArrival().getWindowStartTime())
                + m_arcs_in_route[i].getArrival().getService();
    }
    newCurrentTime += t_distaces[m_arcs_in_route[t_arcIndex].getDeparture().getID()][t_node.getID()];
    if(!t_node.isInTimeWindow(newCurrentTime)) return EMPTY_ROUTE;
    newCurrentTime = (newCurrentTime > t_node.getWindowStartTime() ? newCurrentTime : t_node.getWindowStartTime()) 
            + t_node.getService()
            + t_distaces[t_node.getID()][m_arcs_in_route[t_arcIndex].getArrival().getID()];
    if(!m_arcs_in_route[t_arcIndex].getArrival().isInTimeWindow(newCurrentTime)) return EMPTY_ROUTE;
    newCurrentTime = (newCurrentTime > m_arcs_in_route[t_arcIndex].getArrival().getWindowStartTime() ?
                newCurrentTime : m_arcs_in_route[t_arcIndex].getArrival().getWindowStartTime())
                + m_arcs_in_route[t_arcIndex].getArrival().getService();
    if(!m_arcs_in_route[t_arcIndex].getArrival().isInTimeWindow(newCurrentTime)) return EMPTY_ROUTE;
    for(int i = t_arcIndex + 1; i < m_arcs_in_route.size(); ++i) {
        newCurrentTime += m_arcs_in_route[i].getDistance();
        if(!m_arcs_in_route[i].getArrival().isInTimeWindow(newCurrentTime)) return EMPTY_ROUTE;
    }
    return newCurrentTime;
}

string Route::getRouteToString(){
    string s_route = "ROUTE\n";
    for (Arc arc : m_arcs_in_route){
        s_route += "ARC - partenza: " + to_string(arc.getDeparture().getID()) 
            + " alle: " + to_string(arc.getDeparture().getDeparturTime()) 
            + " . arrivo: " + to_string(arc.getArrival().getID()) 
            + " alle: " + to_string(arc.getArrival().getArrvalTime()) 
            + " servizio min: " + to_string(arc.getArrival().getService())  + "\n";
    }
    return s_route;
}