//#include "../include/vr/problemdef/problemdef.hpp"
#include "../include/hc/hcsolution.hpp"
#include "../include/hc/hcvalidation.hpp"
#include <iostream>

using namespace std;
using namespace homecare;

int main(int argc, char** argv) {
    try {
        string input(argc == 2 ? argv[1] : "../resources/hr_input/toy.json");
        HCData::setData(input);
        HCSolution el;
        el.generateISolution();
        cout << "Ottimizzazione dei percorsi in corso...\n";
        el.optimizeSolution();
    } catch (const exception& e) {
        cerr << "Errore " << e.what() << '\n';
    }
   
    return 0;
}