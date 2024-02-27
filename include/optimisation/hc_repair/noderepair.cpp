#include "noderepair.hpp"

using namespace std;
using namespace homecare;

const int NodeRepair::NOT_ASSIGNED = -1;

NodeRepair::NodeRepair(ALNSOptimisation& t_repairOps) : m_repairOps (t_repairOps) {}

NodeRepair::~NodeRepair() {}
