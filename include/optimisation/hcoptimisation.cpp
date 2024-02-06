#include "hcoptimisation.hpp"

using namespace std;
using namespace homecare;

const int HCOptimisation::ELEMENT_TO_DESTROY = 7;
const int HCOptimisation::PERIOD = 10;

HCOptimisation::HCOptimisation(HCData t_data, std::vector<Route> t_routes, double t_cost) 
        : m_initial (t_routes), m_ops(t_data, t_routes, t_cost) {}

HCOptimisation::~HCOptimisation() {}


