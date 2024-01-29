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
        HCValidation validaiton("../resources/hr_input/000-udine-p10-d2-i0_1-c3-3.json", 
                                "../resources/hr_sol/sol-000-udine-p10-d2-i0.1-c3-3-1584437200.json");
        if(validaiton.checkSolution()) { cout << "OK"; } 
        else { cout<< "sticazzi"; }
    } catch (const exception& e) {
        cerr << "Errore " << e.what();
    }
   
    return 0;
}