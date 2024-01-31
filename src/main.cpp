//#include "../include/vr/problemdef/problemdef.hpp"
#include "../include/hc/hcsolution.hpp"
#include "../include/hc/hcvalidation.hpp"
#include <iostream>

using namespace std;
using namespace homecare;

int main(int argc, char** argv) {
    /*Params param("../config/best_conf_param.txt", "../config/parametri_ottimali.txt");
    std::cout << param.getInsertionCost(1,1,1,1) << std::endl;
    ProblemDef pd("../resources/vr/c101.txt", "../config/best_conf_param.txt", "../config/parametri_ottimali.txt");
    pd.generateFirstSolution();*/
    try {
        string input(argc == 2 ? argv[1] : "../resources/hr_input/000-macerata-p220-d13-i0.27-pt0.76-0.02-0.22-c5-6-4-2.json");
        HCSolution el(input);
        el.generateISolution();
    } catch (const exception& e) {
        cerr << "Errore " << e.what();
    }
   
    return 0;
}