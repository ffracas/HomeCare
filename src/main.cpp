#include "../include/problemdef/problemdef.hpp"
#include "../include/readjson/hcreader.hpp"
#include <iostream>

using namespace homecare;

int main() {
    /*Params param("../config/best_conf_param.txt", "../config/parametri_ottimali.txt");
    std::cout << param.getInsertionCost(1,1,1,1) << std::endl;*/
    //ProblemDef pd("../resources/c101.txt", "../config/best_conf_param.txt", "../config/parametri_ottimali.txt");
    //pd.generateFirstSolution();
    try {
        hcreader("../resources/reggio-emilia-p30-s6-d3-i0_1.json");
    } catch (const exception& e) {
        cerr << "Errore " << e.what();
    }
   
    return 0;
}