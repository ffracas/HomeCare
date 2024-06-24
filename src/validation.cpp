/**
 * Main for solution validation
*/
#include "../include/hc/hcsolution.hpp"
#include "../include/hc/hcvalidation.hpp"
#include <iostream>

using namespace std;
using namespace homecare;

int main(int argc, char** argv) {
    try {
        string input;
        string solution;
        if (argc == 3) {
            input = argv[1];
            solution = argv[2];
        }
        else {
            input = "../resources/hr_input/000-macerata-p220-d13-i0.27-pt0.76-0.02-0.22-c5-6-4-2.json";
            solution = "../resources/hr_sol_ud/sol-000-macerata-p220-d13-i0.27-pt0.76-0.02-0.22-c5-6-4-2-2911047178.json";
        }
        HCValidation validaiton(input, solution);
        if(validaiton.checkSolution()) { cout << "Soluzione valida"; } 
        else { cout<< "Soluzione non valida\n"; }
    } catch (const exception& e) {
        cerr << "Errore " << e.what();
    }
   
    return 0;
}