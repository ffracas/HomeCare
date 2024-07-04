#ifndef ALNSOPTIMISATION_HPP
#define ALNSOPTIMISATION_HPP

#include <map>
#include <sstream>
#include <cmath>
#include <math.h>
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
    // params
    int m_iteration = 0;
    double m_t_start = 100;
    double m_coolingRate = 0.980;
    double m_omega_T = 0.05;
    const double m_t_f = 0.05;
    // fields
    double m_currentCost;
    std::string m_currentSol;
    std::vector<std::string> m_nodeToRelocate;
    ScheduleOptimiser m_ops;   
    std::vector<std::pair<double, std::string>> m_solutionsRank;        //costo, hash-sol
    std::map<std::string, Schedule> m_solutionsDump;                    //hash-sol, sol

    ALNSOptimisation(Schedule, double);
    ALNSOptimisation(const ALNSOptimisation&) = delete;
    ALNSOptimisation& operator=(const ALNSOptimisation&) = delete;

    bool isAccepted(double, double);
   
public:
    ~ALNSOptimisation();

    static const int BEST_SOLUTION = 3;
    static const int BETTER_THAN_CURRENT = 1;
    static const int NO_BEST_BUT_CURRENT = 2;
    static const int OTHERWISE = 0;

    static ALNSOptimisation* getInstance(Schedule&, double);
    static ALNSOptimisation* getInstance() noexcept (false);
     
    // main Operation
    ScheduleOptimiser destroy(ScheduleOptimiser, int, int) noexcept (false);   
    ScheduleOptimiser repairDouble(ScheduleOptimiser, Patient, int, int);
    ScheduleOptimiser repairSingle(ScheduleOptimiser, Patient, int);
    bool startIteration();
    int resetWeight();
    int getNElementToDestroy();

    // save operation
    void saveDestruction(ScheduleOptimiser&, int, int);
    int saveRepair(ScheduleOptimiser&);
    void resetOperation();

    // getter
    double getCurrentCost();
    ScheduleOptimiser getCurrentSchedule();
    ScheduleOptimiser getBestSol();

    // Node to repiar
    bool hasNodeToRepair();                                  
    std::vector<std::string> getNodesToRepair() const;
    std::string scheduledNode(std::string) noexcept (false);
    std::string popNodeToRepair() noexcept (false);          

    // other utilities
    static std::string makeHash(const std::vector<Route>&);         //Fixme: non mi sembra necessaria
    static double generateRandom();                                 // Random generator
};

}

#endif