#include "../include/problemdef/problemdef.hpp"
#include <iostream>

using namespace homecare;

int main() {
    /*Params param("../config/best_conf_param.txt", "../config/parametri_ottimali.txt");
    std::cout << param.getInsertionCost(1,1,1,1) << std::endl;*/
    
    ProblemDef pd("../resources/c101.txt", "../config/best_conf_param.txt", "../config/parametri_ottimali.txt");
    
    return 0;
}