#ifndef ALNSOPTIMISATION_HPP
#define ALNSOPTIMISATION_HPP

#include <vector>
#include <map>
#include <sstream>
#include <cmath>
#include <random>

#include "../../readjson/hcdata.hpp"
#include "../../structures/route.hpp"
#include "infonode.hpp"

namespace homecare {

struct CostCoord {
    double cost;
    int n_route; 
    int n_node;
};

class ALNSOptimisation {
private:
    double m_currentCost;
    std::string m_currentSol;
    std::vector<Route> m_routes;
    std::vector<std::string> m_nodeToRelocate;
    //m_mapOfPatient memorizza gli indici di route e posizione dove si trovano i servizi
    std::map<std::string, std::map<std::string, InfoNode>> m_mapOfPatient;    
    std::vector<std::pair<double, std::string>> m_solutionsRank;        //costo, hash sol
    std::map<std::string, std::vector<Route>> m_solutionsDump;          //hash sol, sol
   
    void updateMapOfPatient(Route, int);

public:
    ALNSOptimisation(std::vector<Route>, double);
    ~ALNSOptimisation();
    void resetOperation();
    int destroy(int, int, std::vector<Route>&, bool = false);            //ritorna il numero di elementi distrutti
    int destroy(int, int);
    //int repair(int node_index, int route, int position in node to relocate) //TODO
    static std::string makeHash(std::vector<Route>);
    double calculateCost(const std::vector<Route>&) const;
    double generateRandom() const;
    double getCurrentCost() const;
    int getNumberOfRoutes() const;
    int getNumberOfNodesInRoute(int) const noexcept (false);
    Node getNodeInRoute(int, int) const noexcept (false);
    std::vector<Route> getRoutes() const;
};

}

#endif