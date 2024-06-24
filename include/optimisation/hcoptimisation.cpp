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
    m_ops->resetIteration();
    while (m_ops->startIteration() < MAX_ITERATIONS) {
        
        int pairIndex = roulette.selectPair();
        int points = 0;
        switch (pairIndex)
        {
        case Roulette::RANDOM_GREEDY:
            rare.removeNodes(ELEMENT_TO_DESTROY);
            points = gr.repairNodes();
            break;
        case Roulette::WORST_GREEDY:
            wore.removeNodes(ELEMENT_TO_DESTROY);
            points = gr.repairNodes();
            break;
        case Roulette::RELATED_GREEDY:
            rere.removeNodes(ELEMENT_TO_DESTROY);
            points = gr.repairNodes();
            break;
        case Roulette::CLUSTER_GREEDY:
            clre.removeNodes(ELEMENT_TO_DESTROY);
            points = gr.repairNodes();
            break;
        case Roulette::RANDOM_REGRET:
            rare.removeNodes(ELEMENT_TO_DESTROY);
            points = rr.repairNodes();
            break;
        case Roulette::WORST_REGRET:
            wore.removeNodes(ELEMENT_TO_DESTROY);
            points = rr.repairNodes();
            break;
        case Roulette::RELATED_REGRET:
            rere.removeNodes(ELEMENT_TO_DESTROY);
            points = rr.repairNodes();
            break;
        case Roulette::CLUSTER_REGRET:
            clre.removeNodes(ELEMENT_TO_DESTROY);
            points = rr.repairNodes();
            break;
        
        default:
            break;
        }
        
        cout<<"\nPoints: "<<points<<endl;
        roulette.updatePoints(pairIndex, points);
        //cout<<"\nrem\n";
        //rare.removeNodes(ELEMENT_TO_DESTROY); // todo: corretto insert in roulette
        //rere.removeNodes(ELEMENT_TO_DESTROY); // todo: corretto insert in roulette
        //wore.removeNodes(ELEMENT_TO_DESTROY); // todo: corretto insert in roulette
        //clre.removeNodes(ELEMENT_TO_DESTROY); // todo: corretto insert in roulette

        

        //todo: to delete
        /*cout<<endl;
        for (const string& nodeToTest : m_ops->getNodesToRepair()) {
            cout<<nodeToTest<<" - ";
        }
        cout<<endl;
        for (auto route : m_ops->getCurrentSchedule().getSchedule()) {
            for (auto node : route.getNodes()) {
                cout<<node.getId()<<"->";
            }
            cout<<endl;
        }*/

        //cout<<"\nrep\n";
        //int points = gr.repairNodes();
        //int points = rr.repairNodes();
        //cout<<"\nPoints: "<<points<<endl;
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
   