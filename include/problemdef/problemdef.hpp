#ifndef PROBLEMDEF_HPP
#define PROBLEMDEF_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <regex>
#include <math.h>
#include "../route/route.hpp"
#include "../params/params.hpp"

using namespace std;
namespace homecare{

class ProblemDef
{
private:
    std::vector<Node> m_nodes;
    double** m_distances;
    Route m_solution;
    Params m_params;
    int m_camionNumber;
    int m_camionCapacity;
    int m_depotIndex;
    
public:
    ProblemDef(string, string, string);
    ~ProblemDef();
};

}  //homecare

#endif