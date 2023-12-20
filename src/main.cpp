#include "../include/vr/problemdef/problemdef.hpp"
#include "../include/hc/hcsolution.hpp"
#include <iostream>

using namespace homecare;

int main() {
    /*Params param("../config/best_conf_param.txt", "../config/parametri_ottimali.txt");
    std::cout << param.getInsertionCost(1,1,1,1) << std::endl;*/
    //ProblemDef pd("../resources/c101.txt", "../config/best_conf_param.txt", "../config/parametri_ottimali.txt");
    //pd.generateFirstSolution();
    try {
        HCSolution("../resources/rome-p240-s10-d20-i0_15.json");
    } catch (const exception& e) {
        cerr << "Errore " << e.what();
    }
   
    return 0;
}