//#include "../include/vr/problemdef/problemdef.hpp"
#include "../include/hc/hcsolution.hpp"
#include "../include/hc/hcvalidation.hpp"
#include <iostream>

using namespace std;
using namespace homecare;

int main() {
    /*Params param("../config/best_conf_param.txt", "../config/parametri_ottimali.txt");
    std::cout << param.getInsertionCost(1,1,1,1) << std::endl;
    ProblemDef pd("../resources/vr/c101.txt", "../config/best_conf_param.txt", "../config/parametri_ottimali.txt");
    pd.generateFirstSolution();*/
    try {
        HCSolution el("../resources/hr_input/495-rome-p240-d6-i0_1-c3-3.json");
        el.generateISolution();
    } catch (const exception& e) {
        cerr << "Errore " << e.what();
    }
   
    return 0;
}