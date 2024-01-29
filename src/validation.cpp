/**
 * Main for solution validation
*/
#include "../include/hc/hcsolution.hpp"
#include "../include/hc/hcvalidation.hpp"
#include <iostream>

using namespace std;
using namespace homecare;

int main() {
    try {
        HCValidation validaiton("../resources/hr_input/495-rome-p240-d6-i0_1-c3-3.json", 
                                "./I_soluzione.json");
        if(validaiton.checkSolution()) { cout << "OK"; } 
        else { cout<< "sticazzi"; }
    } catch (const exception& e) {
        cerr << "Errore " << e.what();
    }
   
    return 0;
}