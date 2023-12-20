#ifndef PROBLEMDEF_HPP
#define PROBLEMDEF_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <regex>
#include <cmath>
#include <json/json.h> 
#include "../route/route.hpp"
#include "../params/params.hpp"
#include "../utils/utils.hpp"

using namespace std;
namespace vr{

class ProblemDef
{
private:
    std::vector<Node> m_nodes;
    vector<vector<double>> m_distances;
    vector<Route> m_solution;
    Params m_params;
    int m_depotIndex;
    static const int NO_INDEX = -1;

    int searchForRoute(Node, int = NO_INDEX) noexcept(false);
    double calculateArrivalTime(int, Node);
    
public:
    ProblemDef(string, string, string);
    ProblemDef(string);
    ~ProblemDef();
    void generateFirstSolution();
    void writeSolutionFile();
};

}  //vr

#endif