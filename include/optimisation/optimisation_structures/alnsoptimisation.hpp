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
#include "servicemanager.hpp"
#include "costcoord.hpp"

namespace homecare {

class ALNSOptimisation {
private:
    double m_currentCost;
    std::string m_currentSol;
    std::vector<Route> m_routes;
    std::vector<std::string> m_nodeToRelocate;
    //m_mapOfPatient memorizza gli indici di route e posizione dove si trovano i servizi
    std::map<std::string, ServiceManager> m_mapOfPatient;    
    std::vector<std::pair<double, std::string>> m_solutionsRank;        //costo, hash sol
    std::map<std::string, std::vector<Route>> m_solutionsDump;          //hash sol, sol
   
    void updateMapOfPatient(const Route&, int);
    void updateMapOfPatient();

public:
    static const double MAX_DOUBLE;

    ALNSOptimisation(std::vector<Route>, double);
    ~ALNSOptimisation();
    void resetOperation();
    int destroy(int, int, std::vector<Route>&, bool = false);            //ritorna il numero di elementi distrutti
    int destroy(int, int);
    double repair(std::vector<Route>&, Patient, int);
    CostCoord repair(Node, int); 
    CostCoord repair(std::vector<Route>&, Node, int, bool = false);
    bool repairDouble(Patient, int);
    void saveSol(const std::vector<Route>&);
    static std::string makeHash(std::vector<Route>);
    double calculateCost(const std::vector<Route>&) const;
    double generateRandom() const;
    double getCurrentCost() const;
    int getNumberOfRoutes() const;
    int getNumberOfNodesInRoute(int) const noexcept (false);
    Node getNodeInRoute(int, int) const noexcept (false);
    std::vector<Route> getRoutes() const;
    bool hasNodeToRepair() const;
    std::string popNodeToRepair();
    std::pair<int, int> getSyncServiceWindow(std::string, std::string); //pazinete, servizio
    void updateSyncServiceTime(std::string, std::string, int);          //pazinete, servizio, nuovo tempo 
};

}

#endif