#ifndef ALNSOPTIMISATION_HPP
#define ALNSOPTIMISATION_HPP

#include <vector>
#include <map>
#include <sstream>
#include <cmath>
#include <random>

#include "../../readjson/hcdata.hpp"
#include "../../structures/service_manager/infonode.hpp" 
#include "../../structures/schedule/scheduleoptimiser.hpp"
#include "costcoord.hpp"

namespace homecare {

// TODO: Trasformare in una classe singleton
class ALNSOptimisation {
private:
    static double m_currentCost;
    static std::string m_currentSol;
    static std::vector<std::string> m_nodeToRelocate;
    static RoutesOpt m_ops;    //FIXME: ScheduleOptimiser
    static std::vector<std::pair<double, std::string>> m_solutionsRank;        //costo, hash-sol
    static std::map<std::string, Schedule> m_solutionsDump;          //hash-sol, sol

    ALNSOptimisation();
   
public:
    ~ALNSOptimisation();
    static ALNSOptimisation& getInstance(Schedule&, double);
    /**/ 
    //RoutesOpt destroy(RoutesOpt, int, int) noexcept (false);                                     
    /**/
    //RoutesOpt repairDouble(RoutesOpt, Patient, int, int);
    //RoutesOpt repairSingle(RoutesOpt, Patient, int);
    /**/
    void saveDestruction(RoutesOpt&, int, int);
    void saveRepair(RoutesOpt&);
    /**/
    double calculateCost(const std::vector<Route>&);
    int getNumberOfRoutes();
    /**/
    void resetOperation();
    static std::string makeHash(const std::vector<Route>);
    //RoutesOpt getCurrentSchedule();
    RoutesOpt getBestSol();
    double getCurrentCost();

    /*Node to repiar*/
    bool hasNodeToRepair();                                  // Node to repair
    std::string popNodeToRepair() noexcept (false);          // Node to repair

    /*other utilities*/
    double generateRandom();                                 // Random generator
};

}

#endif