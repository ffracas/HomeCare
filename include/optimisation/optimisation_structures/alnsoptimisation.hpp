#ifndef ALNSOPTIMISATION_HPP
#define ALNSOPTIMISATION_HPP

#include <map>
#include <sstream>
#include <cmath>
#include <random>
#include <optional>

#include "../../readjson/hcdata.hpp"
#include "../../structures/service_manager/infonode.hpp" 
#include "../../structures/schedule/scheduleoptimiser.hpp"
#include "costcoord.hpp"

namespace homecare {

class ALNSOptimisation {
private:
    static std::optional<ALNSOptimisation*> m_instance;

    double m_currentCost;
    std::string m_currentSol;
    std::vector<std::string> m_nodeToRelocate;
    ScheduleOptimiser m_ops;   
    std::vector<std::pair<double, std::string>> m_solutionsRank;        //costo, hash-sol
    std::map<std::string, Schedule> m_solutionsDump;                    //hash-sol, sol

    ALNSOptimisation(Schedule, double);
    ALNSOptimisation(const ALNSOptimisation&) = delete;
    ALNSOptimisation& operator=(const ALNSOptimisation&) = delete;
   
public:
    ~ALNSOptimisation();

    static ALNSOptimisation* getInstance(Schedule&, double);
    static ALNSOptimisation* getInstance() noexcept (false);
     
    // main Operation
    ScheduleOptimiser destroy(ScheduleOptimiser, int, int) noexcept (false);     //todo: mettere in scheduleoptimiser
    ScheduleOptimiser repairDouble(ScheduleOptimiser, Patient, int, int);
    ScheduleOptimiser repairSingle(ScheduleOptimiser, Patient, int);

    // save operation
    void saveDestruction(ScheduleOptimiser&, int, int);
    void saveRepair(ScheduleOptimiser&);
    void resetOperation();

    // getter
    double getCurrentCost();
    ScheduleOptimiser getCurrentSchedule();
    ScheduleOptimiser getBestSol();

    // Node to repiar
    bool hasNodeToRepair();                                  // Node to repair
    std::string popNodeToRepair() noexcept (false);          // Node to repair

    // other utilities
    static double calculateCost(const std::vector<Route>&);  //Fixme: non mi sembra necessaria
    static std::string makeHash(const std::vector<Route>);   //Fixme: non mi sembra necessaria
    static double generateRandom();                                 // Random generator
};

}

#endif