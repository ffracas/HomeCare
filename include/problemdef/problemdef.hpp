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
    vector<Route> m_solution;
    Params m_params;
    int m_camionNumber;
    int m_camionCapacity;
    int m_depotIndex;
    static const int NO_INDEX_NODE = -1;

    int searchForSeed(vector<Node>);
    int searchForNextNode(vector<Node>, Route);
    
public:
    ProblemDef(string, string, string);
    ~ProblemDef();
    void generateFirstSolution();
};

}  //homecare

#endif