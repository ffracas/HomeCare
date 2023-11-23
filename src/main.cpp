#include "../include/Params/params.h"
#include <iostream>

int main() {
    params param_("../config/best_conf_param.txt", "../config/parametri_ottimali.txt");
    std::cout << param_.getHeuristicsCost(1,1,1,1) << std::endl;
    return 0;
}