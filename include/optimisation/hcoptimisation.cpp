#include "hcoptimisation.hpp"

#include <csignal>
#include <iostream>
#include <cstdlib>
#include <execinfo.h>
#include <unistd.h>

using namespace std;
using namespace homecare;

HCOptimisation::HCOptimisation(Schedule t_schedule, double t_cost) 
        : m_solution (t_schedule), m_ops (ALNSOptimisation::getInstance(t_schedule, t_cost)) {}

HCOptimisation::~HCOptimisation() {}

void signalHandler(int signal) {
    if (signal == SIGSEGV) {
        std::cerr << "Segmentation Fault detected (signal " << signal << ")." << std::endl;

        // Ottenere il backtrace
        void* array[10];
        size_t size = backtrace(array, 10);

        // Stampare il backtrace
        std::cerr << "Backtrace:\n";
        char** messages = backtrace_symbols(array, size);
        for (size_t i = 0; i < size; i++) {
            std::cerr << messages[i] << std::endl;
        }
        free(messages);

        // Puoi aggiungere azioni di pulizia qui
        std::_Exit(EXIT_FAILURE);  // Termina il programma in modo sicuro
    }
}

Schedule HCOptimisation::optimise() {
    RandomRemoval rare;
    RelatedRemoval rere;
    WorstRemoval wore;
    ClusterRemoval clre;

    GreedyRepair gr; 
    RegretRepair rr;

    Roulette roulette;

    signal(SIGSEGV, signalHandler);
    
    //cout<<"\n-----\nIterarion "<<m_ops->startIteration()<<endl;
    m_ops->resetWeight();
    int i = 0;
    int firebreak = 0;
    while (m_ops->startIteration() && firebreak < MAX_ITERATIONS) {
        
        if (i == MAX_ITERATIONS) {
            i = 0;
            firebreak++;
            vector<int> occ = roulette.getOccurrences();
            for (int i = 0; i < occ.size(); ++i) {
                cout<<i<<": "<<occ[i]<<endl;
            }
            roulette.updateProbabilities();
            m_ops->resetWeight();
        }
        i++;

        cout<< "\n periodo "<<i<<endl;
        element_to_destroy = m_ops->getNElementToDestroy();
        cout<< "\n elementi da eliminare "<<element_to_destroy<<endl;
        cout<< "\n nuova iterazione\n";
        int pairIndex = roulette.selectPair();
        int points = 0;
        switch (pairIndex)
        {
        case Roulette::RANDOM_GREEDY:
            rare.removeNodes(element_to_destroy);
            points = gr.repairNodes();
            break;
        case Roulette::WORST_GREEDY:
            wore.removeNodes(element_to_destroy);
            points = gr.repairNodes();
            break;
        case Roulette::RELATED_GREEDY:
            rere.removeNodes(element_to_destroy);
            points = gr.repairNodes();
            break;
        case Roulette::CLUSTER_GREEDY:
            clre.removeNodes(element_to_destroy);
            points = gr.repairNodes();
            break;
        case Roulette::RANDOM_REGRET:
            rare.removeNodes(element_to_destroy);
            points = rr.repairNodes();
            break;
        case Roulette::WORST_REGRET:
            wore.removeNodes(element_to_destroy);
            points = rr.repairNodes();
            break;
        case Roulette::RELATED_REGRET:
            rere.removeNodes(element_to_destroy);
            points = rr.repairNodes();
            break;
        case Roulette::CLUSTER_REGRET:
            clre.removeNodes(element_to_destroy);
            points = rr.repairNodes();
            break;
        
        default:
            break;
        }
        
        cout<<"\nPoints: "<<points<<endl;
        roulette.updatePoints(pairIndex, points);
    }

    cout<<"\n----------\n";
    //todo: to delete
    for (auto route : m_ops->getCurrentSchedule().getSchedule()) {
        for (auto node : route.getNodes()) {
            cout<<node.getId()<<"->";
        }
        cout<<endl;
    }
       
    return m_ops->getBestSol();
}

// pilotated destroy for testing purposes
void HCOptimisation::destroyPilot() {
    ScheduleOptimiser routes(m_ops->getCurrentSchedule());
    ScheduleOptimiser r1(m_ops->destroy(routes, 0, 3));
    m_ops->saveDestruction(r1,0,3);
    cout<<"--------------\n";
    ScheduleOptimiser r2(m_ops->destroy(m_ops->getCurrentSchedule(), 12,1));
    m_ops->saveDestruction(r2,12,1);
    cout<<"--------------\n";
    ScheduleOptimiser r3(m_ops->destroy(m_ops->getCurrentSchedule(), 7,4));
    m_ops->saveDestruction(r3,7,4);
    cout<<"--------------\n";
    ScheduleOptimiser r4(m_ops->destroy(m_ops->getCurrentSchedule(), 12,1));
    m_ops->saveDestruction(r4,12,1);
    cout<<"--------------\n";
    ScheduleOptimiser r5(m_ops->destroy(m_ops->getCurrentSchedule(), 5,3));
    m_ops->saveDestruction(r5,5,3);
    cout<<"--------------\n";
}
   