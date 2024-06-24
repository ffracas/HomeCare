//#include "../include/vr/problemdef/problemdef.hpp"
#include "../include/hc/hcsolution.hpp"
#include "../include/hc/hcvalidation.hpp"
#include <iostream>

using namespace std;
using namespace homecare;

int main(int argc, char** argv) {
    try {
        string input(argc == 2 ? argv[1] : 
            "../resources/hr_input/001-cesena-p68-d6-i0.04-pt0.74-0.08-0.18-c6-6-3.json");
        HCData::setData(input);
        HCSolution el;
        el.generateISolution();
        cout << "I soluzione -> ALNS";
        //el.optimizeSolution();
    } catch (const exception& e) {
        cerr << "Errore " << e.what() << '\n';
    }
   
    return 0;
}