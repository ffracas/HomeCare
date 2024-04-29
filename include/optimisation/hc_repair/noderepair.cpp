#include "noderepair.hpp"

using namespace std;
using namespace homecare;

const int NodeRepair::NOT_ASSIGNED = -1;

NodeRepair::NodeRepair() : m_data(ALNSOptimisation::getInstance()) {}

NodeRepair::~NodeRepair() {}
