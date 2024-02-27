//#include "../include/vr/problemdef/problemdef.hpp"
#include "../include/hc/hcsolution.hpp"
#include "../include/hc/hcvalidation.hpp"
#include <iostream>

using namespace std;
using namespace homecare;

int main(int argc, char** argv) {
    try {
        string input(argc == 2 ? argv[1] : "../resources/hr_input/000-macerata-p220-d13-i0.27-pt0.76-0.02-0.22-c5-6-4-2.json");
        HCData::setData(input);
        HCSolution el;
        el.generateISolution();
        cout << "I soluzione -> ALNS";
        el.optimizeSolution();
    } catch (const exception& e) {
        cerr << "Errore " << e.what() << '\n';
    }
   
    return 0;
}