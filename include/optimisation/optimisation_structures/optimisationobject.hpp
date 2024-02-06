#ifndef HCOPTIMISATIONOBJECT_HPP
#define HCOPTIMISATIONOBJECT_HPP

#include <vector>
#include <map>
#include <set>
#include <sstream>

#include "../../readjson/hcdata.hpp"
#include "../../structures/route.hpp"
#include "infonode.hpp"

namespace homecare {

class OptimisationObject {
private:
    HCData m_data;
    int m_actual;
    std::vector<Route> m_operation;
    std::vector<std::string> m_nodeToRelocate;
    //m_mapOfPatient memorizza gli indici di route e posizione dove si trovano i servizi
    std::map<std::string, std::map<std::string, InfoNode>> m_mapOfPatient;    
    std::map<double, std::map<std::string, int>> m_solutionsRank;   //costo, hash soluz, posizione in dump
    std::vector<std::vector<Route>> m_solutionsDump;                //lista di soluzioni viste (più simile ad un set)
    std::string makeHash(std::vector<Route>) const;
    int destroy(int, int);                                          //ritorna il numero di elementi distrutti
    //int repair(int node_index, int route, int position in node to relocate)
    static const int NOT_ASSIGED;

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