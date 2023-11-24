#ifndef SOLOMON_HPP
#define SOLOMON_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "../node/node.hpp"
#include "../route/route.hpp"

using namespace std;
namespace homecare{

class ProblemDef
{
private:
    std::vector<Node> nodes;
    double** distances;
    Route solution;
    
public:
    ProblemDef(string t_pathDataFile);
    ~ProblemDef();
};

ProblemDef::ProblemDef(string t_pathDataFile){
    std::ifstream file(t_pathDataFile);
    if (!file.is_open()) {
        throw std::runtime_error("Errore nell'apertura del file dei dati del sistema.");
    }


}

ProblemDef::~ProblemDef(){}
 
}

#endif