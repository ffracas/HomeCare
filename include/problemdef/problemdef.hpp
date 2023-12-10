#ifndef PROBLEMDEF_HPP
#define PROBLEMDEF_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <regex>
#include <cmath>
#include "../route/route.hpp"
#include "../params/params.hpp"
#include "../utils/utils.hpp"

using namespace std;
namespace homecare{

class ProblemDef
{
private:
    std::vector<Node> m_nodes;
    double** m_distances;
    vector<Route> m_solution;
    Params m_params;
    int m_depotIndex;
    static const int NO_INDEX = -1;

    int searchForRoute(Node, int = NO_INDEX);
    double syncTime(int, int, Node);
    
public:
    ProblemDef(string, string, string);
    ~ProblemDef();
    void generateFirstSolution();
};

}  //homecare

#endif