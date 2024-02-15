#ifndef HCOPTIMISATIONOBJECT_HPP
#define HCOPTIMISATIONOBJECT_HPP

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

class OptimisationObject {
private:
    HCData m_data;
    double m_actualCost;
    std::string m_actualSol;
    std::vector<Route> m_operation;

    std::vector<std::string> m_nodeToRelocate;
    const double p_worst;                                               //parametro worst removal
    const double p_related;                                             //parametro related removal
    const double p_distanceWeight;                                      //parametro related removal
    const double p_windowWeight;                                        //parametro related removal
    const double p_serviceWeight;                                       //parametro related removal

    //m_mapOfPatient memorizza gli indici di route e posizione dove si trovano i servizi
    std::map<std::string, std::map<std::string, InfoNode>> m_mapOfPatient;    
    std::vector<std::pair<double, std::string>> m_solutionsRank;        //costo, hash sol
    std::map<std::string, std::vector<Route>> m_solutionsDump;          //hash sol, sol
    std::string makeHash(std::vector<Route>) const;

    int destroy(int, int, std::vector<Route>&, bool = true);            //ritorna il numero di elementi distrutti
    //int repair(int node_index, int route, int position in node to relocate)
    double calculateCost(std::vector<Route>);
    double calculateSimilarity(int, int, int, bool, int, int, int);
    double generateRandom();
    void updateMapOfPatient(Route, int);
    void resetOperation();
    void findMinMaxRelatetion(int&, int&, int&, int&, int&, int, int);
    std::pair<int, int> getRandomNode(std::vector<std::pair<int, int>>) noexcept (false);

public:
    OptimisationObject(HCData, std::vector<Route>, double);
    ~OptimisationObject();
    /*metodi di destroy repair*/
    void randomRemoval  (int);
    void worseRemoval   (int);
    void relatedRemoval (int);
    void clusterRemoval (int);
};

}

#endif