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
#include "routesopt.hpp"
#include "costcoord.hpp"

namespace homecare {

class ALNSOptimisation {
private:
    static double m_currentCost;
    static std::string m_currentSol;
    static std::vector<std::string> m_nodeToRelocate;
    static RoutesOpt m_ops;    
    static std::vector<std::pair<double, std::string>> m_solutionsRank;        //costo, hash sol
    static std::map<std::string, std::vector<Route>> m_solutionsDump;          //hash sol, sol
   
public:
    static const double MAX_DOUBLE;

    ~ALNSOptimisation();
    void setISol(std::vector<Route>, double);

    /**/ 
    static RoutesOpt destroy(RoutesOpt, int, int);                                     
    /**/
    /**/
    static RoutesOpt repairDouble(RoutesOpt, Patient, int, int);
    static RoutesOpt repairSingle(RoutesOpt, Patient, int);
    /**/
    void saveDestruction(RoutesOpt&, int, int);
    void saveRepair(RoutesOpt&);
    /**/
    static double calculateCost(const std::vector<Route>&);
    static int getNumberOfRoutes();
    /**/
    static void resetOperation();
    static std::string makeHash(const std::vector<Route>);
    static RoutesOpt getCurrentSol();
    static double getCurrentCost();

    /*Node to repiar*/
    static bool hasNodeToRepair();                                  // Node to repair
    static std::string popNodeToRepair();                           // Node to repair

    /*other utilities*/
    static double generateRandom();                                 // Random generator
    
    /*DA RIVEDERE O ELIMINARE*/
    /*int destroy(int, int, std::vector<Route>&, bool = false);            //ritorna il numero di elementi distrutti
    //int destroy(int, int);
    //double repair(std::vector<Route>&, Patient, int);
    //CostCoord repair(Node, int); 
    //CostCoord repair(std::vector<Route>&, Node, int, bool = false);*/
    //static bool isServiceAvailableInRoute(RoutesOpt&, std::string, int);
    //static int getNumberOfRoutes(RoutesOpt&);
    //static int getNumberOfNodesInRoute(RoutesOpt&, int) noexcept (false);
    //static Node getNodeInRoute(RoutesOpt&, int, int) noexcept (false);
};

}

#endif